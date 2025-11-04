module;

#include <wx/wx.h>
#include <wx/msgdlg.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <wx/toolbar.h>
#include <wx/artprov.h>
#include <wx/grid.h>

module cfdi.app:window;

import std;
import cfdi.doc;
import :window;

using std::exception;
using std::ifstream;
using std::string;
using std::ofstream;
using std::ostringstream;
using cfdi::CFDIDocument;
using cfdi::CFDIScanner;
using cfdi::CFDISummary;
using cfdi::CFDIWritter;
using cfdi::CFDIScanner;
using cfdi::Window;

Window::Window()
    : wxFrame(nullptr, wxID_ANY, "Blendwerk Procesador de CFDI v0.1.1"), 
      _grid { nullptr }
{
    initMenu();
    initStatusBar();
    initToolBar();
    initGrid();

    SetSize({ 1200, 700 });
    Center();
}

void Window::initMenu() {
    wxMenu* menuFile = new wxMenu { };
    menuFile->Append(Commands::Open, wxString::FromUTF8("&Abrir...\tCtrl+A"), wxString::FromUTF8("Escanea un directorio buscando archivos CFDI."));
    menuFile->Append(Commands::Save, wxString::FromUTF8("&Guardar...\tCtrl+G"), wxString::FromUTF8("Exporta el condensado de CFDIs en un archivo CSV."));
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, wxString::FromUTF8("&Salir")); 
    wxMenu* menuHelp = new wxMenu { };
    menuHelp->Append(wxID_ABOUT, wxString::FromUTF8("A&cerca de...")); 
    wxMenuBar* menuBar = new wxMenuBar { };
    menuBar->Append(menuFile, wxString::FromUTF8("&Archivo"));
    menuBar->Append(menuHelp, wxString::FromUTF8("&Ayuda")); 
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Window::onOpen, this, Commands::Open);
    Bind(wxEVT_MENU, &Window::onSave, this, Commands::Save);
    Bind(wxEVT_MENU, &Window::onAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Window::onExit, this, wxID_EXIT);
}

void Window::initStatusBar() {
    CreateStatusBar();
    SetStatusText(wxString::FromUTF8("Listo para procesar CFDIs"));
}

void Window::initToolBar() {
    wxToolBar* toolbar { CreateToolBar(wxTB_HORIZONTAL | wxTB_TEXT | wxTB_FLAT) };
    
    toolbar->SetToolBitmapSize(wxSize(24, 24));    
    toolbar->AddTool(Commands::Open, 
                    wxString::FromUTF8("Abrir"), 
                    wxArtProvider::GetBitmap(wxART_FILE_OPEN, wxART_TOOLBAR, wxSize(24, 24)),
                    wxString::FromUTF8("Escanear directorio con archivos CFDI"));
    toolbar->AddSeparator();    
    toolbar->AddTool(Commands::Save, 
                    wxString::FromUTF8("Guardar"), 
                    wxArtProvider::GetBitmap(wxART_FILE_SAVE, wxART_TOOLBAR, wxSize(24, 24)),
                    wxString::FromUTF8("Exportar resumen de CFDIs"));    
    toolbar->Realize();
    
    Bind(wxEVT_TOOL, &Window::onOpen, this, Commands::Open);
    Bind(wxEVT_TOOL, &Window::onSave, this, Commands::Save);
}

void Window::initGrid() {
    _grid = new wxGrid { this, wxID_ANY };    
    _grid->CreateGrid(0, 10); 
    
    _grid->SetColLabelValue(0, wxString::FromUTF8("Fecha"));
    _grid->SetColLabelValue(1, wxString::FromUTF8("Razón Social"));
    _grid->SetColLabelValue(2, wxString::FromUTF8("RFC Emisor"));
    _grid->SetColLabelValue(3, wxString::FromUTF8("Factura"));
    _grid->SetColLabelValue(4, wxString::FromUTF8("Método Pago"));
    _grid->SetColLabelValue(5, wxString::FromUTF8("Tipo Pago"));
    _grid->SetColLabelValue(6, wxString::FromUTF8("Expedición"));
    _grid->SetColLabelValue(7, wxString::FromUTF8("SubTotal"));
    _grid->SetColLabelValue(8, wxString::FromUTF8("IVA"));
    _grid->SetColLabelValue(9, wxString::FromUTF8("Total"));
    
    _grid->SetColSize(0, 120);
    _grid->SetColSize(1, 250);
    _grid->SetColSize(2, 150);
    _grid->SetColSize(3, 250);
    _grid->SetColSize(4, 100);
    _grid->SetColSize(5, 100);
    _grid->SetColSize(6, 100);
    _grid->SetColSize(7, 100);
    _grid->SetColSize(8, 100);
    _grid->SetColSize(9, 100);
    _grid->EnableEditing(false);
    
    wxBoxSizer* sizer = new wxBoxSizer { wxVERTICAL };
    sizer->Add(_grid, 1, wxEXPAND | wxALL, 5);
    SetSizer(sizer);
}

void Window::populateGrid() {
    if (_grid->GetNumberRows() > 0) {
        _grid->DeleteRows(0, _grid->GetNumberRows());
    }
    
    for (const auto& summary : _summaries) {
        _grid->AppendRows(1);
        int row { _grid->GetNumberRows() - 1 };
        
        _grid->SetCellValue(row, 0, wxString::FromUTF8(summary.date));
        _grid->SetCellValue(row, 1, wxString::FromUTF8(summary.issuerName));
        _grid->SetCellValue(row, 2, wxString::FromUTF8(summary.issuerTaxCode));
        _grid->SetCellValue(row, 3, wxString::FromUTF8(summary.invoiceId));
        _grid->SetCellValue(row, 4, wxString::FromUTF8(summary.paymentMethod));
        _grid->SetCellValue(row, 5, wxString::FromUTF8(summary.paymentType));
        _grid->SetCellValue(row, 6, wxString::FromUTF8(summary.placeOfIssue));
        _grid->SetCellValue(row, 7, wxString::FromUTF8(summary.subTotal));
        _grid->SetCellValue(row, 8, wxString::FromUTF8(summary.taxes));
        _grid->SetCellValue(row, 9, wxString::FromUTF8(summary.total));
    }
}

void Window::onExit(wxCommandEvent& e) {
    Close(true);
}

void Window::onAbout(wxCommandEvent& e) {
    wxMessageBox(wxString::FromUTF8("Blendwerk Procesador de CFDI v0.1.1\n\nAplicación para procesar archivos CFDI y exportar resúmenes."), 
                 wxString::FromUTF8("Acerca de"), wxOK | wxICON_INFORMATION);
}

void Window::onOpen(wxCommandEvent& e) {
    wxDirDialog dirDialog { this, wxString::FromUTF8("Seleccionar directorio con archivos CFDI"), "",  wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST };
    
    if (dirDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    wxString selectedPath { dirDialog.GetPath() };    
    if (!wxDirExists(selectedPath)) {
        wxMessageBox(wxString::FromUTF8("La ruta seleccionada no es un directorio válido."), 
                    wxString::FromUTF8("Error"), wxOK | wxICON_ERROR);
        return;
    }
    
    try {
        CFDIScanner scanner;
        string pathStr { selectedPath.ToStdString() };
        auto found = scanner.scan(pathStr);
        
        if (found == 0) {
            wxMessageBox(wxString::FromUTF8("No se encontraron archivos CFDI en el directorio seleccionado."), 
                        wxString::FromUTF8("Sin Archivos"), wxOK | wxICON_WARNING);
            return;
        }
        
        _summaries.clear();
                
        int processed { 0 };
        int errors = { 0 };        
        for (auto it = scanner.begin(); it != scanner.end(); ++it) {
            try {
                ifstream file { *it };
                if (file.is_open()) {
                    ostringstream ss { };
                    ss << file.rdbuf();
                    string xml { ss.str() };
                    
                    CFDIDocument doc { CFDIDocument::fromXml(xml) };
                    CFDISummary summary { doc.summarize() };
                    _summaries.push_back(summary);
                    ++processed;
                } else {
                    ++errors;
                }
            } catch (const exception& ex) {
                ++errors;
            }
        }
        
        populateGrid();                
        SetStatusText(wxString::Format("Procesados: %d archivos, Errores: %d", processed, errors));
        
    } catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("Error durante el procesamiento: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

void Window::onSave(wxCommandEvent& e) {
    if (_summaries.empty()) {
        wxMessageBox(wxString::FromUTF8("No hay datos para exportar. Primero escanee un directorio con archivos CFDI."), 
                    wxString::FromUTF8("Sin Datos"), wxOK | wxICON_WARNING);
        return;
    }
    
    wxString wildcard = "Archivos CSV (*.csv)|*.csv|"
                        "Archivos JSON (*.json)|*.json|"
                        "Archivos XML (*.xml)|*.xml";
    
    wxFileDialog saveDialog { this, wxString::FromUTF8("Exportar resumen de CFDIs"), "", "cfdis_resumen.csv", wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT };    
    if (saveDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    wxString filePath { saveDialog.GetPath() };
    int filterIndex { saveDialog.GetFilterIndex() };
        
    try {
        ofstream file { filePath.ToStdString() };
        if (!file.is_open()) {
            wxMessageBox(wxString::FromUTF8("No se pudo crear el archivo de exportación."), 
                        wxString::FromUTF8("Error"), wxOK | wxICON_ERROR);
            return;
        }

        CFDIWritter writter;
        
        file << "\xEF\xBB\xBF"; // necesario para que excel abra el archivo con UTF-8
        switch (filterIndex) {
            case 0: // csv
                file << "Fecha,Razón Social,Descripción,RFC Emisor,Factura,Método Pago,Forma Pago,Expedición,SubTotal,IVA,Total\n";
                for (const auto& summary : _summaries) {
                    file << writter.writeCsv(summary) << "\n";
                }
                break;
                
            case 1: // json
                file << "[\n";
                for (const auto& summary : _summaries) {
                    file << writter.writeJson(summary) << ", \n";
                }
                file << "]\n";
                break;
                
            case 2: // xml
                file << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Summaries>\n";
                for (const auto& summary : _summaries) {
                    file << writter.writeXml(summary) << "\n";
                }
                file << "</Summaries>\n";
                break;
        }
        
        file.close();
        
        SetStatusText(wxString::Format("Archivo exportado: %s", filePath));
        
    } catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("Error durante la exportación: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

