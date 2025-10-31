export module cfdi.doc:cfditransfer;

import std;

namespace cfdi {
    using std::string;

    export struct CFDITransfer {        // cfdi:Traslado
        string base;                    // cfdi:Traslado - Base
        string code;                    // cfdi:Traslado - Impuesto
        string factorType;              // cfdi:Traslado - TipoFactor
        string rate;                    // cfdi:Traslado - TasaOCuota
        string amount;                  // cfdi:Traslado - Importe
    };
}