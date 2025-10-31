export module cfdi.doc:cfditax;

import std;
import :cfditransfer;

namespace cfdi {
    using std::string;
    using std::list;
    using cfdi::CFDITransfer;

    export struct CFDITax {             // cfdi:Impuesto
        string total;                   // cfdi:Impuesto - TotalImpuestosTrasladados
        list<CFDITransfer> transfers;   // cfdi:Impuesto / cfdi:Traslados
    }; 
}
