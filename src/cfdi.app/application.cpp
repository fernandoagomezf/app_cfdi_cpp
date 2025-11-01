module;

#include <wx/wx.h>

module cfdi.app:application;

import :window;
import :application;

using cfdi::Application;
using cfdi::Window;

// This macro creates the wxWidgets entry point
wxIMPLEMENT_APP(Application);

bool Application::OnInit() {
    Window* window = new Window { };
    window->Show(true);

    return true;
}