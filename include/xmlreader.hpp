#ifndef _XMLREADER_HPP_
#define _XMLREADER_HPP_

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <istream>
#include "xmlnodetype.hpp"
#include "xmlnode.hpp"
#include "xmlbuffer.hpp"

namespace cfdi {
    using std::string;
    using std::string_view;
    using std::map;
    using std::unique_ptr;
    using std::istream;
    using cfdi::XmlNodeType;
    using cfdi::XmlNode;
    using cfdi::XmlBuffer;

    class XmlReader {
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

#endif 
