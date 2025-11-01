module cfdi.xml:xmldoctypeparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;
import :xmldoctypeparser;

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlDocTypeParser;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlDocTypeParser::XmlDocTypeParser(XmlBuffer& buffer)
    : XmlFragmentParser(buffer) 
{
}

XmlNode XmlDocTypeParser::parse() {     // parse the section <!DOCTYPE ... >
    auto& buffer { getBuffer() };

    // make sure we're dealing with a doctype
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
        throw runtime_error("Invalid DOCTYPE syntax");
    }

    XmlNode node { 
        .nodeType = XmlNodeType::DocumentType,
        .name = "DOCTYPE",
        .value = dtd,
    };
    
    return node;
}