#ifndef _XMLPROCESSINGINSTRUCTIONPARSER_HPP_
#define _XMLPROCESSINGINSTRUCTIONPARSER_HPP_

#include "xmlbuffer.hpp"
#include "xmlnode.hpp"
#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlProcessingInstructionParser : public XmlFragmentParser {
        public:
            explicit XmlProcessingInstructionParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}

#endif