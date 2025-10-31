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
    
    // XML name must start with letter, underscore, or colon
    if (!isalpha(static_cast<unsigned char>(c)) && c != '_' && c != ':') {
        return name;
    }

    while (_buffer.canRead()) {
        c = _buffer.peek();
        
        // Valid name characters
        if (isalnum(static_cast<unsigned char>(c)) || 
            c == '_' || c == ':' || c == '-' || c == '.') {
            name += _buffer.read();
        } else {
            break;
        }
    }

    return name;
}

map<string, string> XmlFragmentParser::parseAttributes() {
    map<string, string>attributes { };

    while (_buffer.canRead()) {
        _buffer.skipWhiteSpace();

        auto c = _buffer.peek();
        
        // Check for end of tag
        if (c == '>' || c == '/' || c == '?') {
            break;
        }

        // Read attribute name
        string attrName { parseName() };        
        if (attrName.empty()) {
            break;
        }

        _buffer.skipWhiteSpace();

        // Expect '='
        if (!_buffer.canRead() || _buffer.read() != '=') {
            throw runtime_error("Expected '=' after attribute name");
        }

        _buffer.skipWhiteSpace();

        // Read attribute value
        string attrValue { parseAttributeValue() };
        attributes[attrName] = attrValue;
    }

    return attributes;
}

string XmlFragmentParser::parseAttributeValue() {
    if (!_buffer.canRead()) {
        throw runtime_error("Expected attribute value");
    }

    auto quote = _buffer.read();    
    if (quote != '"' && quote != '\'') {
        throw runtime_error("Attribute value must be quoted");
    }

    string value { };
    while (_buffer.canRead()) {
        auto c = _buffer.read();        
        if (c == quote) {
            break;
        } else if (c == '&') {
            // Handle entity references
            string entity { };
            while (_buffer.canRead() && _buffer.peek() != ';') {
                entity += _buffer.read();
            }
            if (_buffer.canRead()) {
                _buffer.read(); // consume ';'
            }
            
            // Decode common entities
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
                // Numeric character reference
                int code { 0 };
                if (entity.length() > 1 && entity[1] == 'x') {
                    // Hexadecimal
                    code = stoi(entity.substr(2), nullptr, 16);
                } else {
                    // Decimal
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