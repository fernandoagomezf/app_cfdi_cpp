export module cfdi.xml:xmltextparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    export class XmlTextParser : public XmlFragmentParser {
        public:
            explicit XmlTextParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}

