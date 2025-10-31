export module cfdi.doc:cfdireceiver;

import std;

namespace cfdi {
    using std::string;
    
    export struct CFDIReceiver {               // cfdi:Receptor
        string taxCode;                 // cfdi:Receptor - RFC
        string name;                    // cfdi:Receptor - Nombre
        string address;                 // cfdi:Receptor - DomicilioFiscalReceptor
        string taxRegime;               // cfdi:Receptor - RegimenFiscalReceptor
        string usage;                   // cfdi:Receptor - UsoCFDI
    };
}