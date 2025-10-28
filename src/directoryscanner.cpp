#include "directoryscanner.hpp"
#include <fstream>
#include <filesystem>
#include <format>
#include <stdexcept>

using std::filesystem::current_path;
using std::filesystem::path;
using std::filesystem::filesystem_error;
using std::filesystem::directory_iterator;
using std::format;
using std::ifstream;
using std::runtime_error;
using std::string;
using std::string_view;
using cfdi::DirectoryScanner;

string_view DirectoryScanner::directory() const {
    return _directory;
}

DirectoryScanner::const_iterator DirectoryScanner::begin() const {
    return _files.begin();
}

DirectoryScanner::const_iterator DirectoryScanner::end() const {
    return _files.end();
}

DirectoryScanner::size_type DirectoryScanner::scan(string_view directory) {    
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

DirectoryScanner::size_type DirectoryScanner::scan() {    
    auto current { current_path().string() };
    return scan(current);
}