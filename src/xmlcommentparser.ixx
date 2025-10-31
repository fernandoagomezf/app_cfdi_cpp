export module cfdi.xml:xmlcommentparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    export class XmlCommentParser : public XmlFragmentParser {
        public:
            explicit XmlCommentParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}