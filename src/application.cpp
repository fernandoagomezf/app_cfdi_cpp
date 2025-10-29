#include "application.hpp"
#include "window.hpp"

using cfdi::Application;
using cfdi::Window;

// This macro creates the wxWidgets entry point
wxIMPLEMENT_APP(Application);

bool Application::OnInit() {
    Window* window = new Window { };
    window->Show(true);

    return true;
}