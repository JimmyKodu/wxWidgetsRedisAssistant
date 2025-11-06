#ifndef REDIS_FRAME_H
#define REDIS_FRAME_H

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <memory>
#include "RedisClient.h"

class RedisFrame : public wxFrame
{
public:
    RedisFrame(const wxString& title, const wxPoint& pos, const wxSize& size);

private:
    void OnConnect(wxCommandEvent& event);
    void OnDisconnect(wxCommandEvent& event);
    void OnGet(wxCommandEvent& event);
    void OnSet(wxCommandEvent& event);
    void OnDel(wxCommandEvent& event);
    void OnKeys(wxCommandEvent& event);
    void OnQuit(wxCommandEvent& event);
    void OnAbout(wxCommandEvent& event);
    
    void UpdateStatus(const wxString& message);
    
    // UI Components
    wxTextCtrl* m_hostCtrl;
    wxTextCtrl* m_portCtrl;
    wxTextCtrl* m_passwordCtrl;
    wxTextCtrl* m_keyCtrl;
    wxTextCtrl* m_valueCtrl;
    wxListCtrl* m_keysList;
    wxStatusBar* m_statusBar;
    
    // Redis client
    std::unique_ptr<RedisClient> m_redisClient;
    
    enum
    {
        ID_Connect = 1,
        ID_Disconnect,
        ID_Get,
        ID_Set,
        ID_Del,
        ID_Keys
    };
    
    wxDECLARE_EVENT_TABLE();
};

#endif // REDIS_FRAME_H
