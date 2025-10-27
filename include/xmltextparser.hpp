#ifndef _XMLTEXTPARSER_HPP_
#define _XMLTEXTPARSER_HPP_

#include "xmlbuffer.hpp"
#include "xmlnode.hpp"
#include "xmlnodetype.hpp"
#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;
    using cfdi::XmlNodeType;

    class XmlTextParser : public XmlFragmentParser {
        public:
            virtual XmlNode parse(XmlBuffer& buffer) final;
    };
}

#endif
