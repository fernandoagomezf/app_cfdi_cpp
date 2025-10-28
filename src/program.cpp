#include <iostream>
#include <fstream>
#include <filesystem>
#include <locale>
#include <format>
#include <stdexcept>
#include "document.hpp"

using std::cout;
using std::ifstream;
using std::filesystem::current_path;
using std::locale;
using std::ostringstream;
using std::format;
using std::runtime_error;
using std::string;
using std::println;
using cfdi::Document;
using cfdi::Summary;

int main() {
    locale esmx { "es_MX.UTF-8" };
    locale::global(esmx);
    cout.imbue(esmx);

    println(cout, "=== Blendwerk Procesador de CFDI ===\n");
    
    try {
        string fileName { "data/d0e7ed17-e3ec-41ca-9cf8-61b01da88fc5.xml" };
        ifstream file { fileName };
        if (!file.is_open()) {
            println(cout, "\t *** Archivo CFDI '{0}' no encontrado. ***", fileName);
            println(cout, "\t {0}", current_path().string());
            return -1;
        }

        ostringstream ss { };
        ss << file.rdbuf();
        auto xml { ss.str() };

        Document doc { Document::fromXml(xml) };
        Summary summary { doc.summarize() };
        println(cout, "\t Fecha: \t\t{0}",          summary.date);
        println(cout, "\t Descripción: \t\t{0}",    summary.description);
        println(cout, "\t RFC: \t\t\t{0}",          summary.issuerTaxCode);
        println(cout, "\t No. Factura: \t\t{0}",    summary.invoiceId);
        println(cout, "\t Sub Total: \t\t{0}",      summary.subTotal);
        println(cout, "\t IVA: \t\t\t{0}",          summary.taxes);
        println(cout, "\t Total: \t\t{0}",          summary.total);
        println(cout);

    } catch (const runtime_error& ex) {
        println(cout, "*** Error al leer archivo CFDI: {0} ***", ex.what());
    }

    println(cout, "\n=== FIN ===");

    return 0;
}
