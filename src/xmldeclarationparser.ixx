export module cfdi.xml:xmldeclarationparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    export class XmlDeclarationParser : public XmlFragmentParser {
        public:
            explicit XmlDeclarationParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}
