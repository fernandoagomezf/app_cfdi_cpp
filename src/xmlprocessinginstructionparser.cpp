module cfdi.xml:xmlprocessinginstructionparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;
import :xmlprocessinginstructionparser;

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;
using cfdi::XmlProcessingInstructionParser;

XmlProcessingInstructionParser::XmlProcessingInstructionParser(XmlBuffer& buffer)
    : XmlFragmentParser(buffer)
{

}

XmlNode XmlProcessingInstructionParser::parse() {
    auto& buffer { getBuffer() };
    string target = { parseName() };
    
    if (target.empty()) {
        throw runtime_error("Invalid processing instruction target");
    }

    buffer.skipWhiteSpace();

    string data { };
    bool foundEnd { false };

    while (buffer.canRead()) {
        auto c = buffer.read();        
        if (c == '?' && buffer.canRead() && buffer.peek() == '>') {
            buffer.read(); // consume '>'
            foundEnd = true;
            break;
        } else {
            data += c;
        }
    }

    if (!foundEnd) {
        throw runtime_error("Unclosed processing instruction");
    }

    XmlNode node { 
        .nodeType = XmlNodeType::ProcessingInstruction,
        .name = target,
        .value = data,
    };

    return node;
}
