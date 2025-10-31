export module cfdi.xml:xmlelementparser;

import std;
import :xmlbuffer;
import :xmlfragmentparser;
import :xmlnode;

namespace cfdi {
    using cfdi::XmlBuffer;
    using cfdi::XmlFragmentParser;
    using cfdi::XmlNode;

    export class XmlElementParser : public XmlFragmentParser {
        public:
            explicit XmlElementParser(XmlBuffer& buffer);
            virtual XmlNode parse() final;

        private:
            XmlNode parseElementStart();
            XmlNode parseElementEnd();
    };
}