export module cfdi.xml:xmlcdataparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    export class XmlCDataParser : public XmlFragmentParser {
        public:
            explicit XmlCDataParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}