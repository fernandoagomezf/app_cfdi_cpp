#ifndef _CFDISTAMP_HPP_
#define _CFDISTAMP_HPP_

#include <string>

namespace cfdi {
    using std::string;

    struct CFDIStamp {                  // tfd:TimbreFiscalDigital
        string version;                 // tfd:TimbreFiscalDigital - Version
        string uuid;                    // tfd:TimbreFiscalDigital - UUID
        string date;                    // tfd:TimbreFiscalDigital - FechaTimbrado
        string provider;                // tfd:TimbreFiscalDigital - RfcProvCertif
        string seal;                    // tfd:TimbreFiscalDigital - SelloCFDI
        string satCertificate;          // tfd:TimbreFiscalDigital - NoCertificadoSAT
        string satSeal;                 // tfd:TimbreFiscalDigital - SelloSAT
    };
}

#endif 
