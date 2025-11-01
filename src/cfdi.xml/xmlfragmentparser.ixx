export module cfdi.xml:xmlfragmentparser;

import std;
import :xmlnode;
import :xmlbuffer;

namespace cfdi {
    using std::map;
    using std::pair;
    using std::string;
    using std::string_view;
    using cfdi::XmlBuffer;
    using cfdi::XmlNode;

    export class XmlFragmentParser {
        public:
            virtual ~XmlFragmentParser() = default;
            virtual XmlNode parse() = 0;

        protected:
            explicit XmlFragmentParser(XmlBuffer& buffer);

            XmlBuffer& getBuffer();
        
            string parseName();
            map<string, string> parseAttributes();
            string parseAttributeValue();
            pair<string, string> splitQualifiedName(string_view name) const;
        
        private:
            XmlBuffer& _buffer;
    };
}