#ifndef _CFDITRANSFER_HPP_
#define _CFDITRANSFER_HPP_

#include <string>

namespace cfdi {
    using std::string;

    struct CFDITransfer {               // cfdi:Traslado
        string base;                    // cfdi:Traslado - Base
        string taxCode;                 // cfdi:Traslado - Impuesto
        string factorType;              // cfdi:Traslado - TipoFactor
        string rate;                    // cfdi:Traslado - TasaOCuota
        string amount;                  // cfdi:Traslado - Importe
    };
}

#endif 
