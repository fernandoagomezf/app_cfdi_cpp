export module cfdi.doc:cfdisummary;

import std;

namespace cfdi {
    using std::string;
    using std::ostream;
    using std::ios_base;

    export struct CFDISummary {
        string date;
        string description;
        string issuerTaxCode;
        string invoiceId;
        string paymentMethod;
        string placeOfIssue;
        string subTotal;
        string taxes;
        string total;
    };
}