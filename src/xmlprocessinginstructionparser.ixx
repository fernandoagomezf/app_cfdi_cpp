export module cfdi.xml:xmlprocessinginstructionparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    export class XmlProcessingInstructionParser : public XmlFragmentParser {
        public:
            explicit XmlProcessingInstructionParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;
    };
}