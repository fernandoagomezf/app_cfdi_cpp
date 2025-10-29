#ifndef _OUTPUT_HPP_
#define _OUTPUT_HPP_

#include <iostream>
#include <string>

namespace cfdi {
    using std::ostream;
    using std::string;
    using std::string_view;

    enum class OutputFormat {
        CSV,
        JSON,
        XML
    };

    ostream& csv(ostream& stream);
    ostream& json(ostream& stream);
    ostream& xml(ostream& stream);
    string escape_csv(string_view field);
    string escape_json(string_view field);
    string escape_xml(string_view field);
    int format_index();
    OutputFormat format_value(ostream& stream);
}

#endif 
