#include <iostream>
#include <fstream>
#include <filesystem>
#include <locale>
#include <format>
#include <stdexcept>
#include "document.hpp"
#include "directoryscanner.hpp"
#include "output.hpp"
#include "utilities.hpp"

using std::cout;
using std::cin;
using std::endl;
using std::exception;
using std::filesystem::current_path;
using std::format;
using std::ifstream;
using std::locale;
using std::ofstream;
using std::ostringstream;
using std::runtime_error;
using std::string;
using cfdi::csv;
using cfdi::json;
using cfdi::DirectoryScanner;
using cfdi::Document;
using cfdi::Summary;
using cfdi::xml;

int main() {
    locale esmx { "es_MX.UTF-8" };
    locale::global(esmx);
    cout.imbue(esmx);

    cout << "=== Blendwerk Procesador de CFDI v0.1 ===" << endl;
    cout << endl;    
    
    try {
        cout << "Ingrese el directorio donde se encuentran sus CFDIs> " << endl;
        string path;
        cin >> path;
        cout << endl;

        DirectoryScanner scanner { };
        auto found = scanner.scan(path);
        cout << format("Se encontraron {0} archivos. ", found) << endl;
        
        ofstream csvFile { format("{0}/cfdis.csv", path) };
        csvFile << csv << "Fecha,Descripción,RFC,Factura,SubTotal,IVA,Total" << endl;

        for (auto it = scanner.begin(); it != scanner.end(); ++it) {
            ifstream cfdiFile { *it };
            if (cfdiFile.is_open()){
                ostringstream ss { };
                ss << cfdiFile.rdbuf();
                auto str { ss.str() };

                try {
                    cout << format("\tProcesando archivo {0} ==", it->string()) << endl;

                    Document doc { Document::fromXml(str) };
                    Summary summary { doc.summarize() };
                    csvFile << csv << summary << endl;
                } catch (const runtime_error& ex) {
                    cout << format("*** Error: no se pudo procesar el archivo {0}", it->string()) << endl;
                    cout << format("*** Motivo: {0}", ex.what()) << endl;
                }
            } else {
                cout << format("*** Error: no se pudo abrir el archivo CFDI: {0} ***", it->string());
            }
        }
        csvFile << endl;
        cout << "Archivos procesados con éxito." << endl;

    } catch (const exception& ex) {
        cout << format("*** Error inesperado, el programa será abortado: {0} ***", ex.what()) << endl;
    }

    println(cout, "\n=== FIN ===");
    print(cout, "\nPresione 'Enter' para continuar...");

    return 0;
}

