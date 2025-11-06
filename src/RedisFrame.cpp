#include "RedisFrame.h"
#include <wx/stattext.h>
#include <wx/sizer.h>

wxBEGIN_EVENT_TABLE(RedisFrame, wxFrame)
    EVT_BUTTON(ID_Connect, RedisFrame::OnConnect)
    EVT_BUTTON(ID_Disconnect, RedisFrame::OnDisconnect)
    EVT_BUTTON(ID_Get, RedisFrame::OnGet)
    EVT_BUTTON(ID_Set, RedisFrame::OnSet)
    EVT_BUTTON(ID_Del, RedisFrame::OnDel)
    EVT_BUTTON(ID_Keys, RedisFrame::OnKeys)
    EVT_MENU(wxID_EXIT, RedisFrame::OnQuit)
    EVT_MENU(wxID_ABOUT, RedisFrame::OnAbout)
wxEND_EVENT_TABLE()

RedisFrame::RedisFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
    : wxFrame(nullptr, wxID_ANY, title, pos, size)
{
    // Create menu bar
    wxMenu* menuFile = new wxMenu;
    menuFile->Append(wxID_EXIT);
    
    wxMenu* menuHelp = new wxMenu;
    menuHelp->Append(wxID_ABOUT);
    
    wxMenuBar* menuBar = new wxMenuBar;
    menuBar->Append(menuFile, "&File");
    menuBar->Append(menuHelp, "&Help");
    SetMenuBar(menuBar);
    
    // Create status bar
    m_statusBar = CreateStatusBar();
    UpdateStatus("Ready");
    
    // Create main panel
    wxPanel* panel = new wxPanel(this);
    wxBoxSizer* mainSizer = new wxBoxSizer(wxVERTICAL);
    
    // Connection section
    wxStaticBoxSizer* connSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Connection");
    wxFlexGridSizer* connGrid = new wxFlexGridSizer(3, 2, 5, 5);
    connGrid->AddGrowableCol(1);
    
    connGrid->Add(new wxStaticText(panel, wxID_ANY, "Host:"), 0, wxALIGN_CENTER_VERTICAL);
    m_hostCtrl = new wxTextCtrl(panel, wxID_ANY, "127.0.0.1");
    connGrid->Add(m_hostCtrl, 1, wxEXPAND);
    
    connGrid->Add(new wxStaticText(panel, wxID_ANY, "Port:"), 0, wxALIGN_CENTER_VERTICAL);
    m_portCtrl = new wxTextCtrl(panel, wxID_ANY, "6379");
    connGrid->Add(m_portCtrl, 1, wxEXPAND);
    
    connGrid->Add(new wxStaticText(panel, wxID_ANY, "Password:"), 0, wxALIGN_CENTER_VERTICAL);
    m_passwordCtrl = new wxTextCtrl(panel, wxID_ANY, "", wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
    connGrid->Add(m_passwordCtrl, 1, wxEXPAND);
    
    connSizer->Add(connGrid, 0, wxEXPAND | wxALL, 5);
    
    wxBoxSizer* connBtnSizer = new wxBoxSizer(wxHORIZONTAL);
    connBtnSizer->Add(new wxButton(panel, ID_Connect, "Connect"), 0, wxRIGHT, 5);
    connBtnSizer->Add(new wxButton(panel, ID_Disconnect, "Disconnect"), 0);
    connSizer->Add(connBtnSizer, 0, wxALL, 5);
    
    mainSizer->Add(connSizer, 0, wxEXPAND | wxALL, 5);
    
    // Operations section
    wxStaticBoxSizer* opsSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Operations");
    wxFlexGridSizer* opsGrid = new wxFlexGridSizer(2, 2, 5, 5);
    opsGrid->AddGrowableCol(1);
    
    opsGrid->Add(new wxStaticText(panel, wxID_ANY, "Key:"), 0, wxALIGN_CENTER_VERTICAL);
    m_keyCtrl = new wxTextCtrl(panel, wxID_ANY);
    opsGrid->Add(m_keyCtrl, 1, wxEXPAND);
    
    opsGrid->Add(new wxStaticText(panel, wxID_ANY, "Value:"), 0, wxALIGN_CENTER_VERTICAL);
    m_valueCtrl = new wxTextCtrl(panel, wxID_ANY);
    opsGrid->Add(m_valueCtrl, 1, wxEXPAND);
    
    opsSizer->Add(opsGrid, 0, wxEXPAND | wxALL, 5);
    
    wxBoxSizer* opsBtnSizer = new wxBoxSizer(wxHORIZONTAL);
    opsBtnSizer->Add(new wxButton(panel, ID_Get, "GET"), 0, wxRIGHT, 5);
    opsBtnSizer->Add(new wxButton(panel, ID_Set, "SET"), 0, wxRIGHT, 5);
    opsBtnSizer->Add(new wxButton(panel, ID_Del, "DEL"), 0, wxRIGHT, 5);
    opsBtnSizer->Add(new wxButton(panel, ID_Keys, "KEYS *"), 0);
    opsSizer->Add(opsBtnSizer, 0, wxALL, 5);
    
    mainSizer->Add(opsSizer, 0, wxEXPAND | wxALL, 5);
    
    // Keys list
    wxStaticBoxSizer* keysSizer = new wxStaticBoxSizer(wxVERTICAL, panel, "Keys");
    m_keysList = new wxListCtrl(panel, wxID_ANY, wxDefaultPosition, wxDefaultSize, 
                                 wxLC_REPORT | wxLC_SINGLE_SEL);
    m_keysList->AppendColumn("Key", wxLIST_FORMAT_LEFT, 300);
    m_keysList->AppendColumn("Type", wxLIST_FORMAT_LEFT, 100);
    keysSizer->Add(m_keysList, 1, wxEXPAND | wxALL, 5);
    
    mainSizer->Add(keysSizer, 1, wxEXPAND | wxALL, 5);
    
    panel->SetSizer(mainSizer);
    
    // Initialize Redis client
    m_redisClient = std::make_unique<RedisClient>();
}

void RedisFrame::OnConnect(wxCommandEvent& event)
{
    wxString host = m_hostCtrl->GetValue();
    wxString port = m_portCtrl->GetValue();
    wxString password = m_passwordCtrl->GetValue();
    
    long portNum;
    if (!port.ToLong(&portNum))
    {
        wxMessageBox("Invalid port number", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (m_redisClient->Connect(host.ToStdString(), static_cast<int>(portNum), 
                                password.ToStdString()))
    {
        UpdateStatus("Connected to " + host + ":" + port);
        wxMessageBox("Connected successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
    else
    {
        UpdateStatus("Connection failed");
        std::string error = m_redisClient->GetLastError();
        wxString errorMsg = "Failed to connect to Redis server";
        if (!error.empty())
        {
            errorMsg += ":\n" + wxString::FromUTF8(error);
        }
        wxMessageBox(errorMsg, "Error", wxOK | wxICON_ERROR);
    }
}

void RedisFrame::OnDisconnect(wxCommandEvent& event)
{
    m_redisClient->Disconnect();
    UpdateStatus("Disconnected");
    wxMessageBox("Disconnected from Redis server", "Info", wxOK | wxICON_INFORMATION);
}

void RedisFrame::OnGet(wxCommandEvent& event)
{
    if (!m_redisClient->IsConnected())
    {
        wxMessageBox("Not connected to Redis", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    wxString key = m_keyCtrl->GetValue();
    if (key.IsEmpty())
    {
        wxMessageBox("Please enter a key", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    std::string value;
    if (m_redisClient->Get(key.ToStdString(), value))
    {
        m_valueCtrl->SetValue(wxString::FromUTF8(value));
        UpdateStatus("GET " + key + " succeeded");
    }
    else
    {
        UpdateStatus("GET " + key + " failed");
        wxMessageBox("Key not found or error occurred", "Error", wxOK | wxICON_ERROR);
    }
}

void RedisFrame::OnSet(wxCommandEvent& event)
{
    if (!m_redisClient->IsConnected())
    {
        wxMessageBox("Not connected to Redis", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    wxString key = m_keyCtrl->GetValue();
    wxString value = m_valueCtrl->GetValue();
    
    if (key.IsEmpty())
    {
        wxMessageBox("Please enter a key", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (m_redisClient->Set(key.ToStdString(), value.ToStdString()))
    {
        UpdateStatus("SET " + key + " succeeded");
        wxMessageBox("Key set successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
    else
    {
        UpdateStatus("SET " + key + " failed");
        wxMessageBox("Failed to set key", "Error", wxOK | wxICON_ERROR);
    }
}

void RedisFrame::OnDel(wxCommandEvent& event)
{
    if (!m_redisClient->IsConnected())
    {
        wxMessageBox("Not connected to Redis", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    wxString key = m_keyCtrl->GetValue();
    if (key.IsEmpty())
    {
        wxMessageBox("Please enter a key", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    if (m_redisClient->Del(key.ToStdString()))
    {
        UpdateStatus("DEL " + key + " succeeded");
        wxMessageBox("Key deleted successfully!", "Success", wxOK | wxICON_INFORMATION);
    }
    else
    {
        UpdateStatus("DEL " + key + " failed");
        wxMessageBox("Failed to delete key", "Error", wxOK | wxICON_ERROR);
    }
}

void RedisFrame::OnKeys(wxCommandEvent& event)
{
    if (!m_redisClient->IsConnected())
    {
        wxMessageBox("Not connected to Redis", "Error", wxOK | wxICON_ERROR);
        return;
    }
    
    std::vector<std::string> keys;
    if (m_redisClient->Keys("*", keys))
    {
        m_keysList->DeleteAllItems();
        for (size_t i = 0; i < keys.size(); ++i)
        {
            long index = m_keysList->InsertItem(i, wxString::FromUTF8(keys[i]));
            std::string keyType;
            if (m_redisClient->GetType(keys[i], keyType))
            {
                m_keysList->SetItem(index, 1, wxString::FromUTF8(keyType));
            }
            else
            {
                m_keysList->SetItem(index, 1, "unknown");
            }
        }
        UpdateStatus(wxString::Format("Found %zu keys", keys.size()));
    }
    else
    {
        UpdateStatus("KEYS command failed");
        wxMessageBox("Failed to retrieve keys", "Error", wxOK | wxICON_ERROR);
    }
}

void RedisFrame::OnQuit(wxCommandEvent& event)
{
    Close(true);
}

void RedisFrame::OnAbout(wxCommandEvent& event)
{
    wxMessageBox("Redis Assistant v1.0.0\nA wxWidgets-based Redis GUI client\n\nSupports basic Redis operations.",
                 "About Redis Assistant", wxOK | wxICON_INFORMATION);
}

void RedisFrame::UpdateStatus(const wxString& message)
{
    m_statusBar->SetStatusText(message);
}
