#include "xmltextparser.hpp"

using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;
using cfdi::XmlTextParser;

XmlNode XmlTextParser::parse(XmlBuffer& buffer) {
    string text { };    
    while (buffer.canRead() && buffer.peek() != '<') {
        auto c = buffer.read();
        
        // Handle entity references
        if (c == '&') {
            string entity { };
            while (buffer.canRead() && buffer.peek() != ';') {
                entity += buffer.read();
            }
            if (buffer.canRead()) {
                buffer.read(); // consume ';'
            }
            
            // Decode common entities
            if (entity == "lt") {
                text += '<';
            } else if (entity == "gt") {
                text += '>';
            } else if (entity == "amp") {
                text += '&';
            } else if (entity == "quot") {
                text += '"';
            } else if (entity == "apos") {
                text += '\'';
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
                text += static_cast<char>(code);
            } else {
                text += '&' + entity + ';';
            }
        } else {
            text += c;
        }
    }

    // Check if it's all whitespace
    bool allWhitespace = true;
    for (char c : text) {
        if (!XmlBuffer::isWhiteSpace(c)) {
            allWhitespace = false;
            break;
        }
    }

    XmlNode node { };
    node.nodeType = allWhitespace && !text.empty() ? XmlNodeType::Whitespace : XmlNodeType::Text;
    node.value = text;
    
    return node;
}