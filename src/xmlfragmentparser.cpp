#include "xmlfragmentparser.hpp"
#include <cmath>
#include <map>
#include <string>
#include <stdexcept>

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

XmlFragmentParser::XmlFragmentParser()
{    
}

string XmlFragmentParser::parseName(XmlBuffer& buffer) {
    string name { };
    if (!buffer.canRead()) {
        return name;
    }

    auto c = buffer.peek();
    
    // XML name must start with letter, underscore, or colon
    if (!isalpha(static_cast<unsigned char>(c)) && c != '_' && c != ':') {
        return name;
    }

    while (buffer.canRead()) {
        c = buffer.peek();
        
        // Valid name characters
        if (isalnum(static_cast<unsigned char>(c)) || 
            c == '_' || c == ':' || c == '-' || c == '.') {
            name += buffer.read();
        } else {
            break;
        }
    }

    return name;
}

map<string, string> XmlFragmentParser::parseAttributes(XmlBuffer& buffer) {
    map<string, string>attributes { };

    while (buffer.canRead()) {
        buffer.skipWhiteSpace();

        auto c = buffer.peek();
        
        // Check for end of tag
        if (c == '>' || c == '/' || c == '?') {
            break;
        }

        // Read attribute name
        string attrName { parseName(buffer) };        
        if (attrName.empty()) {
            break;
        }

        buffer.skipWhiteSpace();

        // Expect '='
        if (!buffer.canRead() || buffer.read() != '=') {
            throw runtime_error("Expected '=' after attribute name");
        }

        buffer.skipWhiteSpace();

        // Read attribute value
        string attrValue { readAttributeValue(buffer) };

        attributes[attrName] = attrValue;
    }

    return attributes;
}

string XmlFragmentParser::readAttributeValue(XmlBuffer& buffer) {
    if (!buffer.canRead()) {
        throw runtime_error("Expected attribute value");
    }

    auto quote = buffer.read();    
    if (quote != '"' && quote != '\'') {
        throw runtime_error("Attribute value must be quoted");
    }

    string value { };
    while (buffer.canRead()) {
        auto c = buffer.read();        
        if (c == quote) {
            break;
        } else if (c == '&') {
            // Handle entity references
            string entity { };
            while (buffer.canRead() && buffer.peek() != ';') {
                entity += buffer.read();
            }
            if (buffer.canRead()) {
                buffer.read(); // consume ';'
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