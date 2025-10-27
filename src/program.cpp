#include <iostream>
#include <fstream>
#include <filesystem>
#include <fmt/core.h>
#include "xmlreader.hpp"

using std::cin;
using std::ostringstream;
using std::string;
using std::ifstream;
using fmt::print;
using cfdi::XmlNode;
using cfdi::XmlNodeType;
using cfdi::XmlReader;

int main() {
    print("\n=== XmlReader Demo ===\n");

    string fileName { "../data/d0e7ed17-e3ec-41ca-9cf8-61b01da88fc5.xml" };
    ifstream file(fileName);
    if (!file.is_open()) {
        print("\n *** CFDI file {} not found.", fileName);
        print("\n {}", std::filesystem::current_path().string());
        cin.get();
        return -1;
    }
    
    XmlReader reader { file };
    
    while (reader.read()) {        
        XmlNode node { reader.current() };
        for (int i = 0; i < node.depth; i++) {
            print("  ");
        }
        
        switch (node.nodeType) {
            case XmlNodeType::Element:
                print("Element: {} (prefix: '{}', localName: '{}')", node.name, node.prefix, node.localName);
                if (node.isEmpty) {
                    print(" [Empty]");
                }
                print("\n");
                
                if (node.hasAttributes()) {
                    for (const auto& [name, value] : node.attributes) {
                        for (int i = 0; i < node.depth + 1; i++) {
                            print("  ");
                        }
                        print("@{} = \"{}\"\n", name, value);
                    }
                }
                break;
                
            case XmlNodeType::EndElement:
                print("EndElement: {}\n", node.name);
                break;
                
            case XmlNodeType::Text:
                if (!node.value.empty()) {
                    print("Text: \"{}\"\n", node.value);
                }
                break;
                
            default:
                break;
        }
    }
    
    print("\n=== End of XmlReader Demo ===\n");

    return 0;
}
