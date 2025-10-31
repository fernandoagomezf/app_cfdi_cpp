export module cfdi.xml:xmlreader;

import std;
import :xmlnode;
import :xmlbuffer;

namespace cfdi {
    using std::string;
    using std::string_view;
    using std::map;
    using std::unique_ptr;
    using std::istream;
    using cfdi::XmlNode;
    using cfdi::XmlBuffer;

    export class XmlReader {
        public:            
            XmlReader(string_view xml);
            XmlReader(istream& stream);
            virtual ~XmlReader() = default;

            bool read();
            XmlNode current() const;

        private:
            void resetNode();            
            
            bool _eof;
            int _depth;
            XmlBuffer _buffer;            
            XmlNode _currentNode;
    };

} 