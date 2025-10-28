#ifndef _CFDITAX_HPP_
#define _CFDITAX_HPP_

#include <string>
#include <list>
#include "cfditransfer.hpp"

namespace cfdi {
    using std::string;
    using std::list;
    using cfdi::CFDITransfer;

    struct CFDITax {                    // cfdi:Impuesto
        string total;                   // cfdi:Impuesto - TotalImpuestosTrasladados
        list<CFDITransfer> transfers;   // cfdi:Impuesto / cfdi:Traslados
    };
}

#endif 
