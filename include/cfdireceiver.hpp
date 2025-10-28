#ifndef _CFDIRECEIVER_HPP_
#define _CFDIRECEIVER_HPP_

#include <string>

namespace cfdi {
    using std::string;
    
    struct CFDIReceiver {               // cfdi:Receptor
        string taxCode;                 // cfdi:Receptor - RFC
        string name;                    // cfdi:Receptor - Nombre
        string address;                 // cfdi:Receptor - DomicilioFiscalReceptor
        string taxRegime;               // cfdi:Receptor - RegimenFiscalReceptor
        string usage;                   // cfdi:Receptor - UsoCFDI
    };
}

#endif 
