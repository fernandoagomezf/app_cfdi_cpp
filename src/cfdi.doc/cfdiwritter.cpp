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
           << escapeCsv(summary.issuerName) << ","
           << escapeCsv(summary.description) << ","
           << escapeCsv(summary.issuerTaxCode) << ","
           << escapeCsv(summary.invoiceId) << ","
           << escapeCsv(summary.paymentMethod) << ","
           << escapeCsv(summary.paymentType) << ","
           << escapeCsv(summary.placeOfIssue) << ","
           << escapeCsv(summary.subTotal) << ","
           << escapeCsv(summary.taxes) << ","
           << escapeCsv(summary.total);
    return stream.str();
}

string CFDIWritter::writeJson(const CFDISummary& summary) {
    ostringstream stream;
    stream << "{\n"
           << "  \"date\": \"" << escapeJson(summary.date) << "\",\n"
           << "  \"issuerName\": \"" << escapeJson(summary.issuerName) << "\",\n"
           << "  \"description\": \"" << escapeJson(summary.description) << "\",\n"
           << "  \"issuerTaxCode\": \"" << escapeJson(summary.issuerTaxCode) << "\",\n"
           << "  \"invoiceId\": \"" << escapeJson(summary.invoiceId) << "\",\n"
           << "  \"paymentMethod\": \"" << escapeJson(summary.paymentMethod) << "\",\n"
           << "  \"paymentType\": \"" << escapeJson(summary.paymentType) << "\",\n"
           << "  \"placeOfIssue\": \"" << escapeJson(summary.placeOfIssue) << "\",\n"
           << "  \"subTotal\": \"" << escapeJson(summary.subTotal) << "\",\n"
           << "  \"taxes\": \"" << escapeJson(summary.taxes) << "\",\n"
           << "  \"total\": \"" << escapeJson(summary.total) << "\"\n"
           << "}";

    return stream.str();
}

string CFDIWritter::writeXml(const CFDISummary& summary) {
    ostringstream stream;
    stream << "<Summary>\n"
           << "  <date>" << escapeXml(summary.date) << "</date>\n"
           << "  <issuerName>" << escapeXml(summary.issuerName) << "</issuerName>\n"
           << "  <description>" << escapeXml(summary.description) << "</description>\n"
           << "  <issuerTaxCode>" << escapeXml(summary.issuerTaxCode) << "</issuerTaxCode>\n"
           << "  <invoiceId>" << escapeXml(summary.invoiceId) << "</invoiceId>\n"
           << "  <paymentMethod>" << escapeXml(summary.paymentMethod) << "</paymentMethod>\n"
           << "  <paymentType>" << escapeXml(summary.paymentType) << "</paymentType>\n"
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

