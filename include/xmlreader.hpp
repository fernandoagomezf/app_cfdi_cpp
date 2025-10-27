#ifndef _XMLREADER_HPP_
#define _XMLREADER_HPP_

#include <string>
#include <string_view>
#include <map>
#include <memory>
#include <istream>

namespace cfdi {
    using std::string;
    using std::string_view;
    using std::map;
    using std::unique_ptr;
    using std::istream;

    /// <summary>
    /// Specifies the type of node being read by the XmlReader
    /// </summary>
    enum class XmlNodeType {
        None,                   // No node type (initial state or end of document)
        Element,                // An element (e.g., <item>)
        EndElement,             // An end element tag (e.g., </item>)
        Text,                   // The text content of a node
        CDATA,                  // A CDATA section (e.g., <![CDATA[...]]>)
        Comment,                // A comment (e.g., <!-- ... -->)
        ProcessingInstruction,  // A processing instruction (e.g., <?pi test?>)
        XmlDeclaration,         // The XML declaration (e.g., <?xml version='1.0'?>)
        Whitespace,             // Whitespace between markup
        DocumentType            // The document type declaration (e.g., <!DOCTYPE ...>)
    };

    /// <summary>
    /// Represents a forward-only reader that provides fast, non-cached access to XML data
    /// </summary>
    class XmlReader {
    public:
        /// <summary>
        /// Creates an XmlReader instance from a string
        /// </summary>
        /// <param name="xml">The XML string to read</param>
        /// <returns>A unique pointer to an XmlReader</returns>
        static unique_ptr<XmlReader> Create(string_view xml);

        /// <summary>
        /// Creates an XmlReader instance from an input stream
        /// </summary>
        /// <param name="stream">The input stream to read from</param>
        /// <returns>A unique pointer to an XmlReader</returns>
        static unique_ptr<XmlReader> Create(istream& stream);

        /// <summary>
        /// Destructor
        /// </summary>
        virtual ~XmlReader() = default;

        /// <summary>
        /// Reads the next node from the XML stream
        /// </summary>
        /// <returns>true if the next node was read successfully; false if there are no more nodes</returns>
        bool Read();

        /// <summary>
        /// Gets the type of the current node
        /// </summary>
        XmlNodeType NodeType() const;

        /// <summary>
        /// Gets the qualified name of the current node
        /// </summary>
        string_view Name() const;

        /// <summary>
        /// Gets the local name of the current node (without namespace prefix)
        /// </summary>
        string_view LocalName() const;

        /// <summary>
        /// Gets the namespace prefix of the current node
        /// </summary>
        string_view Prefix() const;

        /// <summary>
        /// Gets the text value of the current node
        /// </summary>
        string_view Value() const;

        /// <summary>
        /// Gets the depth of the current node in the XML tree
        /// </summary>
        int Depth() const;

        /// <summary>
        /// Gets a value indicating whether the current node is an empty element (e.g., <item />)
        /// </summary>
        bool IsEmptyElement() const;

        /// <summary>
        /// Gets the number of attributes on the current node
        /// </summary>
        int AttributeCount() const;

        /// <summary>
        /// Gets a value indicating whether the current node has any attributes
        /// </summary>
        bool HasAttributes() const;

        /// <summary>
        /// Gets the value of the attribute with the specified name
        /// </summary>
        /// <param name="name">The qualified name of the attribute</param>
        /// <returns>The value of the specified attribute, or empty string if not found</returns>
        string_view GetAttribute(string_view name) const;

        /// <summary>
        /// Gets all attributes of the current element
        /// </summary>
        const map<string, string>& GetAttributes() const;

        /// <summary>
        /// Gets a value indicating whether the reader has reached the end of the input
        /// </summary>
        bool IsEOF() const;

    private:
        XmlReader(string_view xml);
        XmlReader(istream& stream);

        // Internal implementation methods
        void Initialize();
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

        // State
        string _xml;
        size_t _position;
        bool _eof;
        
        // Current node information
        XmlNodeType _nodeType;
        string _name;
        string _localName;
        string _prefix;
        string _value;
        int _depth;
        bool _isEmptyElement;
        map<string, string> _attributes;
        
        // Helper state
        bool _readContent;
    };

} // namespace cfdi

#endif // _XMLREADER_HPP_
