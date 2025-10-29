#include "output.hpp"
#include <format>

using std::ios_base;
using std::ostream;
using std::string;
using std::string_view;
using cfdi::OutputFormat;
using cfdi::csv;
using cfdi::json;
using cfdi::xml;
using cfdi::format_index;
using cfdi::format_value;

int cfdi::format_index() {
    static int index = ios_base::xalloc();
    return index;
}

OutputFormat cfdi::format_value(ostream& stream) {
    auto idx { format_index() };
    auto fmt { stream.iword(idx) };
    return static_cast<OutputFormat>(fmt);
}

ostream& cfdi::csv(ostream& stream) {
    stream.iword(format_index()) = static_cast<long>(OutputFormat::CSV);
    return stream;
}

ostream& cfdi::json(ostream& stream) {
    stream.iword(format_index()) = static_cast<long>(OutputFormat::JSON);
    return stream;
}

ostream& cfdi::xml(ostream& stream) {
    stream.iword(format_index()) = static_cast<long>(OutputFormat::XML);
    return stream;
}

string cfdi::escape_csv(string_view field) {
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

string cfdi::escape_json(string_view field) {
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

string cfdi::escape_xml(string_view field) {
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