export module cfdi.doc:cfdicomplement;

import std;
import :cfdistamp;

namespace cfdi {
    using cfdi::CFDIStamp;
 
    export struct CFDIComplement {
        CFDIStamp stamp;                // tfd:TimbreFiscalDigital
    };
}