export module cfdi.xml:xmlnodetype;

namespace cfdi {
    export enum class XmlNodeType {
        None,
        Element,
        EndElement,
        Text,
        CDATA,
        Comment,
        ProcessingInstruction,
        XmlDeclaration,
        Whitespace,
        DocumentType
    };
}