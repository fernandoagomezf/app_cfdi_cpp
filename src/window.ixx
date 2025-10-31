export module cfdi.app:window;

import <wx/wx.h>;
import <wx/grid.h>;

import std;
import cfdi.doc;

namespace cfdi {
    using std::vector;

    export class Window final : public wxFrame {
        public:
            Window();

        private:
            enum Commands {
                Open, Save
            };

            void onExit(wxCommandEvent& event);
            void onAbout(wxCommandEvent& event);
            void onOpen(wxCommandEvent& event);
            void onSave(wxCommandEvent& event);
            
            void initGrid();
            void initMenu();
            void initStatusBar();
            void initToolBar();
            void populateGrid();
            
            wxGrid* _grid;
            vector<CFDISummary> _summaries;
    };
}