module cfdi.xml:xmlnode;

import std;
import :xmlnodetype;
import :xmlnode;

using std::string;
using std::string_view;
using cfdi::XmlNode;

int XmlNode::attributeCount() const {
    auto size  { attributes.size() };
    return static_cast<int>(size);
}

bool XmlNode::hasAttributes() const {
    return !attributes.empty();
}

string_view XmlNode::getAttribute(string_view name) const{
    string result { };
    
    auto it = attributes.find(string { name });
    if (it != attributes.end()) {
        result = it->second;
    }

    return result;
}