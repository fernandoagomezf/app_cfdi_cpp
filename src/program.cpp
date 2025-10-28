#include <iostream>
#include <fstream>
#include <filesystem>
#include <locale>
#include <format>
#include <stdexcept>
#include "document.hpp"
#include "directoryscanner.hpp"
#include "utilities.hpp"

using std::cout;
using std::cin;
using std::ifstream;
using std::filesystem::current_path;
using std::locale;
using std::ostringstream;
using std::exception;
using std::format;
using std::runtime_error;
using std::string;
using std::println;
using std::endl;
using cfdi::Document;
using cfdi::Summary;
using cfdi::DirectoryScanner;

void logo();

int main() {
    locale esmx { "es_MX.UTF-8" };
    locale::global(esmx);
    cout.imbue(esmx);

    println(cout, "=== Blendwerk Procesador de CFDI v0.1 ===\n");
    
    try {
        DirectoryScanner scanner { };

        cout << format("Buscando archivos CFDI...") << endl;
        auto found = scanner.scan("data/");
        cout << format("Se encontraron {0} archivos. ", found) << endl;

        for (auto it = scanner.begin(); it != scanner.end(); ++it) {
            ifstream file(*it);
            if (file.is_open()){
                ostringstream ss { };
                ss << file.rdbuf();
                auto xml { ss.str() };

                try {
                    cout << format("== Procesando archivo {0} ==", it->string()) << endl;

                    Document doc { Document::fromXml(xml) };
                    Summary summary { doc.summarize() };

                    cout << format("\t Fecha: \t\t{0}", summary.date) << endl;;
                    cout << format("\t Descripción: \t\t{0}", summary.description) << endl;
                    cout << format("\t RFC: \t\t\t{0}", summary.issuerTaxCode) << endl;
                    cout << format("\t No. Factura: \t\t{0}", summary.invoiceId) << endl;
                    cout << format("\t Sub Total: \t\t{0}", summary.subTotal) << endl;
                    cout << format("\t IVA: \t\t\t{0}", summary.taxes) << endl;
                    cout << format("\t Total: \t\t{0}", summary.total) << endl;
                    cout << endl;
                } catch (const runtime_error& ex) {
                    cout << format("*** Error: no se pudo procesar el archivo {0}", it->string()) << endl;
                    cout << format("*** Motivo: {0}", ex.what()) << endl;
                }
            } else {
                cout << format("*** Error: no se pudo abrir el archivo CFDI: {0} ***", it->string());
            }
        }
    } catch (const exception& ex) {
        cout << format("*** Error inesperado, el programa será abortado: {0} ***", ex.what()) << endl;
    }

    println(cout, "\n=== FIN ===");
    print(cout, "\nPresione 'Enter' para continuar...");
    cin.get();

    cfdi::about(cout);

    return 0;
}

