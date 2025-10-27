#include "xmlreader.hpp"
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
using cfdi::XmlNodeType;
using cfdi::XmlNode;
using cfdi::XmlReader;

XmlReader::XmlReader(string_view xml)
    : _xml { xml },
      _position { 0 },
      _eof { false },
      _nodeType { XmlNodeType::None },
      _depth { 0 },
      _isEmptyElement { false },
      _readContent { false }
{
    _eof = _xml.empty();
}

XmlReader::XmlReader(istream& stream)
    : _xml { },
      _position { 0 },
      _eof { false },
      _nodeType { XmlNodeType::None },
      _depth { 0 },
      _isEmptyElement { false },
      _readContent { false }
{
    ostringstream ss;
    ss << stream.rdbuf();
    _xml = ss.str();
    _eof = _xml.empty();
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

bool XmlReader::canRead() const {
    return _position < _xml.length();
}

char XmlReader::peekChar() {
    auto ch = '\0';
    if (_position < _xml.length()) {
        ch = _xml[_position];
    }
    return ch;
}

char XmlReader::readChar() {
    auto ch = '\0';
    if (_position < _xml.length()) {
        ch = _xml[_position];
        ++_position;
    }
    return ch;
}

void XmlReader::consumeChar() {
    if (_position < _xml.length()) {
        ++_position;
    }
}

bool XmlReader::readInternal() {
    skipWhitespace();

    if (!canRead()) {
        _eof = true;
        _nodeType = XmlNodeType::None;
        return false;
    }

    auto c = peekChar();

    // Check for markup
    if (c == '<') {
        readChar(); // consume '<'        
        if (!canRead()) {
            throw runtime_error("Unexpected end of XML");
        }

        auto next = peekChar();
        if (next == '!') {
            readChar(); // consume '!'            
            if (!canRead()) {
                throw runtime_error("Unexpected end of XML");
            }

            auto third = peekChar();            
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
            readChar(); // consume '?'
            
            // Check if it's XML declaration
            if (_position + 3 <= _xml.length() && 
                _xml.substr(_position, 3) == "xml") {
                parseXmlDeclaration();
            } else {
                parseProcessingInstruction();
            }
        } else if (next == '/') {
            readChar(); // consume '/'
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

void XmlReader::skipWhitespace() {
    while (canRead() && isWhitespace(peekChar())) {
        readChar();
    }
}

bool XmlReader::isWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}


void XmlReader::parseElement() {
    string name { readName() };
    
    if (name.empty()) {
        throw std::runtime_error("Invalid element name");
    }

    // Parse attributes
    parseAttributes();
    skipWhitespace();

    // Check for empty element
    if (canRead() && peekChar() == '/') {
        readChar(); // consume '/'
        _isEmptyElement = true;
    }

    // Expect closing '>'
    if (!canRead() || readChar() != '>') {
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

    skipWhitespace();

    // Expect closing '>'
    if (!canRead() || readChar() != '>') {
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
    string text;
    
    while (canRead() && peekChar() != '<') {
        char c = readChar();
        
        // Handle entity references
        if (c == '&') {
            string entity;
            while (canRead() && peekChar() != ';') {
                entity += readChar();
            }
            if (canRead()) {
                readChar(); // consume ';'
            }
            
            // Decode common entities
            if (entity == "lt") {
                text += '<';
            } else if (entity == "gt") {
                text += '>';
            } else if (entity == "amp") {
                text += '&';
            } else if (entity == "quot") {
                text += '"';
            } else if (entity == "apos") {
                text += '\'';
            } else if (!entity.empty() && entity[0] == '#') {
                // Numeric character reference
                int code = 0;
                if (entity.length() > 1 && entity[1] == 'x') {
                    // Hexadecimal
                    code = stoi(entity.substr(2), nullptr, 16);
                } else {
                    // Decimal
                    code = stoi(entity.substr(1));
                }
                text += static_cast<char>(code);
            } else {
                text += '&' + entity + ';';
            }
        } else {
            text += c;
        }
    }

    // Check if it's all whitespace
    bool allWhitespace = true;
    for (char c : text) {
        if (!isWhitespace(c)) {
            allWhitespace = false;
            break;
        }
    }

    if (allWhitespace && !text.empty()) {
        setNodeInfo(XmlNodeType::Whitespace, "", text);
    } else {
        setNodeInfo(XmlNodeType::Text, "", text);
    }
}

void XmlReader::parseComment() {
    // Expect "<!--"
    if (readChar() != '-' || !canRead() || readChar() != '-') {
        throw runtime_error("Invalid comment syntax");
    }

    string comment { };
    bool foundEnd { false };

    while (canRead()) {
        auto c = readChar();        
        if (c == '-' && canRead() && peekChar() == '-') {
            readChar(); // consume second '-'
            
            if (canRead() && peekChar() == '>') {
                readChar(); // consume '>'
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

    setNodeInfo(XmlNodeType::Comment, "", comment);
}

void XmlReader::parseCDATA() {
    // Expect "<![CDATA["
    string expected = "[CDATA[";
    for (char expectedChar : expected) {
        if (!canRead() || readChar() != expectedChar) {
            throw runtime_error("Invalid CDATA syntax");
        }
    }

    string cdata { };
    bool foundEnd { false };

    while (canRead()) {
        auto c = readChar();        
        if (c == ']' && canRead() && peekChar() == ']') {
            readChar(); // consume second ']'
            
            if (canRead() && peekChar() == '>') {
                readChar(); // consume '>'
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
        throw runtime_error("Unclosed CDATA section");
    }

    setNodeInfo(XmlNodeType::CDATA, "", cdata);
}

void XmlReader::parseProcessingInstruction() {
    string target = { readName() };
    
    if (target.empty()) {
        throw runtime_error("Invalid processing instruction target");
    }

    skipWhitespace();

    string data { };
    bool foundEnd { false };

    while (canRead()) {
        auto c = readChar();
        
        if (c == '?' && canRead() && peekChar() == '>') {
            readChar(); // consume '>'
            foundEnd = true;
            break;
        } else {
            data += c;
        }
    }

    if (!foundEnd) {
        throw runtime_error("Unclosed processing instruction");
    }

    setNodeInfo(XmlNodeType::ProcessingInstruction, target, data);
}

void XmlReader::parseXmlDeclaration() {
    // We're already past "<?xml"
    _position += 3; // skip "xml"

    // Parse attributes (version, encoding, standalone)
    parseAttributes();
    skipWhitespace();

    // Expect "?>"
    if (!canRead() || readChar() != '?' || !canRead() || readChar() != '>') {
        throw runtime_error("Invalid XML declaration syntax");
    }

    setNodeInfo(XmlNodeType::XmlDeclaration, "xml", "");
}

void XmlReader::parseDocumentType() {
    // Expect "<!DOCTYPE"
    string doctype = "DOCTYPE";
    for (char c : doctype) {
        if (!canRead() || readChar() != c) {
            throw runtime_error("Invalid DOCTYPE syntax");
        }
    }

    string dtd { };
    int bracketDepth { 0 };
    bool foundEnd = { false };

    while (canRead()) {
        auto c = readChar();        
        if (c == '[') {
            bracketDepth++;
            dtd += c;
        } else if (c == ']') {
            bracketDepth--;
            dtd += c;
        } else if (c == '>' && bracketDepth == 0) {
            foundEnd = true;
            break;
        } else {
            dtd += c;
        }
    }

    if (!foundEnd) {
        throw runtime_error("Unclosed DOCTYPE");
    }

    setNodeInfo(XmlNodeType::DocumentType, "DOCTYPE", dtd);
}

void XmlReader::parseAttributes() {
    _attributes.clear();

    while (canRead()) {
        skipWhitespace();

        auto c = peekChar();
        
        // Check for end of tag
        if (c == '>' || c == '/' || c == '?') {
            break;
        }

        // Read attribute name
        string attrName { readName() };        
        if (attrName.empty()) {
            break;
        }

        skipWhitespace();

        // Expect '='
        if (!canRead() || readChar() != '=') {
            throw runtime_error("Expected '=' after attribute name");
        }

        skipWhitespace();

        // Read attribute value
        string attrValue = readAttributeValue();

        _attributes[attrName] = attrValue;
    }
}

string XmlReader::readName() {
    string name { };
    if (!canRead()) {
        return name;
    }

    char c = peekChar();
    
    // XML name must start with letter, underscore, or colon
    if (!isalpha(static_cast<unsigned char>(c)) && c != '_' && c != ':') {
        return name;
    }

    while (canRead()) {
        c = peekChar();
        
        // Valid name characters
        if (isalnum(static_cast<unsigned char>(c)) || 
            c == '_' || c == ':' || c == '-' || c == '.') {
            name += readChar();
        } else {
            break;
        }
    }

    return name;
}

string XmlReader::readAttributeValue() {
    if (!canRead()) {
        throw runtime_error("Expected attribute value");
    }

    char quote = readChar();    
    if (quote != '"' && quote != '\'') {
        throw runtime_error("Attribute value must be quoted");
    }

    string value { };
    while (canRead()) {
        auto c = readChar();        
        if (c == quote) {
            break;
        } else if (c == '&') {
            // Handle entity references
            string entity { };
            while (canRead() && peekChar() != ';') {
                entity += readChar();
            }
            if (canRead()) {
                readChar(); // consume ';'
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

