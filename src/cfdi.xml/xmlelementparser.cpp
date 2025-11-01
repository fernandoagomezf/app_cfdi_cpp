module cfdi.xml:xmlelementparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;
import :xmlelementparser;

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlElementParser;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlElementParser::XmlElementParser(XmlBuffer& buffer)
    : XmlFragmentParser(buffer)
{

}

XmlNode XmlElementParser::parse() {     // parse XML elements: <Element attribute='value' ...> ... </Element>
    auto& buffer { getBuffer() };

    if (buffer.peek() == '/') {         // check whether we're dealing with the start tag, e.g. <Element>
        buffer.consume();
        return parseElementEnd();
    } else {                            // otherwise we're dealing with the closing tag, e.g. </Element>
        return parseElementStart();
    }
}

XmlNode XmlElementParser::parseElementStart(){
    auto& buffer { getBuffer() };
    string name { parseName() };
    bool isEmpty { false };
    
    if (name.empty()) {
        throw runtime_error("Invalid element name");
    }

    auto attributes = parseAttributes();    // parse to find the element's attributes
    buffer.skipWhiteSpace();

    if (buffer.canRead() && buffer.peek() == '/') {     // check for a special case of empty tags, e.g. <Element />
        buffer.consume(); // consume '/'
        isEmpty = true;
    }

    if (!buffer.canRead() || buffer.read() != '>') {    // check the tag is well formed
        throw runtime_error("Invalid element tag syntax");
    }

    auto [prefix, localName] = splitQualifiedName(name);    // elements can have a qualified name with a namespace, e.g. <cfdi:Tax>
    
    XmlNode node { 
        .nodeType = XmlNodeType::Element,
        .name = name,
        .localName = localName,
        .prefix = prefix,
        .isEmpty = isEmpty,
        .attributes = attributes,
    };
    
    return node;
}

XmlNode XmlElementParser::parseElementEnd(){
    auto& buffer { getBuffer() };
    string name { parseName() };
        
    if (name.empty()) {
        throw runtime_error("Invalid end element tag syntax");
    }

    buffer.skipWhiteSpace();

    if (!buffer.canRead() || buffer.read() != '>') {    // check the tag is well formed
        throw runtime_error("Invalid end element tag syntax");
    }

    auto [prefix, localName] = splitQualifiedName(name); // elements can have a qualified name with a namespace, e.g. <cfdi:Tax>

    XmlNode node { 
        .nodeType = XmlNodeType::EndElement,
        .name = name,
        .localName = localName,
        .prefix = prefix
    };

    return node;
}
