#ifndef _CFDIISSUER_HPP_
#define _CFDIISSUER_HPP_

#include <string>

namespace cfdi {
    using std::string;
    
    struct CFDIIssuer {                 // cfdi:Emisor
        string taxCode;                 // cfdi:Emisor - RFC
        string name;                    // cfdi:Emisor - Nombre
        string taxRegime;               // cfdi:Emisor - RegimenFiscal
    };
}

#endif 
