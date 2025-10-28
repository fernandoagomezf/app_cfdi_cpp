#include "xmlelementparser.hpp"
#include <stdexcept>

using std::runtime_error;
using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlElementParser;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;

XmlNode XmlElementParser::parse(XmlBuffer& buffer) {
    if (buffer.peek() == '/') {
        buffer.consume();
        return parseElementEnd(buffer);
    } else {
        return parseElementStart(buffer);
    }
}

XmlNode XmlElementParser::parseElementStart(XmlBuffer& buffer){
    string name { parseName(buffer) };
    bool isEmpty { false };
    
    if (name.empty()) {
        throw runtime_error("Invalid element name");
    }

    // Parse attributes
    auto attributes = parseAttributes(buffer);
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

XmlNode XmlElementParser::parseElementEnd(XmlBuffer& buffer){
    string name { parseName(buffer) };
        
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
