#ifndef _XMLNODETYPE_HPP_
#define _XMLNODETYPE_HPP_

namespace cfdi {
    enum class XmlNodeType {
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

#endif 
