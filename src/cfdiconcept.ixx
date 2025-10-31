export module cfdi.doc:cfdiconcept;

import std;
import :cfditax;

namespace cfdi {
    using std::list;
    using std::string;
    using cfdi::CFDITax;

    export struct CFDIConcept {         // cfdi:Concepto
        string code;                    // cfdi:Concepto - ClaveProdServ
        string idNumber;                // cfdi:Concepto - NoIdentificacion
        string quantity;                // cfdi:Concepto - Cantidad
        string unitCode;                // cfdi:Concepto - ClaveUnidad
        string unitName;                // cfdi:Concepto - Unidad
        string description;             // cfdi:Concepto - Descripcion
        string unitValue;               // cfdi:Concepto - ValorUnitario
        string amount;                  // cfdi:Concepto - Importe
        string impObject;               // cfdi:Concepto - ObjetoImp ¡wtf is this!
        list<CFDITax> taxes;            // cfdi:Concepto / cfdi:Impuestos
    };
}