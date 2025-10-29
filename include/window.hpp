#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <wx/wx.h>
#include <wx/grid.h>
#include <wx/dirdlg.h>
#include <wx/filedlg.h>
#include <vector>
#include "summary.hpp"

namespace cfdi {
    using std::vector;

    class Window final : public wxFrame {
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
            vector<Summary> _summaries;
    };
}

#endif