#ifndef _XMLPARSER_HPP_
#define _XMLPARSER_HPP_

#include <map>
#include <string>
#include <utility>
#include "xmlnode.hpp"
#include "xmlbuffer.hpp"

namespace cfdi {
    using std::map;
    using std::pair;
    using std::string;
    using cfdi::XmlBuffer;
    using cfdi::XmlNode;

    class XmlFragmentParser {
        public:
            virtual ~XmlFragmentParser() = default;
            virtual XmlNode parse() = 0;

        protected:
            explicit XmlFragmentParser(XmlBuffer& buffer);

            XmlBuffer& getBuffer();
        
            string parseName();
            map<string, string> parseAttributes();
            string parseAttributeValue();
            pair<string, string> splitQualifiedName(string_view name) const;
        
        private:
            XmlBuffer& _buffer;
    };
}

#endif
