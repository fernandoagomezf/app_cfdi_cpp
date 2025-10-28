#ifndef _XMLDOCTYPEPARSER_HPP_
#define _XMLDOCTYPEPARSER_HPP_

#include "xmlbuffer.hpp"
#include "xmlnode.hpp"
#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlDocTypeParser : public XmlFragmentParser {
        public:
            explicit XmlDocTypeParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}

#endif 
