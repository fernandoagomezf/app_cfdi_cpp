module cfdi.doc:cfdiscanner;

import std;
import :cfdiscanner;


using std::filesystem::current_path;
using std::filesystem::path;
using std::filesystem::filesystem_error;
using std::filesystem::directory_iterator;
using std::format;
using std::ifstream;
using std::runtime_error;
using std::string;
using std::string_view;
using cfdi::CFDIScanner;

string_view CFDIScanner::directory() const {
    return _directory;
}

CFDIScanner::const_iterator CFDIScanner::begin() const {
    return _files.begin();
}

CFDIScanner::const_iterator CFDIScanner::end() const {
    return _files.end();
}

CFDIScanner::size_type CFDIScanner::scan(string_view directory) {    
    try {
        _directory = directory;
        _files.clear();
        string ext { ".xml" };
        for (const auto& file : directory_iterator(_directory)) {
            if (file.is_regular_file()) {
                auto path { file.path() };
                if (path.extension() == ext) {
                    _files.push_back(path);
                }
            }
        }
    } catch (const filesystem_error& ex) {
        string msg { format("Error while scanning the directory '{0}': {1}", _directory, ex.what()) };
        throw runtime_error(msg);
    }

    return _files.size();
}

CFDIScanner::size_type CFDIScanner::scan() {    
    auto current { current_path().string() };
    return scan(current);
}