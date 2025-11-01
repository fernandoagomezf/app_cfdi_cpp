export module cfdi.doc:cfdidocument;

import std;
import :cfdicomplement;
import :cfdiconcept;
import :cfdiheader;
import :cfdiissuer;
import :cfdireceiver;
import :cfditax;
import :cfdisummary;

namespace cfdi {
    using std::istream;
    using std::list;
    using std::string;
    using std::string_view;
    using cfdi::CFDIComplement;
    using cfdi::CFDIConcept;
    using cfdi::CFDIHeader;
    using cfdi::CFDIIssuer;
    using cfdi::CFDIReceiver;
    using cfdi::CFDITax;
    using cfdi::CFDISummary;

    export class CFDIDocument {
        public:
            const CFDIHeader& header() const;
            const CFDIIssuer& issuer() const;
            const CFDIReceiver& receiver() const;
            const list<CFDIConcept>& concepts() const;
            const CFDITax& taxes() const;
            const CFDIComplement& complement() const;

            CFDISummary summarize() const;

            static CFDIDocument fromXml(string_view xml);
            
        private:
            CFDIHeader _header;             // cfdi:Comprobante
            CFDIIssuer _issuer;             // cfdi:Emisor
            CFDIReceiver _receiver;         // cfdi:Receptor
            list<CFDIConcept> _concepts;    // cfdi:Conceptos
            CFDITax _taxes;                 // cfdi:Impuestos
            CFDIComplement _complement;     // cfdi:Complemento
    };
}