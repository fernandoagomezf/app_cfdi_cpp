#ifndef _XMLCOMMENT_HPP_
#define _XMLCOMMENT_HPP_

#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlCommentParser : public XmlFragmentParser {
        public:
            explicit XmlCommentParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}

#endif 
