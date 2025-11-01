export module cfdi.doc:cfdistamp;

import std;

namespace cfdi {
    using std::string;

    export struct CFDIStamp {           // tfd:TimbreFiscalDigital
        string version;                 // tfd:TimbreFiscalDigital - Version
        string uuid;                    // tfd:TimbreFiscalDigital - UUID
        string date;                    // tfd:TimbreFiscalDigital - FechaTimbrado
        string provider;                // tfd:TimbreFiscalDigital - RfcProvCertif
        string seal;                    // tfd:TimbreFiscalDigital - SelloCFDI
        string satCertificate;          // tfd:TimbreFiscalDigital - NoCertificadoSAT
        string satSeal;                 // tfd:TimbreFiscalDigital - SelloSAT
    };
}