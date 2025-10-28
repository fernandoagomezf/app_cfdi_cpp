#include "xmlelementparser.hpp"
#include <stdexcept>

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

XmlNode XmlElementParser::parse() {
    auto& buffer { getBuffer() };

    if (buffer.peek() == '/') {
        buffer.consume();
        return parseElementEnd();
    } else {
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

    // Parse attributes
    auto attributes = parseAttributes();
    buffer.skipWhiteSpace();

    // Check for empty element
    if (buffer.canRead() && buffer.peek() == '/') {
        buffer.read(); // consume '/'
        isEmpty = true;
    }

    // Expect closing '>'
    if (!buffer.canRead() || buffer.read() != '>') {
        throw runtime_error("Expected '>' to close element tag");
    }

    auto [prefix, localName] = splitQualifiedName(name);
    
    XmlNode node { };
    node.nodeType = XmlNodeType::Element;
    node.name = name;
    node.prefix = prefix;
    node.localName = localName;
    node.attributes = attributes;
    node.isEmpty = isEmpty;
    
    return node;
}

XmlNode XmlElementParser::parseElementEnd(){
    auto& buffer { getBuffer() };
    string name { parseName() };
        
    if (name.empty()) {
        throw runtime_error("Invalid end element name");
    }

    buffer.skipWhiteSpace();

    // Expect closing '>'
    if (!buffer.canRead() || buffer.read() != '>') {
        throw runtime_error("Expected '>' to close end element tag");
    }

    auto [prefix, localName] = splitQualifiedName(name);

    XmlNode node { };
    node.nodeType = XmlNodeType::EndElement;
    node.name = name;
    node.prefix = prefix;
    node.localName = localName;

    return node;
}
