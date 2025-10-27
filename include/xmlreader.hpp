#ifndef _XMLREADER_HPP_
#define _XMLREADER_HPP_

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <istream>
#include "xmlnodetype.hpp"
#include "xmlnode.hpp"

namespace cfdi {
    using std::string;
    using std::string_view;
    using std::map;
    using std::unique_ptr;
    using std::istream;
    using cfdi::XmlNodeType;
    using cfdi::XmlNode;

    class XmlReader {
        public:
            static unique_ptr<XmlReader> create(string_view xml);
            static unique_ptr<XmlReader> create(istream& stream);
            
            XmlReader(string_view xml);
            XmlReader(istream& stream);
            virtual ~XmlReader() = default;

            bool read();
            XmlNode current() const;

            XmlNodeType nodeType() const;
            string_view name() const;
            string_view localName() const;
            string_view prefix() const;
            string_view value() const;
            int depth() const;
            bool isEmptyElement() const;
            int getAttributeCount() const;
            bool hasAttributes() const;
            string_view getAttribute(string_view name) const;
            const map<string, string>& getAttributes() const;
            bool eof() const;

        private:
            

            bool ReadInternal();
            void SkipWhitespace();
            bool IsWhitespace(char c) const;
            char PeekChar();
            char ReadChar();
            bool CanRead() const;
            void ParseElement();
            void ParseEndElement();
            void ParseText();
            void ParseComment();
            void ParseCDATA();
            void ParseProcessingInstruction();
            void ParseXmlDeclaration();
            void ParseDocumentType();
            void ParseAttributes();
            string ReadName();
            string ReadAttributeValue();
            void SetNodeInfo(XmlNodeType type, const string& name, const string& value);
            void SplitQualifiedName(const string& qualifiedName, string& prefix, string& localName);

            string _xml;
            size_t _position;
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
