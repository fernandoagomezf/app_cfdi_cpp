module cfdi.doc:cfdidocument;

import cfdi.util;
import cfdi.xml;
import :cfdicomplement;
import :cfdiconcept;
import :cfdiissuer;
import :cfdireceiver;
import :cfditax;
import :cfdisummary;
import :cfdidocument;
import :cfdiwritter;

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
using cfdi::CFDISummary;
using cfdi::CFDIDocument;
using cfdi::CFDIWritter;
using cfdi::XmlReader;
using cfdi::join;

const CFDIHeader& CFDIDocument::header() const {
    return _header;
}

const CFDIIssuer& CFDIDocument::issuer() const {
    return _issuer;
}

const CFDIReceiver& CFDIDocument::receiver() const {
    return _receiver;
}

const list<CFDIConcept>& CFDIDocument::concepts() const {
    return _concepts;
}

const CFDITax& CFDIDocument::taxes() const {
    return _taxes;
}

const CFDIComplement& CFDIDocument::complement() const {
    return _complement;
}

CFDISummary CFDIDocument::summarize() const {
    using namespace std::literals;
    return  { 
        .date = _header.date, 
        .description = join(_concepts, ", ", [](const CFDIConcept& c) { return c.description; }),
        .issuerTaxCode = _issuer.taxCode, 
        .invoiceId = _complement.stamp.uuid, 
        .paymentMethod = _header.paymentMethod,
        .placeOfIssue = _header.placeOfIssue,
        .subTotal = _header.subTotal, 
        .taxes = _taxes.isExent() ? "N/A"s : _taxes.total,
        .total = _header.total
    };
}

CFDIDocument CFDIDocument::fromXml(string_view xml) {
    CFDIDocument doc { };
    
    XmlReader reader { xml };
    while (reader.read()) {
        XmlNode node { reader.current() };
        if (node.nodeType == XmlNodeType::Element) {
            if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Comprobante") {
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
            } else if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Emisor") {
                doc._issuer = {
                    .taxCode = node.attributes["Rfc"], 
                    .name = node.attributes["Nombre"], 
                    .taxRegime = node.attributes["RegimenFiscal"]
                };
            } else if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Receptor") {
                doc._receiver = {
                    .taxCode = node.attributes["Rfc"], 
                    .name = node.attributes["Nombre"], 
                    .address = node.attributes["DomicilioFiscalReceptor"],
                    .taxRegime = node.attributes["RegimenFiscalReceptor"],
                    .usage = node.attributes["UsoCFDI"]
                };
            } else if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Conceptos") {
                while (reader.read()) {
                    node = reader.current();
                    if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Concepto") {
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
                    } else if (node.nodeType == XmlNodeType::EndElement && node.name == "cfdi:Conceptos"){
                        break;
                    }
                }
            } else if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Impuestos") {
                doc._taxes = {
                    .total = node.attributes["TotalImpuestosTrasladados"]
                };
                while (reader.read()) {
                    node = reader.current();
                    if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Traslado"){
                        doc._taxes.transfers.push_back({
                            .base = node.attributes["Base"],
                            .code = node.attributes["Impuesto"],
                            .factorType = node.attributes["TipoFactor"],
                            .rate = node.attributes["TasaOCuota"],
                            .amount = node.attributes["Importe"]
                        });
                    } else if (node.nodeType == XmlNodeType::EndElement && node.name == "cfdi:Impuestos") {
                        break;
                    }
                }
            } else if (node.nodeType == XmlNodeType::Element && node.name == "cfdi:Complemento") {
                if (reader.read()) {
                    node = reader.current();
                    if (node.nodeType == XmlNodeType::Element && node.name == "tfd:TimbreFiscalDigital") {
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

