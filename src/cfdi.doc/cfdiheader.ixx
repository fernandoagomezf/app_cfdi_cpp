export module cfdi.doc:cfdiheader;

import std;

namespace cfdi {
    using std::string;
    using std::list;

    export struct CFDIHeader {          // cfdi:Comprobante
        string version;                 // cfdi:Comprobante - Version
        string series;                  // cfdi:Comprobante - Serie
        string folium;                  // cfdi:Comprobante - Folio
        string date;                    // cfdi:Comprobante - Fecha
        string seal;                    // cfdi:Comprobante - Sello
        string paymentType;             // cfdi:Comprobante - FormaPago
        string paymentMethod;           // cfdi:Comprobante - MetodoPago
        string certificateNumber;       // cfdi:Comprobante - NoCertificado
        string certificateBody;         // cfdi:Comprobante - Certificado
        string subTotal;                // cfdi:Comprobante - SubTotal
        string currency;                // cfdi:Comprobante - Moneda
        string exchangeRate;            // cfdi:Comprobante - TipoCambio
        string total;                   // cfdi:Comprobante - Total
        string receiptType;             // cfdi:Comprobante - TipoDeComprobante
        string placeOfIssue;            // cfdi:Comprobante - LugarExpedicion
        string exporting;               // cfdi:Comprobante - Exportacion

        string shortDate() const;
    };
}