#include "window.hpp"
#include "document.hpp"
#include "directoryscanner.hpp"
#include "output.hpp"
#include <wx/msgdlg.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <fstream>
#include <sstream>
#include <filesystem>
#include <stdexcept>

using std::exception;
using std::ifstream;
using std::string;
using std::vector;
using std::ofstream;
using std::ostringstream;
using cfdi::csv;
using cfdi::json;
using cfdi::xml;
using cfdi::Window;
using cfdi::Document;
using cfdi::DirectoryScanner;
using cfdi::Summary;

Window::Window()
    : wxFrame(nullptr, wxID_ANY, "Blendwerk Procesador de CFDI v0.1.0"), 
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
    menuFile->Append(Commands::Open, "&Abrir...\tCtrl+A", "Escanea un directorio buscando archivos CFDI.");
    menuFile->Append(Commands::Save, "&Guardar...\tCtrl+G", "Exporta el condensado de CFDIs en un archivo CSV.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT, "&Salir"); 
    wxMenu* menuHelp = new wxMenu { };
    menuHelp->Append(wxID_ABOUT, "A&cerca de..."); 
    wxMenuBar* menuBar = new wxMenuBar { };
    menuBar->Append(menuFile, "&Archivo");
    menuBar->Append(menuHelp, "&Ayuda"); 
    SetMenuBar(menuBar);

    Bind(wxEVT_MENU, &Window::onOpen, this, Commands::Open);
    Bind(wxEVT_MENU, &Window::onSave, this, Commands::Save);
    Bind(wxEVT_MENU, &Window::onAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Window::onExit, this, wxID_EXIT);
}

void Window::initStatusBar() {
    CreateStatusBar();
    SetStatusText("Listo para procesar CFDIs");
}

void Window::initToolBar() {

}

void Window::initGrid() {
    _grid = new wxGrid { this, wxID_ANY };    
    _grid->CreateGrid(0, 7); 
    
    _grid->SetColLabelValue(0, "Fecha");
    _grid->SetColLabelValue(1, "Descripción");
    _grid->SetColLabelValue(2, "RFC Emisor");
    _grid->SetColLabelValue(3, "No. Factura");
    _grid->SetColLabelValue(4, "SubTotal");
    _grid->SetColLabelValue(5, "IVA");
    _grid->SetColLabelValue(6, "Total");
    
    _grid->SetColSize(0, 100);  // Fecha
    _grid->SetColSize(1, 200);  // Descripción (wider for longer text)
    _grid->SetColSize(2, 120);  // RFC Emisor
    _grid->SetColSize(3, 120);  // No. Factura
    _grid->SetColSize(4, 100);  // SubTotal
    _grid->SetColSize(5, 80);   // IVA
    _grid->SetColSize(6, 100);  // Total    
    _grid->EnableEditing(false);
    
    // Create sizer and add grid
    wxBoxSizer* sizer = new wxBoxSizer(wxVERTICAL);
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
        _grid->SetCellValue(row, 1, wxString::FromUTF8(summary.description));
        _grid->SetCellValue(row, 2, wxString::FromUTF8(summary.issuerTaxCode));
        _grid->SetCellValue(row, 3, wxString::FromUTF8(summary.invoiceId));
        _grid->SetCellValue(row, 4, wxString::FromUTF8(summary.subTotal));
        _grid->SetCellValue(row, 5, wxString::FromUTF8(summary.taxes));
        _grid->SetCellValue(row, 6, wxString::FromUTF8(summary.total));
    }
    
    _grid->AutoSize();
}

void Window::onExit(wxCommandEvent& e) {
    Close(true);
}

void Window::onAbout(wxCommandEvent& e) {
    wxMessageBox("Blendwerk Procesador de CFDI v0.1.0\n\nAplicación para procesar archivos CFDI y exportar resúmenes.", 
                 "Acerca de", wxOK | wxICON_INFORMATION);
}

void Window::onOpen(wxCommandEvent& e) {
    wxDirDialog dirDialog { this, "Seleccionar directorio con archivos CFDI", "",  wxDD_DEFAULT_STYLE | wxDD_DIR_MUST_EXIST };
    
    if (dirDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    wxString selectedPath { dirDialog.GetPath() };    
    if (!wxDirExists(selectedPath)) {
        wxMessageBox("La ruta seleccionada no es un directorio válido.", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    try {
        DirectoryScanner scanner;
        string pathStr { selectedPath.ToStdString() };
        auto found = scanner.scan(pathStr);
        
        if (found == 0) {
            wxMessageBox("No se encontraron archivos CFDI en el directorio seleccionado.", "Sin Archivos", wxOK | wxICON_WARNING);
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
                    
                    Document doc { Document::fromXml(xml) };
                    Summary summary { doc.summarize() };
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
        wxMessageBox("No hay datos para exportar. Primero escanee un directorio con archivos CFDI.", "Sin Datos", wxOK | wxICON_WARNING);
        return;
    }
    
    wxString wildcard = "Archivos CSV (*.csv)|*.csv|"
                        "Archivos JSON (*.json)|*.json|"
                        "Archivos XML (*.xml)|*.xml";
    
    wxFileDialog saveDialog { this, "Exportar resumen de CFDIs", "", "cfdis_resumen.csv", wildcard, wxFD_SAVE | wxFD_OVERWRITE_PROMPT };    
    if (saveDialog.ShowModal() == wxID_CANCEL) {
        return;
    }
    
    wxString filePath { saveDialog.GetPath() };
    int filterIndex { saveDialog.GetFilterIndex() };
    
    wxMessageBox(wxString::Format("Archivo a exportar:\n%s", filePath), "Archivo Seleccionado", wxOK | wxICON_INFORMATION);
    
    try {
        ofstream file { filePath.ToStdString() };
        if (!file.is_open()) {
            wxMessageBox("No se pudo crear el archivo de exportación.", "Error", wxOK | wxICON_ERROR);
            return;
        }
        
        file << "\xEF\xBB\xBF";        
        switch (filterIndex) {
            case 0: // CSV
                for (const auto& summary : _summaries) {
                    file << csv << "Fecha,Descripción,RFC Emisor,No. Factura,SubTotal,IVA,Total\n";
                    file << csv << summary << "\n";
                }
                break;
                
            case 1: // JSON
                file << json << "[\n";
                for (size_t i = 0; i < _summaries.size(); ++i) {
                    file << json << "  " << _summaries[i];
                    if (i < _summaries.size() - 1) {
                        file << json << ",";
                    }
                    file << json << "\n";
                }
                file << json << "]\n";
                break;
                
            case 2: // XML
                file << xml << "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n<Summaries>\n";
                for (const auto& summary : _summaries) {
                    file << xml << summary << "\n";
                }
                file << xml << "</Summaries>\n";
                break;
        }
        
        file.close();
        
        SetStatusText(wxString::Format("Archivo exportado: %s", filePath));
        
    } catch (const std::exception& ex) {
        wxMessageBox(wxString::Format("Error durante la exportación: %s", ex.what()), "Error", wxOK | wxICON_ERROR);
    }
}

