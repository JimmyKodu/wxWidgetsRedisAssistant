// Redis Assistant - Main Application Entry Point
#include <wx/wx.h>
#include "RedisFrame.h"

class RedisApp : public wxApp
{
public:
    virtual bool OnInit() override;
};

wxIMPLEMENT_APP(RedisApp);

bool RedisApp::OnInit()
{
    RedisFrame* frame = new RedisFrame("Redis Assistant", wxPoint(50, 50), wxSize(800, 600));
    frame->Show(true);
    return true;
}
