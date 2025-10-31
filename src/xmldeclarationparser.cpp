module cfdi.xml:xmldeclarationparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;
import :xmldeclarationparser;

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlDeclarationParser;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlDeclarationParser::XmlDeclarationParser(XmlBuffer& buffer)
    : XmlFragmentParser(buffer) 
{

}

XmlNode XmlDeclarationParser::parse() {
    auto& buffer { getBuffer() };

    // We're already past "<?xml"    
    buffer.consume(3);

    // Parse attributes (version, encoding, standalone)
    auto attributes { parseAttributes() };
    buffer.skipWhiteSpace();

    // Expect "?>"
    if (!buffer.canRead() || buffer.read() != '?' || !buffer.canRead() || buffer.read() != '>') {
        throw runtime_error("Invalid XML declaration syntax");
    }

    XmlNode node { 
        .nodeType = XmlNodeType::XmlDeclaration,
        .name = "xml",
        .attributes = attributes,
    };

    return node;
}
