#include "xmldoctypeparser.hpp"
#include <stdexcept>

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlDocTypeParser;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlNode XmlDocTypeParser::parse(XmlBuffer& buffer) {
    // Expect "<!DOCTYPE"
    string doctype = "DOCTYPE";
    for (char c : doctype) {
        if (!buffer.canRead() || buffer.read() != c) {
            throw runtime_error("Invalid DOCTYPE syntax");
        }
    }

    string dtd { };
    int bracketDepth { 0 };
    bool foundEnd = { false };

    while (buffer.canRead()) {
        auto c = buffer.read();        
        if (c == '[') {
            bracketDepth++;
            dtd += c;
        } else if (c == ']') {
            bracketDepth--;
            dtd += c;
        } else if (c == '>' && bracketDepth == 0) {
            foundEnd = true;
            break;
        } else {
            dtd += c;
        }
    }

    if (!foundEnd) {
        throw runtime_error("Unclosed DOCTYPE");
    }

    XmlNode node { };
    node.nodeType = XmlNodeType::DocumentType;
    node.name = "DOCTYPE";
    node.value = dtd;
    
    return node;
}