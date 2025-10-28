#ifndef _SUMMARY_HPP_
#define _SUMMARY_HPP_

#include <string>

namespace cfdi {
    using std::string;

    struct Summary {
        string date;
        string description;
        string issuerTaxCode;
        string invoiceId;
        string subTotal;
        string taxes;
        string total;
    };
}

#endif 
