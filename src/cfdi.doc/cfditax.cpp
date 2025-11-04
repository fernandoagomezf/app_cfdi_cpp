module cfdi.doc:cfditax;

import std;
import :cfditax;

using std::all_of;
using std::isspace;
using cfdi::CFDITax;

bool CFDITax::isExent() const {
    auto lambda = [](unsigned char ch) {
        return isspace(ch);
    };
    return total.empty()
        || all_of(total.begin(), total.end(), lambda);
}