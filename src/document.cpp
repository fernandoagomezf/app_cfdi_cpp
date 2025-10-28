#include "document.hpp"
#include "xmlreader.hpp"
#include <sstream>
#include <ranges>
#include <algorithm>
#include <numeric>
#include "utilities.hpp"

using std::list;
using std::ostringstream;
using std::string;
using std::string_view;
using cfdi::CFDIComplement;
using cfdi::CFDIConcept;
using cfdi::CFDIHeader;
using cfdi::CFDIIssuer;
using cfdi::CFDIReceiver;
using cfdi::CFDITax;
using cfdi::Document;
using cfdi::Summary;
using cfdi::XmlReader;
using cfdi::join;

const CFDIHeader& Document::header() const {
    return _header;
}

const CFDIIssuer& Document::issuer() const {
    return _issuer;
}

const CFDIReceiver& Document::receiver() const {
    return _receiver;
}

const list<CFDIConcept>& Document::concepts() const {
    return _concepts;
}

const CFDITax& Document::taxes() const {
    return _taxes;
}

const CFDIComplement& Document::complement() const {
    return _complement;
}

Summary Document::summarize() const {
    return  { 
        .date = _header.date, 
        .description = join(_concepts, ", ", [](const CFDIConcept& c) { return c.description; }),
        .issuerTaxCode = _issuer.taxCode, 
        .invoiceId = _complement.stamp.uuid, 
        .subTotal = _header.subTotal, 
        .taxes = _taxes.total,
        .total = _header.total
    };
}

Document Document::fromXml(string_view xml) {
    Document doc { };
    
    XmlReader reader { xml };
    while (reader.read()) {
        XmlNode node { reader.current() };
        if (node.nodeType == XmlNodeType::Element) {
            if (node.nodeType == XmlNodeType::Element && node.localName == "Comprobante") {
                doc._header = {
                    .version = node.attributes["Version"],
                    .series = node.attributes["Serie"], 
                    .folium = node.attributes["Folio"],
                    .date = node.attributes["Fecha"], 
                    .seal = node.attributes["Sello"],
                    .paymentType = node.attributes["FormaPago"], 
                    .paymentMethod = node.attributes["MetodoPago"],
                    .certificateNumber = node.attributes["NoCertificado"], 
                    .certificateBody = node.attributes["Certificado"],
                    .subTotal = node.attributes["SubTotal"],
                    .currency = node.attributes["Moneda"],
                    .exchangeRate = node.attributes["TipoCambio"],
                    .total = node.attributes["Total"],
                    .receiptType = node.attributes["TipoDeComprobante"],
                    .placeOfIssue = node.attributes["LugarExpedicion"],
                    .exporting = node.attributes["Exportacion"]
                };
            } else if (node.nodeType == XmlNodeType::Element && node.localName == "Emisor") {
                doc._issuer = {
                    .taxCode = node.attributes["Rfc"], 
                    .name = node.attributes["Nombre"], 
                    .taxRegime = node.attributes["RegimenFiscal"]
                };
            } else if (node.nodeType == XmlNodeType::Element && node.localName == "Receptor") {
                doc._receiver = {
                    .taxCode = node.attributes["Rfc"], 
                    .name = node.attributes["Nombre"], 
                    .address = node.attributes["DomicilioFiscalReceptor"],
                    .taxRegime = node.attributes["RegimenFiscalReceptor"],
                    .usage = node.attributes["UsoCFDI"]
                };
            } else if (node.nodeType == XmlNodeType::Element && node.localName == "Conceptos") {
                while (reader.read()) {
                    node = reader.current();
                    if (node.nodeType == XmlNodeType::Element && node.localName == "Concepto") {
                        doc._concepts.push_back({
                            .code = node.attributes["ClaveProdServ"], 
                            .idNumber = node.attributes["NoIdentificacion"],
                            .quantity = node.attributes["Cantidad"], 
                            .unitCode = node.attributes["ClaveUnidad"],
                            .unitName = node.attributes["Unidad"],
                            .description = node.attributes["Descripcion"],
                            .unitValue = node.attributes["ValorUnitario"],
                            .amount = node.attributes["Importe"],
                            .impObject = node.attributes["ObjetoImp"]
                        });
                    } else if (node.nodeType == XmlNodeType::EndElement && node.localName == "Conceptos"){
                        break;
                    }
                }
            } else if (node.nodeType == XmlNodeType::Element && node.localName == "Impuestos") {
                doc._taxes = {
                    .total = node.attributes["TotalImpuestosTrasladados"]
                };
                while (reader.read()) {
                    node = reader.current();
                    if (node.nodeType == XmlNodeType::Element && node.localName == "Traslado"){
                        doc._taxes.transfers.push_back({
                            .base = node.attributes["Base"],
                            .code = node.attributes["Impuesto"],
                            .factorType = node.attributes["TipoFactor"],
                            .rate = node.attributes["TasaOCuota"],
                            .amount = node.attributes["Importe"]
                        });
                    } else if (node.nodeType == XmlNodeType::EndElement && node.localName == "Impuestos") {
                        break;
                    }
                }
            } else if (node.nodeType == XmlNodeType::Element && node.localName == "Complemento") {
                if (reader.read()) {
                    node = reader.current();
                    if (node.nodeType == XmlNodeType::Element && node.localName == "TimbreFiscalDigital") {
                        doc._complement = {
                            .stamp = {
                                .version = node.attributes["Version"],
                                .uuid = node.attributes["UUID"], 
                                .date = node.attributes["FechaTimbrado"], 
                                .provider = node.attributes["RfcProvCertif"],
                                .seal = node.attributes["SelloCFD"],
                                .satCertificate = node.attributes["NoCertificadoSAT"],
                                .satSeal = node.attributes["SelloSAT"]
                            }
                        };
                    }
                }
            }
        }
    }

    return doc;
}


