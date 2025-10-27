#include "xmlcdataparser.hpp"
#include <stdexcept>
#include <string>

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlCDataParser;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlNode XmlCDataParser::parse(XmlBuffer& buffer) {
    // Expect "<![CDATA["
    string expected = { "[CDATA[" };
    for (auto expectedChar : expected) {
        if (!buffer.canRead() || buffer.read() != expectedChar) {
            throw runtime_error("Invalid CDATA syntax");
        }
    }

    string cdata { };
    bool foundEnd { false };

    while (buffer.canRead()) {
        auto c = buffer.read();        
        if (c == ']' && buffer.canRead() && buffer.peek() == ']') {
            buffer.read(); // consume second ']'
            
            if (buffer.canRead() && buffer.peek() == '>') {
                buffer.read(); // consume '>'
                foundEnd = true;
                break;
            } else {
                cdata += "]]";
            }
        } else {
            cdata += c;
        }
    }

    if (!foundEnd) {
        throw runtime_error("Unclosed CDATA section");
    }

    XmlNode node { };
    node.nodeType = XmlNodeType::CDATA;
    node.value = cdata;

    return node;
}