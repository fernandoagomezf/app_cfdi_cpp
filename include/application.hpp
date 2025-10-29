#ifndef _APPLICATION_HPP_
#define _APPLICATION_HPP_

#include <wx/wx.h>

namespace cfdi {
    class Application : public wxApp {
        public:
            bool OnInit() override;
    };
}

#endif