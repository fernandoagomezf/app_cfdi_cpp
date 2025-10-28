#include "xmlreader.hpp"
#include "xmltextparser.hpp"
#include "xmlcommentparser.hpp"
#include "xmlcdataparser.hpp"
#include "xmlprocessinginstructionparser.hpp"
#include "xmldeclarationparser.hpp"
#include "xmldoctypeparser.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

using std::isalnum;
using std::isalpha;
using std::map;
using std::ostringstream;
using std::runtime_error;
using std::stoi;
using std::string;
using std::string_view;
using std::unique_ptr;
using cfdi::XmlCDataParser;
using cfdi::XmlCommentParser;
using cfdi::XmlDeclarationParser;
using cfdi::XmlDocTypeParser;
using cfdi::XmlNodeType;
using cfdi::XmlNode;
using cfdi::XmlProcessingInstructionParser;
using cfdi::XmlReader;
using cfdi::XmlTextParser;

XmlReader::XmlReader(string_view xml)
    : _buffer { xml, 0 },
      _eof { false },
      _nodeType { XmlNodeType::None },
      _depth { 0 },
      _isEmptyElement { false },
      _readContent { false }
{
    _eof = xml.empty();
}

XmlReader::XmlReader(istream& stream)
    : _buffer { { }, 0  },
      _eof { false },
      _nodeType { XmlNodeType::None },
      _depth { 0 },
      _isEmptyElement { false },
      _readContent { false }
{
    ostringstream ss;
    ss << stream.rdbuf();
    auto str = ss.str();
    _buffer = { str, 0 };
    _eof = str.empty();
}

bool XmlReader::read() {
    if (_eof) {
        return false;
    }

    _attributes.clear();
    _isEmptyElement = false;
    _name.clear();
    _localName.clear();
    _prefix.clear();
    _value.clear();

    auto result { readInternal() };

    return result;
}

XmlNode XmlReader::current() const {
    return {
        _nodeType, 
        _name, 
        _localName, 
        _prefix, 
        _value, 
        _depth,
        _isEmptyElement,
        _eof, 
        _attributes
    };
}


bool XmlReader::readInternal() {
    _buffer.skipWhiteSpace();

    if (!_buffer.canRead()) {
        _eof = true;
        _nodeType = XmlNodeType::None;
        return false;
    }

    auto c = _buffer.peek();

    // Check for markup
    if (c == '<') {
        _buffer.read(); // consume '<'        
        if (!_buffer.canRead()) {
            throw runtime_error("Unexpected end of XML");
        }

        auto next = _buffer.peek();
        if (next == '!') {
            _buffer.read(); // consume '!'            
            if (!_buffer.canRead()) {
                throw runtime_error("Unexpected end of XML");
            }

            auto third = _buffer.peek();            
            if (third == '-') {
                parseComment();
            } else if (third == '[') {
                parseCDATA();
            } else if (third == 'D') {
                parseDocumentType();
            } else {
                throw runtime_error("Invalid XML syntax");
            }
        } else if (next == '?') {
            _buffer.read(); // consume '?'
            
            // Check if it's XML declaration
            if (_buffer.position() + 3 <= _buffer.length()) {
                string ss { _buffer.substr(3) };
                if (ss == "xml")  {
                    parseXmlDeclaration();
                } else {
                    parseProcessingInstruction();    
                }
            } else {
                parseProcessingInstruction();
            }
        } else if (next == '/') {
            _buffer.read(); // consume '/'
            parseEndElement();
        } else {
            parseElement();
        }

        return true;
    } else {
        // Text content
        parseText();
        return true;
    }
}

void XmlReader::parseElement() {
    string name { readName() };
    
    if (name.empty()) {
        throw std::runtime_error("Invalid element name");
    }

    // Parse attributes
    parseAttributes();
    _buffer.skipWhiteSpace();

    // Check for empty element
    if (_buffer.canRead() && _buffer.peek() == '/') {
        _buffer.read(); // consume '/'
        _isEmptyElement = true;
    }

    // Expect closing '>'
    if (!_buffer.canRead() || _buffer.read() != '>') {
        throw std::runtime_error("Expected '>' to close element tag");
    }

    if (!_isEmptyElement) {
        _depth++;
    }

    string prefix { };
    string localName { };
    splitQualifiedName(name, prefix, localName);    
    setNodeInfo(XmlNodeType::Element, name, "");
    _prefix = prefix;
    _localName = localName;
}

void XmlReader::parseEndElement() {
    string name = readName();
    
    if (name.empty()) {
        throw runtime_error("Invalid end element name");
    }

    _buffer.skipWhiteSpace();

    // Expect closing '>'
    if (!_buffer.canRead() || _buffer.read() != '>') {
        throw runtime_error("Expected '>' to close end element tag");
    }

    _depth--;
    if (_depth < 0) {
        _depth = 0;
    }

    string prefix { };
    string localName { };
    splitQualifiedName(name, prefix, localName);    
    setNodeInfo(XmlNodeType::EndElement, name, "");
    _prefix = prefix;
    _localName = localName;
}

void XmlReader::parseText() {
    XmlTextParser parser { };
    XmlNode node { parser.parse(_buffer) };
    
    _nodeType = node.nodeType;
    _name = node.name;
    _value = node.value;
}

void XmlReader::parseComment() {
    XmlCommentParser parser { };
    XmlNode node { parser.parse(_buffer) };

    _nodeType = node.nodeType;
    _name = node.name;
    _value = node.value;
}

void XmlReader::parseCDATA() {
    XmlCDataParser parser { };
    XmlNode node { parser.parse(_buffer) };

    _nodeType = node.nodeType;
    _name = node.name;
    _value = node.value;
}

void XmlReader::parseProcessingInstruction() {
    XmlProcessingInstructionParser parser { };
    XmlNode node { parser.parse(_buffer) };
    
    _nodeType = node.nodeType;
    _name = node.name;
    _value = node.value;
}

void XmlReader::parseXmlDeclaration() {
    XmlDeclarationParser parser { };
    XmlNode node { parser.parse(_buffer) };

    _nodeType = node.nodeType;
    _name = node.name;
    _value = node.value;
    _attributes = node.attributes;
}

void XmlReader::parseDocumentType() {
    XmlDeclarationParser parser { };
    XmlNode node { parser.parse(_buffer) };

    _nodeType = node.nodeType;
    _name = node.name;
    _value = node.value;
}

void XmlReader::parseAttributes() {
    _attributes.clear();

    while (_buffer.canRead()) {
        _buffer.skipWhiteSpace();

        auto c = _buffer.peek();
        
        // Check for end of tag
        if (c == '>' || c == '/' || c == '?') {
            break;
        }

        // Read attribute name
        string attrName { readName() };        
        if (attrName.empty()) {
            break;
        }

        _buffer.skipWhiteSpace();

        // Expect '='
        if (!_buffer.canRead() || _buffer.read() != '=') {
            throw runtime_error("Expected '=' after attribute name");
        }

        _buffer.skipWhiteSpace();

        // Read attribute value
        string attrValue { readAttributeValue() };

        _attributes[attrName] = attrValue;
    }
}

string XmlReader::readName() {
    string name { };
    if (!_buffer.canRead()) {
        return name;
    }

    auto c = _buffer.peek();
    
    // XML name must start with letter, underscore, or colon
    if (!isalpha(static_cast<unsigned char>(c)) && c != '_' && c != ':') {
        return name;
    }

    while (_buffer.canRead()) {
        c = _buffer.peek();
        
        // Valid name characters
        if (isalnum(static_cast<unsigned char>(c)) || 
            c == '_' || c == ':' || c == '-' || c == '.') {
            name += _buffer.read();
        } else {
            break;
        }
    }

    return name;
}

string XmlReader::readAttributeValue() {
    if (!_buffer.canRead()) {
        throw runtime_error("Expected attribute value");
    }

    char quote = _buffer.read();    
    if (quote != '"' && quote != '\'') {
        throw runtime_error("Attribute value must be quoted");
    }

    string value { };
    while (_buffer.canRead()) {
        auto c = _buffer.read();        
        if (c == quote) {
            break;
        } else if (c == '&') {
            // Handle entity references
            string entity { };
            while (_buffer.canRead() && _buffer.peek() != ';') {
                entity += _buffer.read();
            }
            if (_buffer.canRead()) {
                _buffer.read(); // consume ';'
            }
            
            // Decode common entities
            if (entity == "lt") {
                value += '<';
            } else if (entity == "gt") {
                value += '>';
            } else if (entity == "amp") {
                value += '&';
            } else if (entity == "quot") {
                value += '"';
            } else if (entity == "apos") {
                value += '\'';
            } else if (!entity.empty() && entity[0] == '#') {
                // Numeric character reference
                int code { 0 };
                if (entity.length() > 1 && entity[1] == 'x') {
                    // Hexadecimal
                    code = stoi(entity.substr(2), nullptr, 16);
                } else {
                    // Decimal
                    code = stoi(entity.substr(1));
                }
                value += static_cast<char>(code);
            } else {
                value += '&' + entity + ';';
            }
        } else {
            value += c;
        }
    }

    return value;
}

void XmlReader::setNodeInfo(XmlNodeType type, const string& name, const string& value) {
    _nodeType = type;
    _name = name;
    _value = value;
}

void XmlReader::splitQualifiedName(const string& qualifiedName, string& prefix, string& localName) {
    size_t colonPos = qualifiedName.find(':');
    
    if (colonPos != string::npos) {
        prefix = qualifiedName.substr(0, colonPos);
        localName = qualifiedName.substr(colonPos + 1);
    } else {
        prefix.clear();
        localName = qualifiedName;
    }
}

