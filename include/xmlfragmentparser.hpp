#ifndef _XMLPARSER_HPP_
#define _XMLPARSER_HPP_

#include <map>
#include <string>
#include "xmlnode.hpp"
#include "xmlbuffer.hpp"

namespace cfdi {
    using std::map;
    using std::string;
    using cfdi::XmlBuffer;
    using cfdi::XmlNode;

    class XmlFragmentParser {
        public:
            virtual ~XmlFragmentParser() = default;
            virtual XmlNode parse(XmlBuffer& buffer) = 0;

        protected:
            string parseName(XmlBuffer& buffer);
            map<string, string> parseAttributes(XmlBuffer& buffer);
            string readAttributeValue(XmlBuffer& buffer);
    };
}

#endif
