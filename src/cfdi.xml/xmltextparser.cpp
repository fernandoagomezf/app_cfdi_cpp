module cfdi.xml:xmltextparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;
import :xmltextparser;

using std::string;
using cfdi::XmlBuffer;
using cfdi::XmlFragmentParser;
using cfdi::XmlNode;
using cfdi::XmlNodeType;
using cfdi::XmlTextParser;

XmlTextParser::XmlTextParser(XmlBuffer& buffer)
    : XmlFragmentParser(buffer)
{

}

XmlNode XmlTextParser::parse() {    // check for normal text inside element tags, e.g. <Element> all these text characers </Element>
    auto& buffer { getBuffer() };
    string text { };    
    while (buffer.canRead() && buffer.peek() != '<') {
        auto c = buffer.read();
        
        if (c == '&') { // text can have entities, e.g. &amp; &lt; &quot; etc. so decode them
            string entity { };
            while (buffer.canRead() && buffer.peek() != ';') {
                entity += buffer.read();
            }
            if (buffer.canRead()) {
                buffer.consume(); // consume ';'
            }
            
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
                // if it's not named, we have a numeric (e.g. unicode) entity
                int code { 0 };
                if (entity.length() > 1 && entity[1] == 'x') {  // base-16 entity, deal with hex format
                    code = stoi(entity.substr(2), nullptr, 16);
                } else { 
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

    // check whether we found actual text or only whitespace, if so return Whitespace node type instead of text
    bool allWhitespace = true;
    for (char c : text) {
        if (!XmlBuffer::isWhiteSpace(c)) {
            allWhitespace = false;
            break;
        }
    }

    XmlNode node { 
        .nodeType = allWhitespace && !text.empty() ? XmlNodeType::Whitespace : XmlNodeType::Text,
        .value = text,
    };
    
    return node;
}