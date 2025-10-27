#ifndef _CFDI_HPP_
#define _CFDI_HPP_

#include <string>

namespace cfdi {
    using std::string;
    using std::string_view;

    class Document {
        public:
            Document(string_view version);

            string_view getVersion() const;

        private:
            string _version;
    };
}

#endif
