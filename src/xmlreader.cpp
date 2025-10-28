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
#include <memory>

using std::unique_ptr;
using std::make_unique;
using std::ostringstream;
using std::runtime_error;
using std::string_view;
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

void XmlReader::resetNode() {
        
}

bool XmlReader::read() {
    unique_ptr<XmlFragmentParser> parser;

    _currentNode =  { 
        .nodeType = XmlNodeType::None
    };
    if (_eof) {
        return false;
    }

    _buffer.skipWhiteSpace();
    if (!_buffer.canRead()) {
        _eof = true;
        _currentNode = { 
            .nodeType = XmlNodeType::None
        };
        return false;
    }

    auto c = _buffer.peek();
    if (c == '<') {
        _buffer.consume(); // consume '<'        
        if (!_buffer.canRead()) {
            throw runtime_error("Unexpected end of XML");
        }

        auto next = _buffer.peek();
        if (next == '!') {
            _buffer.consume(); // consume '!'            
            if (!_buffer.canRead()) {
                throw runtime_error("Unexpected end of XML");
            }

            auto third = _buffer.peek();            
            if (third == '-') {
                parser = make_unique<XmlCommentParser>(_buffer);
            } else if (third == '[') {
                parser = make_unique<XmlCDataParser>(_buffer);
            } else if (third == 'D') {
                parser = make_unique<XmlDocTypeParser>(_buffer);
            } else {
                throw runtime_error("Invalid XML syntax");
            }
        } else if (next == '?') {
            _buffer.consume(); // consume '?'
            
            if (_buffer.position() + 3 <= _buffer.length() && _buffer.substr(3) == "xml") {                
                parser = make_unique<XmlDeclarationParser>(_buffer);                
            } else {
                parser = make_unique<XmlProcessingInstructionParser>(_buffer);
            }
        } else {
            parser = make_unique<XmlElementParser>(_buffer);
        }
    } else {
        parser = make_unique<XmlTextParser>(_buffer);
    }

    if (!parser) {
        throw runtime_error("No parser available.");
    }
    _currentNode = { 
        parser->parse()
    };
    if (_currentNode.nodeType == XmlNodeType::Element && !_currentNode.isEmpty) {
        _currentNode.depth = ++_depth;
    } else if (_currentNode.nodeType == XmlNodeType::EndElement) {
        _currentNode.depth = --_depth;
    }

    return true;
}

XmlNode XmlReader::current() const {
    return _currentNode;
}



