#include "xmlfragmentparser.hpp"
#include <string>

using std::string;
using std::isalpha;
using std::isalnum;
using cfdi::XmlBuffer;
using cfdi::XmlFragmentParser;

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