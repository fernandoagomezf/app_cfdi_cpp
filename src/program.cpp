#include <fmt/core.h>
#include <boost/uuid/uuid.hpp>
#include <boost/uuid/uuid_generators.hpp>
#include <boost/uuid/uuid_io.hpp>
#include "xmlreader.hpp"

int main() {
    fmt::print("Hello {}\n", "World!");

    boost::uuids::random_generator gen;
    boost::uuids::uuid uid = gen();

    auto str = boost::uuids::to_string(uid);
    fmt::print("Some uuid: {}\n", str);

    // Example XML (CFDI v4 structure sample)
    std::string xmlSample = R"(<?xml version="1.0" encoding="UTF-8"?>
<cfdi:Comprobante xmlns:cfdi="http://www.sat.gob.mx/cfd/4" 
                   xmlns:xsi="http://www.w3.org/2001/XMLSchema-instance"
                   Version="4.0" 
                   Serie="A" 
                   Folio="12345"
                   Fecha="2025-10-26T12:00:00"
                   Sello="" 
                   FormaPago="01" 
                   NoCertificado="00001000000123456789" 
                   Certificado="" 
                   SubTotal="1000.00" 
                   Moneda="MXN" 
                   Total="1160.00" 
                   TipoDeComprobante="I" 
                   Exportacion="01" 
                   MetodoPago="PUE" 
                   LugarExpedicion="01000">
    <cfdi:Emisor Rfc="XAXX010101000" Nombre="Emisor de Prueba" RegimenFiscal="601"/>
    <cfdi:Receptor Rfc="XAXX010101000" Nombre="Receptor de Prueba" DomicilioFiscalReceptor="01000" 
                   RegimenFiscalReceptor="601" UsoCFDI="G03"/>
    <cfdi:Conceptos>
        <cfdi:Concepto ClaveProdServ="01010101" 
                       Cantidad="1" 
                       ClaveUnidad="E48" 
                       Descripcion="Producto de prueba" 
                       ValorUnitario="1000.00" 
                       Importe="1000.00" 
                       ObjetoImp="02">
            <cfdi:Impuestos>
                <cfdi:Traslados>
                    <cfdi:Traslado Base="1000.00" Impuesto="002" TipoFactor="Tasa" TasaOCuota="0.160000" Importe="160.00"/>
                </cfdi:Traslados>
            </cfdi:Impuestos>
        </cfdi:Concepto>
    </cfdi:Conceptos>
    <cfdi:Impuestos TotalImpuestosTrasladados="160.00">
        <cfdi:Traslados>
            <cfdi:Traslado Base="1000.00" Impuesto="002" TipoFactor="Tasa" TasaOCuota="0.160000" Importe="160.00"/>
        </cfdi:Traslados>
    </cfdi:Impuestos>
</cfdi:Comprobante>
)";

    fmt::print("\n=== XmlReader Demo ===\n");
    
    // Create XmlReader instance
    auto reader = cfdi::XmlReader::Create(xmlSample);
    
    // Read and display XML structure
    while (reader->Read()) {
        // Indent based on depth
        for (int i = 0; i < reader->Depth(); i++) {
            fmt::print("  ");
        }
        
        switch (reader->NodeType()) {
            case cfdi::XmlNodeType::XmlDeclaration:
                fmt::print("XmlDeclaration: {}\n", reader->Name());
                if (reader->HasAttributes()) {
                    for (const auto& [name, value] : reader->GetAttributes()) {
                        for (int i = 0; i < reader->Depth() + 1; i++) fmt::print("  ");
                        fmt::print("@{} = \"{}\"\n", name, value);
                    }
                }
                break;
                
            case cfdi::XmlNodeType::Element:
                fmt::print("Element: {} (prefix: '{}', localName: '{}')", 
                          reader->Name(), reader->Prefix(), reader->LocalName());
                if (reader->IsEmptyElement()) {
                    fmt::print(" [Empty]");
                }
                fmt::print("\n");
                
                if (reader->HasAttributes()) {
                    for (const auto& [name, value] : reader->GetAttributes()) {
                        for (int i = 0; i < reader->Depth() + 1; i++) fmt::print("  ");
                        fmt::print("@{} = \"{}\"\n", name, value);
                    }
                }
                break;
                
            case cfdi::XmlNodeType::EndElement:
                fmt::print("EndElement: {}\n", reader->Name());
                break;
                
            case cfdi::XmlNodeType::Text:
                if (!reader->Value().empty()) {
                    fmt::print("Text: \"{}\"\n", reader->Value());
                }
                break;
                
            case cfdi::XmlNodeType::Whitespace:
                // Skip whitespace nodes in output
                break;
                
            case cfdi::XmlNodeType::Comment:
                fmt::print("Comment: {}\n", reader->Value());
                break;
                
            case cfdi::XmlNodeType::CDATA:
                fmt::print("CDATA: {}\n", reader->Value());
                break;
                
            case cfdi::XmlNodeType::ProcessingInstruction:
                fmt::print("ProcessingInstruction: {} = \"{}\"\n", reader->Name(), reader->Value());
                break;
                
            default:
                break;
        }
    }
    
    fmt::print("\n=== End of XmlReader Demo ===\n");

    return 0;
}
