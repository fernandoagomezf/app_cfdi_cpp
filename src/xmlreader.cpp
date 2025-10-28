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
      _currentNode { .nodeType = XmlNodeType::None },
      _depth { 0 },
      _eof { false }
{
    _eof = xml.empty();
}

XmlReader::XmlReader(istream& stream)
    : _buffer { { }, 0  },
      _currentNode { },
      _depth { 0 },
      _eof { false }
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

    _currentNode =  { 
        .nodeType = XmlNodeType::None
    };
    auto result { readInternal() };

    return result;
}

XmlNode XmlReader::current() const {
    return _currentNode;
}

bool XmlReader::readInternal() {
    _buffer.skipWhiteSpace();

    if (!_buffer.canRead()) {
        _eof = true;
        _currentNode = { 
            .nodeType = XmlNodeType::None
        };
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
    XmlElementParser parser { _buffer };
    _currentNode = { parser.parse() };    
    if (_currentNode.nodeType == XmlNodeType::Element && !_currentNode.isEmpty) {
        _currentNode.depth = ++_depth;
    } else if (_currentNode.nodeType == XmlNodeType::EndElement) {
        _currentNode.depth = --_depth;
    }
}

void XmlReader::parseText() {
    XmlTextParser parser { _buffer };
    _currentNode = { parser.parse() };    
}

void XmlReader::parseComment() {
    XmlCommentParser parser { _buffer };
    _currentNode = { parser.parse() };
}

void XmlReader::parseCDATA() {
    XmlCDataParser parser { _buffer };
    _currentNode = { parser.parse() };
}

void XmlReader::parseProcessingInstruction() {
    XmlProcessingInstructionParser parser { _buffer };
    _currentNode = { parser.parse() };
}

void XmlReader::parseXmlDeclaration() {
    XmlDeclarationParser parser { _buffer };
    _currentNode = { parser.parse() };
}

void XmlReader::parseDocumentType() {
    XmlDeclarationParser parser { _buffer };
    _currentNode = { parser.parse() };
}


