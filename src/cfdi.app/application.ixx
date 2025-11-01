export module cfdi.app:application;

import <wx/wx.h>;

namespace cfdi {
    export class Application : public wxApp {
        public:
            bool OnInit() override;
    };
}