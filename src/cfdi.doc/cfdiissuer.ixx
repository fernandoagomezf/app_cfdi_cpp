export module cfdi.doc:cfdiissuer;

import std;

namespace cfdi {
    using std::string;
    
    export struct CFDIIssuer {          // cfdi:Emisor
        string taxCode;                 // cfdi:Emisor - RFC
        string name;                    // cfdi:Emisor - Nombre
        string taxRegime;               // cfdi:Emisor - RegimenFiscal
    };
}