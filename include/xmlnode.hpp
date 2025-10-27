#ifndef _XMLNODE_HPP_
#define _XMLNODE_HPP_

#include <map>
#include <string>
#include <string_view>
#include "xmlnodetype.hpp"

namespace cfdi {
    using std::map;
    using std::string;
    using std::string_view;
    using cfdi::XmlNodeType;
    
    struct XmlNode {
        XmlNodeType nodeType;
        string_view name;
        string_view localName;
        string_view prefix;
        string_view value;
        int depth;
        bool isEmpty;
        bool eof;
        map<string, string> attributes;

        int attributeCount() const;
        bool hasAttributes() const; 
        string_view getAttribute(string_view name) const;
    };
}

#endif 
