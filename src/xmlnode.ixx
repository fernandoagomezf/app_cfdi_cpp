export module cfdi.xml:xmlnode;

import std;
import :xmlnodetype;

namespace cfdi {
    using std::map;
    using std::string;
    using std::string_view;
    using cfdi::XmlNodeType;
    
    export struct XmlNode {
        XmlNodeType nodeType;
        string name;
        string localName;
        string prefix;
        string value;
        int depth;
        bool isEmpty;
        bool eof;
        map<string, string> attributes;

        int attributeCount() const;
        bool hasAttributes() const; 
        string_view getAttribute(string_view name) const;
    };
}