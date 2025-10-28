#ifndef _XMLCDATAPARSER_HPP_
#define _XMLCDATAPARSER_HPP_

#include "xmlbuffer.hpp"
#include "xmlnode.hpp"
#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlCDataParser : public XmlFragmentParser {
        public:
            explicit XmlCDataParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}

#endif 
