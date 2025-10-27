# XmlReader Class Documentation

## Overview

The `XmlReader` class provides a forward-only, fast, and non-cached XML parser similar to .NET's `XmlReader` implementation. It's designed to efficiently read XML documents, including CFDI v4 documents, without the overhead of building a complete DOM tree.

## Features

- **Forward-only reading**: Efficient memory usage by reading sequentially
- **Node type detection**: Identifies elements, text, comments, CDATA, processing instructions, etc.
- **Attribute parsing**: Full support for element attributes
- **Namespace support**: Handles namespace prefixes and local names
- **Entity decoding**: Automatically decodes common XML entities (`&lt;`, `&gt;`, `&amp;`, `&quot;`, `&apos;`)
- **Numeric character references**: Supports both decimal and hexadecimal character codes
- **Empty element detection**: Identifies self-closing tags
- **Depth tracking**: Maintains current nesting level in the XML tree

## Node Types

The `XmlNodeType` enumeration defines the following node types:

- `None` - No node (initial state or end of document)
- `Element` - Opening tag (e.g., `<item>`)
- `EndElement` - Closing tag (e.g., `</item>`)
- `Text` - Text content
- `CDATA` - CDATA section
- `Comment` - XML comment
- `ProcessingInstruction` - Processing instruction
- `XmlDeclaration` - XML declaration (`<?xml version="1.0"?>`)
- `Whitespace` - Whitespace-only text
- `DocumentType` - DOCTYPE declaration

## Usage

### Creating an XmlReader

```cpp
#include "xmlreader.hpp"

using namespace cfdi;

// From a string
std::string xml = "<?xml version=\"1.0\"?><root>...</root>";
auto reader = XmlReader::Create(xml);

// From an input stream
std::ifstream file("document.xml");
auto reader = XmlReader::Create(file);
```

### Reading XML Content

```cpp
auto reader = XmlReader::Create(xmlContent);

while (reader->Read()) {
    switch (reader->NodeType()) {
        case XmlNodeType::Element:
            // Process element
            std::cout << "Element: " << reader->Name() << std::endl;
            
            // Access attributes
            if (reader->HasAttributes()) {
                for (const auto& [name, value] : reader->GetAttributes()) {
                    std::cout << "  @" << name << " = \"" << value << "\"" << std::endl;
                }
            }
            
            // Check if self-closing
            if (reader->IsEmptyElement()) {
                std::cout << "  (empty element)" << std::endl;
            }
            break;
            
        case XmlNodeType::Text:
            std::cout << "Text: " << reader->Value() << std::endl;
            break;
            
        case XmlNodeType::EndElement:
            std::cout << "End: " << reader->Name() << std::endl;
            break;
    }
}
```

### Working with Namespaces

```cpp
if (reader->NodeType() == XmlNodeType::Element) {
    // Full qualified name (e.g., "cfdi:Comprobante")
    std::string_view fullName = reader->Name();
    
    // Namespace prefix (e.g., "cfdi")
    std::string_view prefix = reader->Prefix();
    
    // Local name without prefix (e.g., "Comprobante")
    std::string_view localName = reader->LocalName();
}
```

### Accessing Attributes

```cpp
if (reader->NodeType() == XmlNodeType::Element) {
    // Get specific attribute
    std::string_view version = reader->GetAttribute("Version");
    
    // Check attribute count
    int count = reader->AttributeCount();
    
    // Iterate all attributes
    const auto& attributes = reader->GetAttributes();
    for (const auto& [name, value] : attributes) {
        // Process attribute
    }
}
```

### Reading CFDI v4 Documents

```cpp
#include "xmlreader.hpp"
#include <fstream>
#include <iostream>

void parseCFDI(const std::string& filePath) {
    std::ifstream file(filePath);
    if (!file.is_open()) {
        std::cerr << "Cannot open file: " << filePath << std::endl;
        return;
    }
    
    auto reader = cfdi::XmlReader::Create(file);
    
    while (reader->Read()) {
        if (reader->NodeType() == cfdi::XmlNodeType::Element) {
            std::string_view localName = reader->LocalName();
            
            if (localName == "Comprobante") {
                // Read root element attributes
                std::string_view version = reader->GetAttribute("Version");
                std::string_view serie = reader->GetAttribute("Serie");
                std::string_view folio = reader->GetAttribute("Folio");
                
                std::cout << "CFDI Version: " << version << std::endl;
                std::cout << "Serie: " << serie << ", Folio: " << folio << std::endl;
            }
            else if (localName == "Emisor") {
                std::string_view rfc = reader->GetAttribute("Rfc");
                std::string_view nombre = reader->GetAttribute("Nombre");
                
                std::cout << "Emisor: " << nombre << " (RFC: " << rfc << ")" << std::endl;
            }
            else if (localName == "Receptor") {
                std::string_view rfc = reader->GetAttribute("Rfc");
                std::string_view nombre = reader->GetAttribute("Nombre");
                
                std::cout << "Receptor: " << nombre << " (RFC: " << rfc << ")" << std::endl;
            }
            else if (localName == "Concepto") {
                std::string_view desc = reader->GetAttribute("Descripcion");
                std::string_view cantidad = reader->GetAttribute("Cantidad");
                std::string_view importe = reader->GetAttribute("Importe");
                
                std::cout << "Concepto: " << desc << std::endl;
                std::cout << "  Cantidad: " << cantidad << ", Importe: " << importe << std::endl;
            }
        }
    }
}
```

## Public API

### Factory Methods

- `static unique_ptr<XmlReader> Create(string_view xml)` - Create reader from string
- `static unique_ptr<XmlReader> Create(istream& stream)` - Create reader from stream

### Reading Methods

- `bool Read()` - Advances to the next node. Returns `false` when no more nodes.

### Node Information

- `XmlNodeType NodeType() const` - Gets the type of the current node
- `string_view Name() const` - Gets the qualified name (with namespace prefix)
- `string_view LocalName() const` - Gets the local name (without namespace prefix)
- `string_view Prefix() const` - Gets the namespace prefix
- `string_view Value() const` - Gets the text value of the node
- `int Depth() const` - Gets the nesting depth (0-based)
- `bool IsEmptyElement() const` - Returns `true` for self-closing tags

### Attribute Methods

- `int AttributeCount() const` - Gets the number of attributes
- `bool HasAttributes() const` - Returns `true` if element has attributes
- `string_view GetAttribute(string_view name) const` - Gets specific attribute value
- `const map<string, string>& GetAttributes() const` - Gets all attributes

### State Methods

- `bool IsEOF() const` - Returns `true` if at end of document

## Performance Considerations

1. **Memory Efficiency**: XmlReader doesn't build a DOM tree, making it suitable for large documents
2. **Forward-Only**: Cannot go backward; must re-parse if you need to revisit nodes
3. **Stream Processing**: Can process XML from streams without loading entire file into memory
4. **Attribute Access**: Attributes are only available when positioned on an Element node

## Error Handling

The XmlReader throws `std::runtime_error` exceptions for malformed XML:
- Unexpected end of XML
- Invalid syntax
- Unclosed tags, comments, or CDATA sections
- Missing required characters (quotes, brackets, etc.)

```cpp
try {
    auto reader = cfdi::XmlReader::Create(xmlContent);
    while (reader->Read()) {
        // Process nodes
    }
} catch (const std::runtime_error& e) {
    std::cerr << "XML parsing error: " << e.what() << std::endl;
}
```

## Limitations

- **No Schema Validation**: Does not validate against XSD schemas
- **No DTD Processing**: DOCTYPE declarations are recognized but not processed
- **No XSL Support**: Does not handle XSL transformations
- **Limited Entity Support**: Supports common entities and numeric references, but not custom entity declarations
- **Forward-Only**: Cannot navigate backwards or randomly access nodes

## Thread Safety

XmlReader instances are **not thread-safe**. Each thread should use its own XmlReader instance.

## Example: Extracting Data from CFDI

```cpp
#include "xmlreader.hpp"
#include <map>
#include <vector>

struct Concepto {
    std::string descripcion;
    double cantidad;
    double valorUnitario;
    double importe;
};

std::vector<Concepto> extractConceptos(const std::string& xmlContent) {
    std::vector<Concepto> conceptos;
    auto reader = cfdi::XmlReader::Create(xmlContent);
    
    while (reader->Read()) {
        if (reader->NodeType() == cfdi::XmlNodeType::Element &&
            reader->LocalName() == "Concepto") {
            
            Concepto c;
            c.descripcion = reader->GetAttribute("Descripcion");
            c.cantidad = std::stod(std::string(reader->GetAttribute("Cantidad")));
            c.valorUnitario = std::stod(std::string(reader->GetAttribute("ValorUnitario")));
            c.importe = std::stod(std::string(reader->GetAttribute("Importe")));
            
            conceptos.push_back(c);
        }
    }
    
    return conceptos;
}
```

## Comparison with .NET XmlReader

The implementation closely mirrors .NET's `XmlReader` API with these differences:

1. Uses modern C++23 features (`string_view`, smart pointers)
2. Factory methods return `unique_ptr` instead of abstract class
3. Attribute access returns `map` instead of collection interface
4. Uses C++ standard library containers and types
5. Exception-based error handling (similar to .NET)

## Summary

The `XmlReader` class provides an efficient, .NET-like API for reading XML documents in C++. It's particularly well-suited for:

- Processing large XML files
- Reading CFDI v4 documents
- Sequential XML data extraction
- Situations where you don't need random access to nodes
- Memory-constrained environments
