export module cfdi.doc:cfdiwritter;

import std;
import :cfdisummary;

namespace cfdi {
    using std::ostream;
    using std::string;
    using std::string_view;
    using cfdi::CFDISummary;

    export class CFDIWritter {
        public:
            string writeCsv(const CFDISummary& summary);
            string writeJson(const CFDISummary& summary);
            string writeXml(const CFDISummary& summary);

        private:
            string escapeCsv(string_view field);
            string escapeJson(string_view field);
            string escapeXml(string_view field);
    };
}