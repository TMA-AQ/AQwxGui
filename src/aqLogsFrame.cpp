#include "aqLogsFrame.h"

using namespace aq::gui;

aqLogsFrame::aqLogsFrame(wxWindow * parent)
  : wxFrame(parent, wxID_ANY, _("AlgoQuest Logs"))
{
  this->logs = new wxTextCtrl(this, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH, wxDefaultValidator, wxTextCtrlNameStr); 
  this->redirect = new wxStreamToTextRedirector(this->logs);

  this->Bind(wxEVT_CLOSE_WINDOW, &aqLogsFrame::OnClose, this, wxID_ANY);
}

void aqLogsFrame::OnClose(wxCloseEvent& WXUNUSED(event))
{
  this->Hide();
}