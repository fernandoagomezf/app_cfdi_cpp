#ifndef _WINDOW_HPP_
#define _WINDOW_HPP_

#include <wx/wx.h>

namespace cfdi {
    class Window : public wxFrame {
        public:
            Window();

        private:
            enum Commands {
                Scan, Export
            };

            void OnExit(wxCommandEvent& event);
            void OnAbout(wxCommandEvent& event);
            void OnScan(wxCommandEvent& event);
            void OnExport(wxCommandEvent& event);
    };
}

#endif