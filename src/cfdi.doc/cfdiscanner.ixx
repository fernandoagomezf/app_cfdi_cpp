export module cfdi.doc:cfdiscanner;

import std;

namespace cfdi {
    using std::filesystem::path;
    using std::list;
    using std::string;
    using std::string_view;

    export class CFDIScanner {
        using container_t = list<path>;
        using size_type = container_t::size_type;
        using const_iterator = container_t::const_iterator;

        public:
            size_type scan();
            size_type scan(string_view directory);

            string_view directory() const;
            const_iterator begin() const;
            const_iterator end() const;

        private:
            string _directory;
            container_t _files;
    };
}