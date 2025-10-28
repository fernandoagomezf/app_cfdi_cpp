#ifndef _XMLDECLARATIONPARSER_HPP_
#define _XMLDECLARATIONPARSER_HPP_

#include "xmlbuffer.hpp"
#include "xmlnode.hpp"
#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlDeclarationParser : public XmlFragmentParser {
        public:
            virtual XmlNode parse(XmlBuffer& buffer) final;
    };
}

#endif 
