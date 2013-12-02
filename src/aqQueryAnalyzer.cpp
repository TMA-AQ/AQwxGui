
#include <aq/AQLQuery.h>

#include "aqQueryAnalyzer.h"
#include <aq/TreeUtilities.h>
#include <aq/SQLPrefix.h>
#include <aq/AQLParser.h>
#include <iostream>

using namespace aq::gui;

aqQueryAnalyzer::aqQueryAnalyzer(wxWindow * parent, const std::string& query, aq::core::SelectStatement::output_t type)
  : wxPanel(parent), tree(nullptr)
{

  wxBoxSizer * b = new wxBoxSizer(wxVERTICAL);
  wxSplitterWindow * splitter = new wxSplitterWindow(this, wxID_ANY);
  wxTextCtrl * sqlQuery = new wxTextCtrl(splitter, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  wxTextCtrl * aqlQuery = new wxTextCtrl(splitter, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE);
  splitter->SetSashGravity(0.5);

  if (aq::SQLParse(query.c_str(), tree) == 0)
  {
    aq::core::SelectStatement ss;
    aq::util::tnodeToSelectStatement(*tree, ss);
    *sqlQuery << ss.to_string(aq::core::SelectStatement::output_t::SQL);
    *aqlQuery << ss.to_string(aq::core::SelectStatement::output_t::AQL);
  }

  b->Add(splitter, 1, wxALL | wxEXPAND, 0);
  this->SetSizer(b);
  splitter->SplitVertically(sqlQuery, aqlQuery, this->GetSize().GetWidth() / 2);
  
}

aqQueryAnalyzer::~aqQueryAnalyzer()
{
  if (this->tree)
  {
    aq::tnode::delete_subtree(this->tree);
  }
}