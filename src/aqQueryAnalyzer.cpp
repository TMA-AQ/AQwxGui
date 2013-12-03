
#include <aq/AQLQuery.h>

#include "aqQueryAnalyzer.h"
#include "aqTnodeExplorer.h"
#include "aqSyntaxColor.h"
#include <aq/TreeUtilities.h>
#include <aq/SQLPrefix.h>
#include <aq/AQLParser.h>
#include <iostream>

using namespace aq::gui;

aqQueryAnalyzer::aqQueryAnalyzer(wxWindow * parent, const std::string& query, aq::core::SelectStatement::output_t type)
  : wxPanel(parent), tree(nullptr)
{
  wxBoxSizer * b1 = new wxBoxSizer(wxVERTICAL);
  wxSplitterWindow * splitter1 = new wxSplitterWindow(this, wxID_ANY);
  splitter1->SetSashGravity(0.5);
  splitter1->SetMinimumPaneSize(200);
  b1->Add(splitter1, 1, wxEXPAND, 0);

  wxPanel * queriesPanel = new wxPanel(splitter1, wxID_ANY);
  wxBoxSizer * b2 = new wxBoxSizer(wxVERTICAL);
  wxSplitterWindow * splitter2 = new wxSplitterWindow(queriesPanel, wxID_ANY);
  splitter2->SetSashGravity(0.5);
  splitter2->SetMinimumPaneSize(200); 
  b2->Add(splitter2, 1, wxEXPAND, 0);

  wxPanel * sqlPanel = new wxPanel(splitter2, wxID_ANY);
  wxPanel * aqlPanel = new wxPanel(splitter2, wxID_ANY);

  wxBoxSizer * sqlSizer = new wxBoxSizer(wxVERTICAL);
  wxBoxSizer * aqlSizer = new wxBoxSizer(wxVERTICAL);

  sqlQuery = new wxTextCtrl(sqlPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
  aqlQuery = new wxTextCtrl(aqlPanel, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);

  sqlSizer->Add(sqlQuery, 1, wxEXPAND, 0);
  aqlSizer->Add(aqlQuery, 1, wxEXPAND, 0);

  sqlPanel->SetSizer(sqlSizer);
  aqlPanel->SetSizer(aqlSizer);

  if (aq::SQLParse(query.c_str(), tree) == 0)
  {
    aq::util::generate_parent(tree);
    aq::core::SelectStatement ss;
    aq::util::tnodeToSelectStatement(*tree, ss);
    *sqlQuery << ss.to_string(aq::core::SelectStatement::output_t::SQL);
    *aqlQuery << ss.to_string(aq::core::SelectStatement::output_t::AQL);
    aqSQLSyntaxColor sc1;
    aqAQLSyntaxColor sc2;
    sc1.colorize(sqlQuery);
    sc2.colorize(aqlQuery);
  }

  queriesPanel->SetSizer(b2);
  b2->SetSizeHints(queriesPanel);

  wxPanel * tnePanel = new wxPanel(splitter1, wxID_ANY);
  wxBoxSizer * tneSizer = new wxBoxSizer(wxVERTICAL);
  aqTnodeExplorer * tnodeExplorer = new aqTnodeExplorer(tnePanel, tree);
  tneSizer->Add(tnodeExplorer, 1, wxEXPAND, 0);
  tnePanel->SetSizer(tneSizer);
  
  this->SetSizer(b1);
  b1->SetSizeHints(this);

  splitter2->SplitVertically(sqlPanel, aqlPanel, queriesPanel->GetSize().GetWidth() / 2); // FIXME
  splitter1->SplitVertically(tnePanel, queriesPanel, this->GetSize().GetWidth() / 4); // FIXME
}

aqQueryAnalyzer::~aqQueryAnalyzer()
{
  if (this->tree)
  {
    aq::tnode::delete_subtree(this->tree);
  }
}
