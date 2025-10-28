#include "xmlreader.hpp"
#include "xmltextparser.hpp"
#include "xmlcommentparser.hpp"
#include "xmlcdataparser.hpp"
#include "xmlprocessinginstructionparser.hpp"
#include "xmldeclarationparser.hpp"
#include "xmldoctypeparser.hpp"
#include "xmlelementparser.hpp"
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
using cfdi::XmlElementParser;
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
    XmlElementParser parser { };
    XmlNode node { parser.parse(_buffer) };
    
    _nodeType = node.nodeType;
    _name = node.name;
    _prefix = node.prefix;
    _localName = node.localName;
    _attributes = node.attributes;
    if (node.nodeType == XmlNodeType::Element && !node.isEmpty)
        ++_depth;
    else 
        --_depth;
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


