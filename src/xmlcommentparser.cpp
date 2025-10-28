#include "xmlcommentparser.hpp"
#include <stdexcept>

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

XmlNode XmlCommentParser::parse() {
    auto& buffer { getBuffer() };

    // Expect "<!--"
    if (buffer.read() != '-' || !buffer.canRead() || buffer.read() != '-') {
        throw runtime_error("Invalid comment syntax");
    }

    string comment { };
    bool foundEnd { false };

    while (buffer.canRead()) {
        auto c = buffer.read();        
        if (c == '-' && buffer.canRead() && buffer.peek() == '-') {
            buffer.read(); // consume second '-'
            
            if (buffer.canRead() && buffer.peek() == '>') {
                buffer.read(); // consume '>'
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