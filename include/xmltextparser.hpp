#ifndef _XMLTEXTPARSER_HPP_
#define _XMLTEXTPARSER_HPP_

#include "xmlbuffer.hpp"
#include "xmlnode.hpp"
#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlTextParser : public XmlFragmentParser {
        public:
            explicit XmlTextParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}

#endif
