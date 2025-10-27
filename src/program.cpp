#include <iostream>
#include <fstream>
#include <filesystem>
#include <fmt/core.h>
#include "xmlreader.hpp"

using std::cin;
using std::string;
using std::ifstream;
using fmt::print;
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
    
    auto reader = XmlReader::create(file);
    
    // Read and display XML structure
    while (reader->read()) {
        // Indent based on depth
        for (int i = 0; i < reader->depth(); i++) {
            print("  ");
        }
        
        auto node = reader->current(); 
        switch (node.nodeType) {
            case XmlNodeType::XmlDeclaration:
                print("XmlDeclaration: {}\n", node.name);
                if (node.hasAttributes()) {
                    for (const auto& [name, value] : node.attributes) {
                        for (int i = 0; i < node.depth + 1; i++) {
                            print("  ");
                        } 
                        print("@{} = \"{}\"\n", name, value);
                    }
                }
                break;
                
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
                
            case XmlNodeType::Whitespace:
                // Skip whitespace nodes in output
                break;
                
            case XmlNodeType::Comment:
                print("Comment: {}\n", node.value);
                break;
                
            case XmlNodeType::CDATA:
                print("CDATA: {}\n", node.value);
                break;
                
            case XmlNodeType::ProcessingInstruction:
                print("ProcessingInstruction: {} = \"{}\"\n", node.name, node.value);
                break;
                
            default:
                break;
        }
    }
    
    print("\n=== End of XmlReader Demo ===\n");

    return 0;
}
