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
            

            bool readInternal();            
            void parseElement();
            void parseText();
            void parseComment();
            void parseCDATA();
            void parseProcessingInstruction();
            void parseXmlDeclaration();
            void parseDocumentType();
            void parseAttributes();
            string readName();
            string readAttributeValue();
            void setNodeInfo(XmlNodeType type, const string& name, const string& value);
            void splitQualifiedName(const string& qualifiedName, string& prefix, string& localName);

            XmlBuffer _buffer;
            bool _eof;
            
            XmlNodeType _nodeType;
            string _name;
            string _localName;
            string _prefix;
            string _value;
            int _depth;
            bool _isEmptyElement;
            map<string, string> _attributes;
            
            bool _readContent;
    };

} 

#endif 
