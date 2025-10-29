#include "summary.hpp"
#include <format>
#include "output.hpp"

using std::ostream;
using std::format;
using cfdi::escape_csv;
using cfdi::escape_json;
using cfdi::escape_xml;
using cfdi::format_index;
using cfdi::format_value;
using cfdi::Summary;
using cfdi::OutputFormat;

ostream& cfdi::operator<<(ostream& stream, const Summary& summary) {
    OutputFormat fmt { format_value(stream) };

    switch (fmt) {
        case OutputFormat::CSV:
            stream << escape_csv(summary.date) << ","
                << escape_csv(summary.description) << ","
                << escape_csv(summary.issuerTaxCode) << ","
                << escape_csv(summary.invoiceId) << ","
                << escape_csv(summary.subTotal) << ","
                << escape_csv(summary.taxes) << ","
                << escape_csv(summary.total);
            break;

        case OutputFormat::JSON:
            stream << format(R"({{"date":"{}","description":"{}","issuerTaxCode":"{}","invoiceId":"{}","subTotal":"{}","taxes":"{}","total":"{}"}})",
                escape_json(summary.date),
                escape_json(summary.description),
                escape_json(summary.issuerTaxCode),
                escape_json(summary.invoiceId),
                escape_json(summary.subTotal),
                escape_json(summary.taxes),
                escape_json(summary.total));
            break;

        case OutputFormat::XML:
            stream << "<Summary>\n"
                << "  <date>" << escape_xml(summary.date) << "</date>\n"
                << "  <description>" << escape_xml(summary.description) << "</description>\n"
                << "  <issuerTaxCode>" << escape_xml(summary.issuerTaxCode) << "</issuerTaxCode>\n"
                << "  <invoiceId>" << escape_xml(summary.invoiceId) << "</invoiceId>\n"
                << "  <subTotal>" << escape_xml(summary.subTotal) << "</subTotal>\n"
                << "  <taxes>" << escape_xml(summary.taxes) << "</taxes>\n"
                << "  <total>" << escape_xml(summary.total) << "</total>\n"
                << "</Summary>";
            break;
    }

    return stream;
}
