#ifndef _SUMMARY_HPP_
#define _SUMMARY_HPP_

#include <string>
#include <ostream>
#include <ios>

namespace cfdi {
    using std::string;
    using std::ostream;
    using std::ios_base;

    struct Summary {
        string date;
        string description;
        string issuerTaxCode;
        string invoiceId;
        string subTotal;
        string taxes;
        string total;
    };

    ostream& operator<<(ostream& stream, const Summary& summary);
}

#endif 
