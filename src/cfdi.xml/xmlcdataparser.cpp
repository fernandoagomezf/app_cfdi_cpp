module cfdi.xml:xmlcdataparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;
import :xmlcdataparser;

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlCDataParser;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlCDataParser::XmlCDataParser(XmlBuffer& buffer)
    : XmlFragmentParser(buffer) 
{
}


XmlNode XmlCDataParser::parse() {       // parse the section "<![CDATA[ ... ]]>"
    auto& buffer { getBuffer() };


    string expected = { "[CDATA[" };    // make sure we're dealing with a CDATA section
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
            buffer.consume(); // consume second ']'
            
            if (buffer.canRead() && buffer.peek() == '>') {
                buffer.consume(); // consume '>'
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
        throw runtime_error("Invalid CDATA syntax");
    }

    XmlNode node { 
        .nodeType = XmlNodeType::CDATA,
        .value = cdata,
    };

    return node;
}