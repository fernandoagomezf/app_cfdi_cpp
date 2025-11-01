export module cfdi.xml:xmldoctypeparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    export class XmlDocTypeParser : public XmlFragmentParser {
        public:
            explicit XmlDocTypeParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}