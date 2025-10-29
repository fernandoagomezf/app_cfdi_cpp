#include "window.hpp"
#include <wx/grid.h>

using cfdi::Window;

Window::Window()
    : wxFrame(nullptr, wxID_ANY, "Blendwerk Procesador de CFDI v0.1.0") 
{
    wxMenu* menuFile = new wxMenu { };
    menuFile->Append(Commands::Scan, "E&scanear...\tCtrl+A", "Escanea un directorio buscando archivos CFDI.");
    menuFile->Append(Commands::Export, "&Exportar...\tCtrl+E", "Exporta el condensado de CFDIs en un archivo CSV.");
    menuFile->AppendSeparator();
    menuFile->Append(wxID_EXIT); 
    wxMenu* menuHelp = new wxMenu { };
    menuHelp->Append(wxID_ABOUT); 
    wxMenuBar* menuBar = new wxMenuBar { };
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help"); 
    SetMenuBar(menuBar);
 
    CreateStatusBar();
    SetStatusText("Welcome to wxWidgets!");

    Bind(wxEVT_MENU, &Window::OnScan, this, Commands::Scan);
    Bind(wxEVT_MENU, &Window::OnExport, this, Commands::Export);
    Bind(wxEVT_MENU, &Window::OnAbout, this, wxID_ABOUT);
    Bind(wxEVT_MENU, &Window::OnExit, this, wxID_EXIT);

    wxGrid* grid = new wxGrid { this, -1, wxPoint { 0, 0 }, wxSize { 400, 300 } }; 
    grid->CreateGrid( 100, 10 );    
    grid->SetRowSize( 0, 60 );
    grid->SetColSize( 0, 120 );
    grid->SetCellValue( 0, 0, "wxGrid is good" );
    grid->SetCellValue( 0, 3, "This is read->only" );
    grid->SetReadOnly( 0, 3 );
    grid->SetCellValue(3, 3, "green on grey");
    grid->SetCellTextColour(3, 3, *wxGREEN);
    grid->SetCellBackgroundColour(3, 3, *wxLIGHT_GREY);    
    grid->SetColFormatFloat(5, 6, 2);
    grid->SetCellValue(0, 6, "3.1415");

    Center();
    SetSize({ 800, 600 });
}

void Window::OnExit(wxCommandEvent& e) {
    Close(true);
}

void Window::OnAbout(wxCommandEvent& e) {
    wxMessageBox("This is a wxWidgets Hello World example", "About Hello World", wxOK | wxICON_INFORMATION);
}

void Window::OnScan(wxCommandEvent& e) {
    
}

void Window::OnExport(wxCommandEvent& e) {

}

