#include "aqQueryExec.h"
#include "aqResultHandler.h"
#include "aqQueryAnalyzer.h"
#include <aq/Base.h>
#include <aq/QueryResolver.h>
#include <aq/AQEngine.h>
#include <sstream>

namespace aq {
namespace gui {

  aqQueryExec::aqQueryExec(wxWindow * parent)
    : wxPanel(parent)
  {
    wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
    
    this->splitter = new wxSplitterWindow(this, wxID_ANY);
    this->splitter->SetSashGravity(0.5);
    this->splitter->SetMinimumPaneSize(20); // Smalest size the

    this->pan1 = new wxPanel(this->splitter, wxID_ANY);
    wxBoxSizer * bs1 = new wxBoxSizer(wxVERTICAL);
    this->sqlQuery = new wxTextCtrl(pan1, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
    bs1->Add(this->sqlQuery, 1, wxALL | wxEXPAND, 0);
    pan1->SetSizer(bs1);

    this->pan2 = new wxPanel(this->splitter, wxID_ANY);
    wxBoxSizer * bs2 = new wxBoxSizer(wxVERTICAL);
    this->result = new wxListView(pan2, wxID_ANY);
    bs2->Add(this->result, 1, wxALL | wxEXPAND, 0);
    pan2->SetSizer(bs2);

    this->splitter->SplitHorizontally(this->pan1, this->pan2, this->GetSize().GetHeight() / 2); // FIXME : should be the panel size (and not the parent)
    this->splitter->Unsplit();
    
    wxButton * analyzeButton = new wxButton(this, ID_Analyze, _("analyze"));
    wxButton * runButton = new wxButton(this, ID_Ok, _("run"));

    box->Add(splitter, 1, wxEXPAND | wxALL, 10);
    box->Add(analyzeButton, 0, wxRIGHT | wxBOTTOM | wxALIGN_RIGHT, 10);
    box->Add(runButton, 0, wxRIGHT | wxBOTTOM | wxALIGN_RIGHT, 10);

    // wxStreamToTextRedirector * redirect = new wxStreamToTextRedirector(this->sqlQuery);

    char * s = ::getenv("AQ_HOME");
    if (s != nullptr)
    {
      this->root = s;
    }

    this->Bind(wxEVT_BUTTON, &aqQueryExec::OnAnalyze, this, ID_Analyze);
    this->Bind(wxEVT_BUTTON, &aqQueryExec::OnRun, this, ID_Ok);

    this->SetSizer(box);
    box->SetSizeHints(this);
  }
  
  void aqQueryExec::OnAnalyze(wxCommandEvent& WXUNUSED(e))
  {
    wxFrame * frame = new wxFrame(this, wxID_ANY, _("Query Analyzer"));
    wxBoxSizer * b = new wxBoxSizer(wxVERTICAL);
    aqQueryAnalyzer * qa = new aqQueryAnalyzer(frame, this->sqlQuery->GetValue().ToStdString());
    b->Add(qa, 1, wxALL | wxEXPAND, 0);
    frame->SetSizer(b);
    frame->SetSize(wxSize(1200, 600));
    frame->Centre();
    frame->Show();
  }

  void aqQueryExec::OnRun(wxCommandEvent& WXUNUSED(e))
  {
    this->result->ClearAll();

    std::string query(this->sqlQuery->GetValue());
    
    aq::Base            bd;
    aq::Settings        settings;  
    aq::AQEngine_Intf * aqEngine = nullptr;
    const std::string   ident; 
    bool                keepFiles = false;
    bool                force = false;

    if ((this->root != "") && (this->database != ""))
    {
      settings.initPath("E:/AQ_DB/" + this->database); // TODO
      aq::Base::load(settings.dbDesc, bd);
      aqEngine = new aq::AQEngineSystem(bd, settings);

      boost::shared_ptr<aq::RowWritter_Intf> resultHandler(new aqResultHandler(*this->result));
      aq::QueryResolver::prepareQuery(query, ident, settings, force);
      aq::QueryResolver::processQuery(query, settings, bd, aqEngine, resultHandler, keepFiles);

      delete aqEngine;

      this->splitter->SplitHorizontally(this->pan1, this->pan2, this->GetSize().GetHeight() / 2); // FIXME : should be the panel size (and not the parent)
    }
  }

}
}
