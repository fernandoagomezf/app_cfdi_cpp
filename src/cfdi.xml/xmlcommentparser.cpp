module cfdi.xml:xmlcommentparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;
import :xmlcommentparser;

using std::runtime_error;
using std::string;
using cfdi::XmlCommentParser;
using cfdi::XmlBuffer;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlCommentParser::XmlCommentParser(XmlBuffer& buffer)
    : XmlFragmentParser(buffer)
{
    
}

XmlNode XmlCommentParser::parse() {     // parse the section "<!-- ... -->"
    auto& buffer { getBuffer() };

    // make sure we're dealing with a comment
    if (buffer.read() != '-' || !buffer.canRead() || buffer.read() != '-') {
        throw runtime_error("Invalid comment syntax");
    }

    string comment { };
    bool foundEnd { false };

    while (buffer.canRead()) {
        auto c = buffer.read();        
        if (c == '-' && buffer.canRead() && buffer.peek() == '-') {
            buffer.consume(); // consume second '-'
            
            if (buffer.canRead() && buffer.peek() == '>') {
                buffer.consume(); // consume '>'
                foundEnd = true;
                break;
            } else {
                comment += "--";
            }
        } else {
            comment += c;
        }
    }

    if (!foundEnd) {
        throw runtime_error("Unclosed comment");
    }

    XmlNode node { 
        .nodeType = XmlNodeType::Comment,
        .value = comment,
    };

    return node;
}