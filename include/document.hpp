#ifndef _CFDI_HPP_
#define _CFDI_HPP_

#include <list>
#include <string>
#include <iostream>
#include "cfdiheader.hpp"
#include "cfdiissuer.hpp"
#include "cfdireceiver.hpp"
#include "cfdiconcept.hpp"
#include "cfditax.hpp"
#include "cfdicomplement.hpp"
#include "summary.hpp"

namespace cfdi {
    using std::istream;
    using std::list;
    using std::string;
    using std::string_view;
    using cfdi::CFDIComplement;
    using cfdi::CFDIConcept;
    using cfdi::CFDIIssuer;
    using cfdi::CFDIReceiver;
    using cfdi::CFDITax;
    using cfdi::Summary;

    class Document {
        public:
            const CFDIHeader& header() const;
            const CFDIIssuer& issuer() const;
            const CFDIReceiver& receiver() const;
            const list<CFDIConcept>& concepts() const;
            const CFDITax& taxes() const;
            const CFDIComplement& complement() const;

            Summary summarize() const;

            static Document fromXml(string_view xml);
        
        private:
            CFDIHeader _header;             // cfdi:Comprobante
            CFDIIssuer _issuer;             // cfdi:Emisor
            CFDIReceiver _receiver;         // cfdi:Receptor
            list<CFDIConcept> _concepts;    // cfdi:Conceptos
            CFDITax _taxes;                 // cfdi:Impuestos
            CFDIComplement _complement;     // cfdi:Complemento
    };
}

#endif
