#include "aqMainFrame.h"
#include "aqQueryExec.h"
#include "aqDBExplorer.h"
#include "aqQueryExplorer.h"

using namespace aq::gui;

aqMainFrame::aqMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size)
  : 
  wxFrame(nullptr, -1, title, pos, size), 
  logsFrame(nullptr),
  settingsFrame(nullptr), 
  settings(nullptr)
{
  
  //
  // initialize verb builder
  vb = new aq::VerbBuilder;
  aq::verb::VerbFactory::GetInstance().setBuilder(vb);

  // Menu
  wxMenu * menuFile = new wxMenu;
  
  menuFile->Append(ID_Open, _("&Open"));
  menuFile->Append(ID_Connect, _("&Connect"));
  menuFile->Append(ID_Properties, _("&Properties"));
  menuFile->Append(ID_Logs, _("&Logs"));
  menuFile->AppendSeparator();
  menuFile->Append(ID_About, _("&About"));
  menuFile->AppendSeparator();
  menuFile->Append(ID_Quit, _("E&xit"));
  
  menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &aqMainFrame::OnOpen, this, ID_Open);
  menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &aqMainFrame::OnConnect, this, ID_Connect);
  menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &aqMainFrame::OnProperties, this, ID_Properties);
  menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &aqMainFrame::OnLogs, this, ID_Logs);
  menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &aqMainFrame::OnAbout, this, ID_About);
  menuFile->Bind(wxEVT_COMMAND_MENU_SELECTED, &aqMainFrame::OnQuit, this, ID_Quit);

  wxMenuBar *menuBar = new wxMenuBar;
  menuBar->Append(menuFile, _("&File"));

  this->SetMenuBar(menuBar);
  
  // Logs
  this->logsFrame = new aqLogsFrame(this);

  //
  wxBoxSizer * box = new wxBoxSizer(wxHORIZONTAL);
  wxSplitterWindow * splitter1 = new wxSplitterWindow(this, wxID_ANY);
  
  wxPanel * qpan = new wxPanel(splitter1, wxID_ANY);
  wxBoxSizer * b2 = new wxBoxSizer(wxHORIZONTAL);
  wxSplitterWindow * splitter2 = new wxSplitterWindow(qpan, wxID_ANY);
  aqQueryExec * qexe = new aqQueryExec(splitter2);
  aqQueryExplorer * qexp = new aqQueryExplorer(splitter2, "queries.xml", qexe);
  b2->Add(splitter2, 1, wxEXPAND, 0);
  splitter2->SetSashGravity(1);
  splitter2->SetMinimumPaneSize(100);
  splitter2->SplitVertically(qexe, qexp, 2000); // FIXME
  qpan->SetSizer(b2);

  aqDBExplorer * db = new aqDBExplorer(splitter1, this, qexe);
  splitter1->SetSashGravity(0);
  splitter1->SetMinimumPaneSize(200);
  box->Add(splitter1, 1, wxALL | wxEXPAND, 0);
  splitter1->SplitVertically(db, qpan, 10);
  this->SetSizer(box);


  // Status Bar
  this->CreateStatusBar();
  this->SetStatusText( _("no database connection") );

  // Icon
  // this->SetIcon(wxIcon(_T("logo.jpg")));
  
  // this->OnProperties(wxCommandEvent());

  this->Centre();
}

aqMainFrame::~aqMainFrame()
{
  if (vb != nullptr)
    delete vb;
  if (logsFrame != nullptr)
  {
    logsFrame->Hide();
    delete logsFrame;
  }
}

void aqMainFrame::setStatusBar(const std::string& msg)
{
  this->SetStatusText(msg);
}

void aqMainFrame::OnOpen(wxCommandEvent& WXUNUSED(event))
{
}

void aqMainFrame::OnConnect(wxCommandEvent& WXUNUSED(event))
{
}

void aqMainFrame::OnProperties(wxCommandEvent& event)
{
  if (this->settingsFrame == nullptr)
  {
    this->settingsFrame = new wxFrame(nullptr, wxID_ANY, _("AlgoQuest Settings"));
    wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
    this->settings = new aqSettings(this->settingsFrame, "settingsConfig.xml");
    box->Add(this->settings, 1, wxALL | wxEXPAND, 0);
    this->settingsFrame->SetSizer(box);
  }
  this->settingsFrame->Show();
}

void aqMainFrame::OnLogs(wxCommandEvent& WXUNUSED(event))
{
  if (logsFrame != nullptr)
    logsFrame->Show();
}

void aqMainFrame::OnQuit(wxCommandEvent& WXUNUSED(event))
{
  Close(true);
}

void aqMainFrame::OnAbout(wxCommandEvent& WXUNUSED(event))
{
  wxMessageBox(_("AlgoQuest System"), _("About AlgoQuest System"), wxOK | wxICON_INFORMATION, this);
}