#ifndef _XMLPARSER_HPP_
#define _XMLPARSER_HPP_

#include <string>
#include "xmlnode.hpp"
#include "xmlbuffer.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlNode;

    class XmlFragmentParser {
        public:
            virtual ~XmlFragmentParser() = default;
            virtual XmlNode parse(XmlBuffer& buffer) = 0;

        protected:
            string parseName(XmlBuffer& buffer);
    };
}

#endif
