#ifndef _XMLELEMENTPARSER_HPP_
#define _XMLELEMENTPARSER_HPP_

#include "xmlbuffer.hpp"
#include "xmlnode.hpp"
#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlElementParser : public XmlFragmentParser {
        public:
            explicit XmlElementParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;

        private:
            XmlNode parseElementStart();
            XmlNode parseElementEnd();
    };
}

#endif 
