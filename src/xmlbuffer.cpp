#include "xmlbuffer.hpp"
#include <stdexcept>

using std::invalid_argument;
using std::size_t;
using std::string;
using std::string_view;
using cfdi::XmlBuffer;

XmlBuffer::XmlBuffer(string_view content)
    : XmlBuffer(content, 0) 
{
}

XmlBuffer::XmlBuffer(string_view content, size_t position) 
    : _buffer { content }, 
      _position { position }
{
    if (position < 0) {
        throw invalid_argument("Position cannot be negative.");
    }
}

bool XmlBuffer::isWhiteSpace(char c) {
    return c == ' ' || c == '\t' || c == '\n' || c == '\r';
}

size_t XmlBuffer::position() const {
    return _position;
}

size_t XmlBuffer::length() const {
    return _buffer.length();
}

string_view XmlBuffer::substr(size_t pos, size_t len) const {
    if (pos < 0) {
        throw invalid_argument("The length cannot be negative.");
    }
    if (len <= 0) {
        throw invalid_argument("The length cannot be negative or zero.");
    }
    return substr(pos, len);
}

string_view XmlBuffer::substr(size_t len) const {
    return substr(_position, len);
}

void XmlBuffer::skipWhiteSpace() {
    while (canRead() && isWhiteSpace(peek())) {
        read();
    }
}

bool XmlBuffer::canRead() const {
    return _position < _buffer.length();
}

char XmlBuffer::peek() {
    auto ch = '\0';
    if (_position < _buffer.length()) {
        ch = _buffer[_position];
    }
    return ch;
}

char XmlBuffer::read() {
    auto ch = '\0';
    if (_position < _buffer.length()) {
        ch = _buffer[_position];
        ++_position;
    }
    return ch;
}

void XmlBuffer::consume(size_t count) {
    for (auto i = 0; i < count; ++i) {
        consume();
    }
}

void XmlBuffer::consume() {
    if (_position < _buffer.length()) {
        ++_position;
    }
}
