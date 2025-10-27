#include "xmlreader.hpp"
#include <sstream>
#include <stdexcept>
#include <cctype>

using std::map;
using std::string;
using std::string_view;
using std::unique_ptr;
using cfdi::XmlNodeType;
using cfdi::XmlNode;
using cfdi::XmlReader;

// Static factory methods
unique_ptr<XmlReader> XmlReader::create(string_view xml) {
    return unique_ptr<XmlReader>(new XmlReader(xml));
}

unique_ptr<XmlReader> XmlReader::create(istream& stream) {
    return unique_ptr<XmlReader>(new XmlReader(stream));
}

// Constructors
XmlReader::XmlReader(string_view xml)
    : _xml(xml)
    , _position(0)
    , _eof(false)
    , _nodeType(XmlNodeType::None)
    , _depth(0)
    , _isEmptyElement(false)
    , _readContent(false)
{
    Initialize();
}

XmlReader::XmlReader(istream& stream)
    : _position(0)
    , _eof(false)
    , _nodeType(XmlNodeType::None)
    , _depth(0)
    , _isEmptyElement(false)
    , _readContent(false)
{
    std::ostringstream ss;
    ss << stream.rdbuf();
    _xml = ss.str();
    Initialize();
}

void XmlReader::Initialize() {
    _position = 0;
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

    return ReadInternal();
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

XmlNodeType XmlReader::nodeType() const {
    return _nodeType;
}

string_view XmlReader::name() const {
    return _name;
}

string_view XmlReader::localName() const {
    return _localName;
}

string_view XmlReader::prefix() const {
    return _prefix;
}

string_view XmlReader::value() const {
    return _value;
}

int XmlReader::depth() const {
    return _depth;
}

bool XmlReader::isEmptyElement() const {
    return _isEmptyElement;
}

int XmlReader::getAttributeCount() const {
    return static_cast<int>(_attributes.size());
}

bool XmlReader::hasAttributes() const {
    return !_attributes.empty();
}

string_view XmlReader::getAttribute(string_view name) const {
    auto it = _attributes.find(string(name));
    if (it != _attributes.end()) {
        return it->second;
    }
    return "";
}

const map<string, string>& XmlReader::getAttributes() const {
    return _attributes;
}

bool XmlReader::eof() const {
    return _eof;
}

bool XmlReader::ReadInternal() {
    SkipWhitespace();

    if (!CanRead()) {
        _eof = true;
        _nodeType = XmlNodeType::None;
        return false;
    }

    char c = PeekChar();

    // Check for markup
    if (c == '<') {
        ReadChar(); // consume '<'
        
        if (!CanRead()) {
            throw std::runtime_error("Unexpected end of XML");
        }

        char next = PeekChar();

        if (next == '!') {
            ReadChar(); // consume '!'
            
            if (!CanRead()) {
                throw std::runtime_error("Unexpected end of XML");
            }

            char third = PeekChar();
            
            if (third == '-') {
                ParseComment();
            } else if (third == '[') {
                ParseCDATA();
            } else if (third == 'D') {
                ParseDocumentType();
            } else {
                throw std::runtime_error("Invalid XML syntax");
            }
        } else if (next == '?') {
            ReadChar(); // consume '?'
            
            // Check if it's XML declaration
            if (_position + 3 <= _xml.length() && 
                _xml.substr(_position, 3) == "xml") {
                ParseXmlDeclaration();
            } else {
                ParseProcessingInstruction();
            }
        } else if (next == '/') {
            ReadChar(); // consume '/'
            ParseEndElement();
        } else {
            ParseElement();
        }

        return true;
    } else {
        // Text content
        ParseText();
        return true;
    }
}

void XmlReader::SkipWhitespace() {
    while (CanRead() && IsWhitespace(PeekChar())) {
        ReadChar();
    }
}

bool XmlReader::IsWhitespace(char c) const {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

char XmlReader::PeekChar() {
    if (_position < _xml.length()) {
        return _xml[_position];
    }
    return '\0';
}

char XmlReader::ReadChar() {
    if (_position < _xml.length()) {
        return _xml[_position++];
    }
    return '\0';
}

bool XmlReader::CanRead() const {
    return _position < _xml.length();
}

void XmlReader::ParseElement() {
    string name = ReadName();
    
    if (name.empty()) {
        throw std::runtime_error("Invalid element name");
    }

    // Parse attributes
    ParseAttributes();

    SkipWhitespace();

    // Check for empty element
    if (CanRead() && PeekChar() == '/') {
        ReadChar(); // consume '/'
        _isEmptyElement = true;
    }

    // Expect closing '>'
    if (!CanRead() || ReadChar() != '>') {
        throw std::runtime_error("Expected '>' to close element tag");
    }

    if (!_isEmptyElement) {
        _depth++;
    }

    string prefix, localName;
    SplitQualifiedName(name, prefix, localName);
    
    SetNodeInfo(XmlNodeType::Element, name, "");
    _prefix = prefix;
    _localName = localName;
}

void XmlReader::ParseEndElement() {
    string name = ReadName();
    
    if (name.empty()) {
        throw std::runtime_error("Invalid end element name");
    }

    SkipWhitespace();

    // Expect closing '>'
    if (!CanRead() || ReadChar() != '>') {
        throw std::runtime_error("Expected '>' to close end element tag");
    }

    _depth--;
    if (_depth < 0) {
        _depth = 0;
    }

    string prefix, localName;
    SplitQualifiedName(name, prefix, localName);
    
    SetNodeInfo(XmlNodeType::EndElement, name, "");
    _prefix = prefix;
    _localName = localName;
}

void XmlReader::ParseText() {
    string text;
    
    while (CanRead() && PeekChar() != '<') {
        char c = ReadChar();
        
        // Handle entity references
        if (c == '&') {
            string entity;
            while (CanRead() && PeekChar() != ';') {
                entity += ReadChar();
            }
            if (CanRead()) {
                ReadChar(); // consume ';'
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
                    code = std::stoi(entity.substr(2), nullptr, 16);
                } else {
                    // Decimal
                    code = std::stoi(entity.substr(1));
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
        if (!IsWhitespace(c)) {
            allWhitespace = false;
            break;
        }
    }

    if (allWhitespace && !text.empty()) {
        SetNodeInfo(XmlNodeType::Whitespace, "", text);
    } else {
        SetNodeInfo(XmlNodeType::Text, "", text);
    }
}

void XmlReader::ParseComment() {
    // Expect "<!--"
    if (ReadChar() != '-' || !CanRead() || ReadChar() != '-') {
        throw std::runtime_error("Invalid comment syntax");
    }

    string comment;
    bool foundEnd = false;

    while (CanRead()) {
        char c = ReadChar();
        
        if (c == '-' && CanRead() && PeekChar() == '-') {
            ReadChar(); // consume second '-'
            
            if (CanRead() && PeekChar() == '>') {
                ReadChar(); // consume '>'
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
        throw std::runtime_error("Unclosed comment");
    }

    SetNodeInfo(XmlNodeType::Comment, "", comment);
}

void XmlReader::ParseCDATA() {
    // Expect "<![CDATA["
    string expected = "[CDATA[";
    for (char expectedChar : expected) {
        if (!CanRead() || ReadChar() != expectedChar) {
            throw std::runtime_error("Invalid CDATA syntax");
        }
    }

    string cdata;
    bool foundEnd = false;

    while (CanRead()) {
        char c = ReadChar();
        
        if (c == ']' && CanRead() && PeekChar() == ']') {
            ReadChar(); // consume second ']'
            
            if (CanRead() && PeekChar() == '>') {
                ReadChar(); // consume '>'
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
        throw std::runtime_error("Unclosed CDATA section");
    }

    SetNodeInfo(XmlNodeType::CDATA, "", cdata);
}

void XmlReader::ParseProcessingInstruction() {
    string target = ReadName();
    
    if (target.empty()) {
        throw std::runtime_error("Invalid processing instruction target");
    }

    SkipWhitespace();

    string data;
    bool foundEnd = false;

    while (CanRead()) {
        char c = ReadChar();
        
        if (c == '?' && CanRead() && PeekChar() == '>') {
            ReadChar(); // consume '>'
            foundEnd = true;
            break;
        } else {
            data += c;
        }
    }

    if (!foundEnd) {
        throw std::runtime_error("Unclosed processing instruction");
    }

    SetNodeInfo(XmlNodeType::ProcessingInstruction, target, data);
}

void XmlReader::ParseXmlDeclaration() {
    // We're already past "<?xml"
    _position += 3; // skip "xml"

    // Parse attributes (version, encoding, standalone)
    ParseAttributes();

    SkipWhitespace();

    // Expect "?>"
    if (!CanRead() || ReadChar() != '?' || !CanRead() || ReadChar() != '>') {
        throw std::runtime_error("Invalid XML declaration syntax");
    }

    SetNodeInfo(XmlNodeType::XmlDeclaration, "xml", "");
}

void XmlReader::ParseDocumentType() {
    // Expect "<!DOCTYPE"
    string doctype = "DOCTYPE";
    for (char c : doctype) {
        if (!CanRead() || ReadChar() != c) {
            throw std::runtime_error("Invalid DOCTYPE syntax");
        }
    }

    string dtd;
    int bracketDepth = 0;
    bool foundEnd = false;

    while (CanRead()) {
        char c = ReadChar();
        
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
        throw std::runtime_error("Unclosed DOCTYPE");
    }

    SetNodeInfo(XmlNodeType::DocumentType, "DOCTYPE", dtd);
}

void XmlReader::ParseAttributes() {
    _attributes.clear();

    while (CanRead()) {
        SkipWhitespace();

        char c = PeekChar();
        
        // Check for end of tag
        if (c == '>' || c == '/' || c == '?') {
            break;
        }

        // Read attribute name
        string attrName = ReadName();
        
        if (attrName.empty()) {
            break;
        }

        SkipWhitespace();

        // Expect '='
        if (!CanRead() || ReadChar() != '=') {
            throw std::runtime_error("Expected '=' after attribute name");
        }

        SkipWhitespace();

        // Read attribute value
        string attrValue = ReadAttributeValue();

        _attributes[attrName] = attrValue;
    }
}

string XmlReader::ReadName() {
    string name;

    if (!CanRead()) {
        return name;
    }

    char c = PeekChar();
    
    // XML name must start with letter, underscore, or colon
    if (!std::isalpha(static_cast<unsigned char>(c)) && c != '_' && c != ':') {
        return name;
    }

    while (CanRead()) {
        c = PeekChar();
        
        // Valid name characters
        if (std::isalnum(static_cast<unsigned char>(c)) || 
            c == '_' || c == ':' || c == '-' || c == '.') {
            name += ReadChar();
        } else {
            break;
        }
    }

    return name;
}

string XmlReader::ReadAttributeValue() {
    if (!CanRead()) {
        throw std::runtime_error("Expected attribute value");
    }

    char quote = ReadChar();
    
    if (quote != '"' && quote != '\'') {
        throw std::runtime_error("Attribute value must be quoted");
    }

    string value;

    while (CanRead()) {
        char c = ReadChar();
        
        if (c == quote) {
            break;
        } else if (c == '&') {
            // Handle entity references
            string entity;
            while (CanRead() && PeekChar() != ';') {
                entity += ReadChar();
            }
            if (CanRead()) {
                ReadChar(); // consume ';'
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
                int code = 0;
                if (entity.length() > 1 && entity[1] == 'x') {
                    // Hexadecimal
                    code = std::stoi(entity.substr(2), nullptr, 16);
                } else {
                    // Decimal
                    code = std::stoi(entity.substr(1));
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

void XmlReader::SetNodeInfo(XmlNodeType type, const string& name, const string& value) {
    _nodeType = type;
    _name = name;
    _value = value;
}

void XmlReader::SplitQualifiedName(const string& qualifiedName, string& prefix, string& localName) {
    size_t colonPos = qualifiedName.find(':');
    
    if (colonPos != string::npos) {
        prefix = qualifiedName.substr(0, colonPos);
        localName = qualifiedName.substr(colonPos + 1);
    } else {
        prefix.clear();
        localName = qualifiedName;
    }
}

