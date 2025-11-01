module cfdi.xml:xmlfragmentparser;

import std;
import :xmlbuffer;
import :xmlnode;
import :xmlfragmentparser;

using std::isalpha;
using std::isalnum;
using std::make_pair;
using std::map;
using std::max;
using std::pair;
using std::runtime_error;
using std::string;
using std::string_view;
using cfdi::XmlBuffer;
using cfdi::XmlFragmentParser;

XmlFragmentParser::XmlFragmentParser(XmlBuffer& buffer)
    : _buffer { buffer }
{
}

XmlBuffer& XmlFragmentParser::getBuffer() {
    return _buffer;
}

string XmlFragmentParser::parseName() {
    string name { };
    if (!_buffer.canRead()) {
        return name;
    }

    auto c = _buffer.peek();
    
    if (!isalpha(static_cast<unsigned char>(c)) && c != '_' && c != ':') { // a valid name must start with letter, underscore, or colon
        return name;
    }

    while (_buffer.canRead()) {
        c = _buffer.peek();        
        if (isalnum(static_cast<unsigned char>(c)) || c == '_' || c == ':' || c == '-' || c == '.') {
            name += _buffer.read();
        } else {
            break;  // found an invalid character for a name, stop right here
        }
    }

    return name;
}

map<string, string> XmlFragmentParser::parseAttributes() {
    map<string, string> attributes { };

    while (_buffer.canRead()) {
        _buffer.skipWhiteSpace();

        auto c = _buffer.peek();
        
        if (c == '>' || c == '/' || c == '?') { // if one of these characters is found, we reached the end of attributes for this element
            break;
        }

        string attrName { parseName() };        
        if (attrName.empty()) {
            break;
        }
        _buffer.skipWhiteSpace();

        if (!_buffer.canRead() || _buffer.read() != '=') {  // attriutes follow the pattern attribute='value' so find the = character
            throw runtime_error("Invalid attribute syntax");
        }
        _buffer.skipWhiteSpace();

        string attrValue { parseAttributeValue() }; // we have the name, now extract the value
        attributes[attrName] = attrValue;
    }

    return attributes;
}

string XmlFragmentParser::parseAttributeValue() {
    if (!_buffer.canRead()) {
        throw runtime_error("Expected attribute value");
    }

    auto quote = _buffer.read();    
    if (quote != '"' && quote != '\'') {    // attributes follow the pattern name='value' or name="value", so check for both chars
        throw runtime_error("Invalid attribute syntax");
    }

    string value { };
    while (_buffer.canRead()) {
        auto c = _buffer.read();        
        if (c == quote) {
            break;
        } else if (c == '&') {
            // deal with entities, they follow the pattern &...; as in &amp; &quot; etc.
            string entity { };
            while (_buffer.canRead() && _buffer.peek() != ';') {
                entity += _buffer.read();
            }
            if (_buffer.canRead()) {
                _buffer.consume(); // consume ';'
            }
            
            // handle common known entities
            if (entity == "lt") {
                value += '<';
            } else if (entity == "gt") {
                value += '>';
            } else if (entity == "amp") {
                value += '&';
            } else if (entity == "quot") {
                value += '"';
            } else if (entity == "apos") {
                value += '\'';
            } else if (!entity.empty() && entity[0] == '#') {
                // otherwise it's a numerical entity (e.g. a unicode entity)
                int code { 0 };
                if (entity.length() > 1 && entity[1] == 'x') {  // base-16 number so skip the hex format
                    code = stoi(entity.substr(2), nullptr, 16);
                } else { 
                    code = stoi(entity.substr(1));
                }
                value += static_cast<char>(code);
            } else {
                value += '&' + entity + ';';
            }
        } else {
            value += c;
        }
    }

    return value;
}

pair<string, string> XmlFragmentParser::splitQualifiedName(string_view name) const {
    size_t colonPos = name.find(':');
    
    string prefix { };
    string localName { };

    if (colonPos != string::npos) {
        prefix = name.substr(0, colonPos);
        localName = name.substr(colonPos + 1);
    } else {
        localName = name;
    }

    return make_pair(prefix, localName);
}