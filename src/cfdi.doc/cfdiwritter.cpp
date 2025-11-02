module cfdi.doc:cfdiwritter;

import std;
import :cfdisummary;
import :cfdiwritter;

using std::ios_base;
using std::ostream;
using std::string;
using std::format;
using std::string_view;
using cfdi::CFDISummary;
using cfdi::CFDIWritter;
using std::ostringstream;

string CFDIWritter::writeCsv(const CFDISummary& summary) {
    ostringstream stream;
    stream << escapeCsv(summary.date) << ","
           << escapeCsv(summary.description) << ","
           << escapeCsv(summary.issuerTaxCode) << ","
           << escapeCsv(summary.invoiceId) << ","
           << escapeCsv(summary.paymentMethod) << ","
           << escapeCsv(summary.placeOfIssue) << ","
           << escapeCsv(summary.subTotal) << ","
           << escapeCsv(summary.taxes) << ","
           << escapeCsv(summary.total);
    return stream.str();
}

string CFDIWritter::writeJson(const CFDISummary& summary) {
    ostringstream stream;
    stream << format(R"({{"date":"{}","description":"{}","issuerTaxCode":"{}","invoiceId":"{}","paymentMethod":"{}","placeOfIssue":"{}","subTotal":"{}","taxes":"{}","total":"{}"}})",
              escapeJson(summary.date),
              escapeJson(summary.description),
              escapeJson(summary.issuerTaxCode),
              escapeJson(summary.invoiceId),
              escapeJson(summary.paymentMethod),
              escapeJson(summary.placeOfIssue),
              escapeJson(summary.subTotal),
              escapeJson(summary.taxes),
              escapeJson(summary.total));
    return stream.str();
}

string CFDIWritter::writeXml(const CFDISummary& summary) {
    ostringstream stream;
    stream << "<Summary>\n"
           << "  <date>" << escapeXml(summary.date) << "</date>\n"
           << "  <description>" << escapeXml(summary.description) << "</description>\n"
           << "  <issuerTaxCode>" << escapeXml(summary.issuerTaxCode) << "</issuerTaxCode>\n"
           << "  <invoiceId>" << escapeXml(summary.invoiceId) << "</invoiceId>\n"
           << "  <paymentMethod>" << escapeXml(summary.paymentMethod) << "</paymentMethod>\n"
           << "  <placeOfIssue>" << escapeXml(summary.placeOfIssue) << "</placeOfIssue>\n"
           << "  <subTotal>" << escapeXml(summary.subTotal) << "</subTotal>\n"
           << "  <taxes>" << escapeXml(summary.taxes) << "</taxes>\n"
           << "  <total>" << escapeXml(summary.total) << "</total>\n"
           << "</Summary>";
    return stream.str();
}

string CFDIWritter::escapeCsv(string_view field) {
    string escaped { field };
    if (field.find(',') != string::npos || 
        field.find('"') != string::npos || 
        field.find('\n') != string::npos) {
        escaped = "\"";
        for (char c : field) {
            if (c == '"') {
                escaped += "\"\"";  
            } else {
                escaped += c;
            }
        }
        escaped += "\"";
    }
    return escaped;
}

string CFDIWritter::escapeJson(string_view field) {
        string escaped;
        for (char c : field) {
            switch (c) {
                case '"': escaped += "\\\""; break;
                case '\\': escaped += "\\\\"; break;
                case '\b': escaped += "\\b"; break;
                case '\f': escaped += "\\f"; break;
                case '\n': escaped += "\\n"; break;
                case '\r': escaped += "\\r"; break;
                case '\t': escaped += "\\t"; break;
                default: escaped += c; break;
            }
        }
        return escaped;
    }

string CFDIWritter::escapeXml(string_view field) {
    string escaped;
    for (char c : field) {
        switch (c) {
            case '&': escaped += "&amp;"; break;
            case '<': escaped += "&lt;"; break;
            case '>': escaped += "&gt;"; break;
            case '"': escaped += "&quot;"; break;
            case '\'': escaped += "&apos;"; break;
            default: escaped += c; break;
        }
    }
    return escaped;
}

