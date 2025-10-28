#ifndef _CFDI_HPP_
#define _CFDI_HPP_

#include <list>
#include <string>
#include "cfdiheader.hpp"
#include "cfdiissuer.hpp"
#include "cfdireceiver.hpp"
#include "cfdiconcept.hpp"
#include "cfditax.hpp"
#include "cfdicomplement.hpp"

namespace cfdi {
    using std::list;
    using std::string;
    using std::string_view;
    using cfdi::CFDIComplement;
    using cfdi::CFDIConcept;
    using cfdi::CFDIIssuer;
    using cfdi::CFDIReceiver;
    using cfdi::CFDITax;

    class Document {
        public:
            const CFDIHeader& header() const;
            const CFDIIssuer& issuer() const;
            const CFDIReceiver& receiver() const;
            const list<CFDIConcept>& concepts() const;
            const list<CFDITax>& taxes() const;
            const CFDIComplement& complement() const;

            static Document fromXml(string_view xml);
        
        private:
            CFDIHeader _header;             // cfdi:Comprobante
            CFDIIssuer _issuer;             // cfdi:Emisor
            CFDIReceiver _receiver;         // cfdi:Receptor
            list<CFDIConcept> _concepts;    // cfdi:Conceptos
            list<CFDITax> _taxes;           // cfdi:Impuestos
            CFDIComplement _complement;     // cfdi:Complemento
    };
}

#endif
