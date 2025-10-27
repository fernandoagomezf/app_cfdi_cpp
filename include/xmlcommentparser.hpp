#ifndef _XMLCOMMENT_HPP_
#define _XMLCOMMENT_HPP_

#include "xmlfragmentparser.hpp"

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    class XmlCommentParser : public XmlFragmentParser {
        public:
            virtual XmlNode parse(XmlBuffer& buffer) final;
    };
}

#endif 
