#include "document.hpp"

using std::string_view;
using cfdi::Document;

Document::Document(string_view version)
    : _version { version } 
{

}

string_view Document::getVersion() const {
    return _version;
}
