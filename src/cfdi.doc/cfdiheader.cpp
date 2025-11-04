module cfdi.doc:cfdiheader;

import std;
import :cfdiheader;

using std::string;
using cfdi::CFDIHeader;

string CFDIHeader::shortDate() const {
    auto pos = date.find('T');
    return pos != string::npos ? date.substr(0, pos) : date;
}