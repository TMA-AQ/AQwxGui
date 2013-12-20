#include "aqTestDatabases.h"
#include "aqSyntaxColor.h"
#include "MySQLHelper.h"
#include <aq/AQLParser.h>
#include <wx/filectrl.h>
#include <fstream>

#include <boost/bind.hpp>
#include <boost/thread.hpp>
#include <boost/filesystem.hpp>

using namespace aq::gui;

namespace helper
{
  const wxColour aq_lightBlue(188, 254, 255);
  const wxColour aq_yellow(240, 255, 188);
  const wxColour aq_red(*wxRED);

  class wxFileCtrlComboPopup : public wxFileCtrl, public wxComboPopup
  {
  public:
    wxFileCtrlComboPopup()
    {
      this->Bind(wxEVT_FILECTRL_FILEACTIVATED, &wxFileCtrlComboPopup::OnFileSelected, this);
    }

    virtual bool Create(wxWindow* parent)
    {
      auto ctrl = wxFileCtrl::Create(parent, wxID_ANY);
      return ctrl;
    }

    virtual wxWindow *GetControl() 
    { 
      return this; 
    }

    virtual void SetStringValue(const wxString& s)
    {
    }

    // Get list selection as a string
    virtual wxString GetStringValue() const
    {
      return this->filename;
    }
    
    // On mouse left up, set the value and close the popup
    void OnMouseClick(wxMouseEvent& WXUNUSED(event))
    {
        // TODO: Send event as well
        Dismiss();
    }

    void OnFileSelected(wxFileCtrlEvent& WXUNUSED(event))
    {
      filename = this->GetPath();
      Dismiss();
    }

  private:
    wxString filename;

  };

}

aqTestDatabases::aqTestDatabases(wxWindow * parent, std::shared_ptr<aq::TestCase::opt_t> _opt)
  : wxPanel(parent, wxID_ANY), opt(_opt)
{ 
  wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
  
  this->splitter = new wxSplitterWindow(this, wxID_ANY);
  this->splitter->SetWindowStyle(wxNO_BORDER);
  this->splitter->SetSashGravity(0);
  this->splitter->SetMinimumPaneSize(50);
  
  this->config = new wxPanel(splitter, wxID_ANY);
  {
    wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);

    wxPanel * genOpt = new wxPanel(config, wxID_ANY);
    {
      wxBoxSizer * bs = new wxBoxSizer(wxVERTICAL);
      wxFlexGridSizer * gs = new wxFlexGridSizer(2, 5, 5);
      gs->AddGrowableCol(0, 0);
      gs->AddGrowableCol(1, 1);

      this->qfName = new wxComboCtrl(genOpt, wxID_ANY, _(opt->generator_filename));
      this->nbTables = new wxSpinCtrl(genOpt, wxID_ANY);
      this->nbRows = new wxSpinCtrl(genOpt, wxID_ANY);
      this->minValues = new wxSpinCtrl(genOpt, wxID_ANY);
      this->maxValues = new wxSpinCtrl(genOpt, wxID_ANY);

      helper::wxFileCtrlComboPopup * fccp = new helper::wxFileCtrlComboPopup();
      qfName->SetPopupControl(fccp);

      nbTables->SetRange(1, 10);
      nbTables->SetValue(opt->nb_tables);
      nbRows->SetRange(1, 100);
      nbRows->SetValue(opt->nb_rows);
      minValues->SetRange(1, 10000);
      minValues->SetValue(opt->min_value);
      maxValues->SetRange(1, 10000);
      maxValues->SetValue(opt->max_value);

      gs->Add(new wxStaticText(genOpt, wxID_ANY, _("queries file generators")), 0, wxRIGHT, 10);
      gs->Add(qfName, 0, wxEXPAND, 0);
      gs->Add(new wxStaticText(genOpt, wxID_ANY, _("nb tables")), 0, wxRIGHT, 10);
      gs->Add(nbTables, 1, wxEXPAND, 0);
      gs->Add(new wxStaticText(genOpt, wxID_ANY, _("nb rows")), 0, wxRIGHT, 10);
      gs->Add(nbRows, 0, wxEXPAND, 0);
      gs->Add(new wxStaticText(genOpt, wxID_ANY, _("min values")), 0, wxRIGHT, 10);
      gs->Add(minValues, 0, wxEXPAND, 0);
      gs->Add(new wxStaticText(genOpt, wxID_ANY, _("max values")), 0, wxRIGHT, 10);
      gs->Add(maxValues, 0, wxEXPAND, 0);
      
      bs->Add(gs, 0, wxEXPAND, 0);

      wxBoxSizer * bsMode = nullptr;
      wxPanel * pMode = nullptr;
      
      pMode = new wxPanel(genOpt, wxID_ANY);
      bsMode = new wxBoxSizer(wxHORIZONTAL);
      this->point_mode1 = new wxRadioButton(pMode, wxID_ANY, _("full db mode"));
      this->point_mode2 = new wxRadioButton(pMode, wxID_ANY, _("merged db mode"));
      bsMode->Add(point_mode1, 1, wxEXPAND, 0);
      bsMode->Add(point_mode2, 1, wxEXPAND, 0);
      pMode->SetSizer(bsMode);
      bs->Add(pMode, 0, wxEXPAND, 0);
      
      pMode = new wxPanel(genOpt, wxID_ANY);
      bsMode = new wxBoxSizer(wxHORIZONTAL);
      this->gen_mode3 = new wxRadioButton(pMode, wxID_ANY, _("full pt mode"));
      this->gen_mode4 = new wxRadioButton(pMode, wxID_ANY, _("intersect pt mode"));
      bsMode->Add(gen_mode3, 1, wxEXPAND, 0);
      bsMode->Add(gen_mode4, 1, wxEXPAND, 0);
      pMode->SetSizer(bsMode);
      bs->Add(pMode, 0, wxEXPAND, 0);
      
      pMode = new wxPanel(genOpt, wxID_ANY);
      bsMode = new wxBoxSizer(wxHORIZONTAL);
      this->value_mode5 = new wxRadioButton(pMode, wxID_ANY, _("all unique values"));
      this->value_mode6 = new wxRadioButton(pMode, wxID_ANY, _("all multiple values"));
      this->value_mode7 = new wxRadioButton(pMode, wxID_ANY, _("random values"));
      bsMode->Add(value_mode5, 1, wxEXPAND, 0);
      bsMode->Add(value_mode6, 1, wxEXPAND, 0);
      bsMode->Add(value_mode7, 1, wxEXPAND, 0);
      pMode->SetSizer(bsMode);
      bs->Add(pMode, 0, wxEXPAND, 0);

      switch (opt->point_mode) 
      {
      case DatabaseGenerator::point_mode_t::FULL:    this->point_mode1->SetValue(true); break;
      case DatabaseGenerator::point_mode_t::MIN_MAX: this->point_mode2->SetValue(true); break;
      }

      switch (opt->gen_mode)
      {
      case DatabaseGenerator::gen_mode_t::ALL:       this->gen_mode3->SetValue(true); break;
      case DatabaseGenerator::gen_mode_t::INTERSECT: this->gen_mode4->SetValue(true); break;
      }

      switch (opt->value_mode)
      {
      case DatabaseGenerator::value_mode_t::ALL_UNIQUE:   this->value_mode5->SetValue(true); break;
      case DatabaseGenerator::value_mode_t::ALL_MULTIPLE: this->value_mode6->SetValue(true); break;
      case DatabaseGenerator::value_mode_t::RANDOM:       this->value_mode7->SetValue(true); break;
      }

      genOpt->SetSizer(bs);
      bs->SetSizeHints(genOpt);
    }

    wxPanel * aqOpt = new wxPanel(config, wxID_ANY);
    {
      this->aqDBPath = new wxTextCtrl(aqOpt, wxID_ANY, _(opt->aq_path));
      this->aqDBName = new wxTextCtrl(aqOpt, wxID_ANY, _(opt->aq_name));
      
      wxFlexGridSizer * gs2 = new wxFlexGridSizer(2, 5, 5);
      gs2->AddGrowableCol(0, 0);
      gs2->AddGrowableCol(1, 1);
      gs2->Add(new wxStaticText(aqOpt, wxID_ANY, _("AQ database path")), 0, wxRIGHT, 10);
      gs2->Add(aqDBPath, 0, wxEXPAND, 0);
      gs2->Add(new wxStaticText(aqOpt, wxID_ANY, _("AQ database name")), 0, wxRIGHT, 10);
      gs2->Add(aqDBName, 0, wxEXPAND, 0);
      aqOpt->SetSizer(gs2);
      gs2->SetSizeHints(aqOpt);
    }
    
    wxPanel * mysqlOpt = new wxPanel(config, wxID_ANY);
    {
      this->mysqlDBHost = new wxTextCtrl(mysqlOpt, wxID_ANY, _(opt->mysql_host));
      this->mysqlDBUser = new wxTextCtrl(mysqlOpt, wxID_ANY, _(opt->mysql_user));
      this->mysqlDBPass = new wxTextCtrl(mysqlOpt, wxID_ANY, _(opt->mysql_pass), wxDefaultPosition, wxDefaultSize, wxTE_PASSWORD);
      this->mysqlDBName = new wxTextCtrl(mysqlOpt, wxID_ANY, _(opt->mysql_name));
      
      wxFlexGridSizer * gs3 = new wxFlexGridSizer(2, 5, 5);
      gs3->AddGrowableCol(0, 0);
      gs3->AddGrowableCol(1, 0);
      gs3->Add(new wxStaticText(mysqlOpt, wxID_ANY, _("MySQL database host")), 0, wxRIGHT, 10);
      gs3->Add(mysqlDBHost, 0, wxEXPAND, 0);
      gs3->Add(new wxStaticText(mysqlOpt, wxID_ANY, _("MySQL database user")), 0, wxRIGHT, 10);
      gs3->Add(mysqlDBUser, 0, wxEXPAND, 0);
      gs3->Add(new wxStaticText(mysqlOpt, wxID_ANY, _("MySQL database pass")), 0, wxRIGHT, 10);
      gs3->Add(mysqlDBPass, 0, wxEXPAND, 0);
      gs3->Add(new wxStaticText(mysqlOpt, wxID_ANY, _("MySQL database name")), 0, wxRIGHT, 10);
      gs3->Add(mysqlDBName, 0, wxEXPAND, 0);
      mysqlOpt->SetSizer(gs3);
      gs3->SetSizeHints(mysqlOpt);
    }
    
    wxPanel * gauges = new wxPanel(config, wxID_ANY);
    {
      this->progressDatabases = new wxGauge(gauges, wxID_ANY, 100);
      this->progressQueries = new wxGauge(gauges, wxID_ANY, 100);

      wxBoxSizer * bs = new wxBoxSizer(wxVERTICAL);
      bs->Add(progressDatabases, 0, wxEXPAND, 0);
      bs->Add(progressQueries, 0, wxEXPAND, 0);
      gauges->SetSizer(bs);
      bs->SetSizeHints(gauges);
    }

    wxPanel * buttons = new wxPanel(config, wxID_ANY);
    {
      wxButton * bReset = new wxButton(buttons, wxID_RESET, _("Reset"));
      wxButton * bSave = new wxButton(buttons, wxID_SAVE);
      wxButton * bOk = new wxButton(buttons, wxID_OK);

      buttons->Bind(wxEVT_BUTTON, &aqTestDatabases::OnReset, this, wxID_RESET);
      buttons->Bind(wxEVT_BUTTON, &aqTestDatabases::OnSave, this, wxID_SAVE);
      buttons->Bind(wxEVT_BUTTON, &aqTestDatabases::OnOk, this, wxID_OK);

      wxBoxSizer * bs = new wxBoxSizer(wxHORIZONTAL);
      wxSizerFlags flags;
      flags.Bottom().Right().Border(wxRIGHT, 10);
      bs->Add(bReset, flags);
      bs->Add(bSave, flags);
      bs->Add(bOk, flags);
      wxBoxSizer * b = new wxBoxSizer(wxVERTICAL);
      b->Add(bs, flags.Proportion(1));
      buttons->SetSizer(b);
      b->SetSizeHints(buttons);
    }

    box->Add(genOpt, 0, wxALL | wxEXPAND, 10);
    box->Add(aqOpt, 0, wxALL | wxEXPAND, 10);
    box->Add(mysqlOpt, 0, wxALL | wxEXPAND, 10);
    box->Add(gauges, 0, wxALL | wxEXPAND, 10);
    box->Add(buttons, 1, wxBOTTOM | wxEXPAND, 10);

    config->SetSizer(box);
    box->SetSizeHints(config);
    box->Fit(config);
  }
  
  this->lvTables = new wxListView(splitter, wxID_ANY);
  
  this->splitter->SplitVertically(this->config, this->lvTables, config->GetSize().GetWidth());

  box->Add(splitter, 1, wxEXPAND | wxALL, 0);
  
  this->SetSizer(box);
  box->SetSizeHints(this);

  this->Bind(wxEVT_PAINT, &aqTestDatabases::OnPaint, this, wxID_ANY);
}

void aqTestDatabases::run(boost::shared_ptr<aq::DatabaseGenerator> gen, boost::shared_ptr<aq::TestCase> tc, std::istream& is, const std::list<std::string>& tables)
{
  cmpQueries = tc;
  this->queriesGenertors.clear();
  while (!is.eof())
  {   
    std::string genQuery;
    QueryGenerator::ops_t ops;
    QueryGenerator::idents_t idents;
    QueryGenerator::parse(is, genQuery, ops, idents);
    if (genQuery != "")
    {
      boost::shared_ptr<QueryGenerator> gen(new QueryGenerator(genQuery, ops, idents));
      this->queriesGenertors.push_back(gen);
    }
  }

  lvTables->ClearAll();
  
  for (const auto& table : tables)
  {
    lvTables->AppendColumn(table);
  }
  lvTables->AppendColumn(_("report"));

  size_t nExpected = gen->getNbDB();
  this->progressDatabases->SetRange(nExpected);
  this->progressDatabases->SetValue(0);

  // size_t nGenerated = gen->generate(this);
  boost::thread th(boost::bind(&aq::DatabaseGenerator::generate, gen, this));

  //if (nExpected > lvTables->GetItemCount())
  //{
  //  wxMessageBox("TOO MUCH GENERATED DATABASES", "WARNING");
  //}
  //else
  //{
  //  std::ostringstream oss;
  //  oss << lvTables->GetItemCount() << " databases generated";
  //  wxMessageBox(oss.str(), "INFOS");
  //}
  // cmpQueries = nullptr;
  
}

void aqTestDatabases::push(const handle_t::tables_t& tables)
{
  assert(cmpQueries);
  std::ostringstream msg;
  if (lvTables->GetItemCount() < 1000)
  {
    auto item = this->lvTables->InsertItem(this->lvTables->GetItemCount(), "");
    this->lvTables->SetItemBackgroundColour(item, this->lvTables->GetItemCount() % 2 ? helper::aq_lightBlue : helper::aq_yellow);
    size_t col = 0;
    cmpQueries->clean();
    for (const auto& table : tables)
    {
      msg.str("");
      auto min = *table.second.begin();
      auto max = *table.second.rbegin();
      msg << "[" << min << ";" << max << "]";
      this->lvTables->SetItem(item, col, msg.str());
      col += 1;

      cmpQueries->createTable(table.first);
      cmpQueries->insertValues(table);
    }

    std::string aqlQuery;
    aq::DatabaseIntf::result_t result;
    size_t nb_queries = 0;
    size_t nb_success = 0;
    size_t nb_failure = 0;
    for (const auto& gen : queriesGenertors) 
    {
      gen->reset();
      nb_queries += gen->getNbQueries();
    }
    this->progressQueries->SetRange(nb_queries);
    this->progressQueries->SetValue(0);
    for (auto& gen : queriesGenertors)
    {
      while ((aqlQuery = gen->next()) != "")
      {
        aq::core::SelectStatement ss;
        aq::parser::parse(aqlQuery, ss);
        if (!cmpQueries->execute(ss, result))
        {
          nb_failure += 1;
          this->lvTables->SetItemBackgroundColour(item, helper::aq_red);
        }
        else
        {
          nb_success += 1;
        }

        this->progressQueries->SetValue(this->progressQueries->GetValue() + 1);
        assert(this->progressQueries->GetValue() <= nb_queries);
      }
    }
    assert((nb_success + nb_failure) == nb_queries);

    msg.str("");
    msg << nb_queries << " checked : sucess => " << nb_success << " | failure => " << nb_failure;
    this->lvTables->SetItem(item, col, msg.str());

    this->progressDatabases->SetValue(this->progressDatabases->GetValue() + 1);
    wxPaintEvent e;
    this->GetEventHandler()->AddPendingEvent(e);

  }
}

void aqTestDatabases::OnReset(wxCommandEvent& WXUNUSED(evt))
{
}

void aqTestDatabases::OnSave(wxCommandEvent& WXUNUSED(evt))
{
}

void aqTestDatabases::OnOk(wxCommandEvent& WXUNUSED(evt))
{

  opt->generator_filename = qfName->GetValue();
  opt->nb_tables = nbTables->GetValue();
  opt->nb_rows = nbRows->GetValue();
  opt->min_value = minValues->GetValue();
  opt->max_value = maxValues->GetValue();

  opt->aq_path = aqDBPath->GetValue();
  opt->aq_name = aqDBName->GetValue();

  opt->mysql_host = mysqlDBHost->GetValue();
  opt->mysql_user = mysqlDBUser->GetValue();
  opt->mysql_pass = mysqlDBPass->GetValue();
  opt->mysql_name = mysqlDBName->GetValue();

  opt->point_mode = this->point_mode1->GetValue() ? DatabaseGenerator::point_mode_t::FULL : DatabaseGenerator::point_mode_t::MIN_MAX;
  opt->gen_mode = this->gen_mode3->GetValue() ? DatabaseGenerator::gen_mode_t::ALL : DatabaseGenerator::gen_mode_t::INTERSECT;
  opt->value_mode = 
    this->value_mode5->GetValue() ? DatabaseGenerator::value_mode_t::ALL_UNIQUE : 
    this->value_mode6->GetValue() ? DatabaseGenerator::value_mode_t::ALL_MULTIPLE :
    DatabaseGenerator::value_mode_t::RANDOM;

  std::ostringstream oss;
  std::list<std::string> tables;
  for (size_t i = 1; i <= opt->nb_tables; i++)
  {
    oss.str("");
    oss << "t" << i;
    tables.push_back(oss.str());
  }

  boost::shared_ptr<aq::DatabaseGenerator> gen(
    new aq::DatabaseGenerator(tables, opt->nb_rows, opt->min_value, opt->max_value, opt->point_mode, opt->gen_mode, opt->value_mode));
  
  boost::shared_ptr<aq::TestCase> tc(new aq::TestCase);
  aq::Settings settings;
  settings.initPath(opt->aq_path + "/" + opt->aq_name);
  settings.aqEngine = "aq-engine.exe";
  settings.aqLoader = "aq-loader.exe";
  try 
  { 
    std::shared_ptr<DatabaseIntf> db1(new AlgoQuestDatabase(settings));
    std::shared_ptr<DatabaseIntf> db2(new MySQLDatabase(opt->mysql_host, opt->mysql_user, opt->mysql_pass, opt->mysql_name));
    tc->add(db1);
    tc->add(db2);

    std::fstream fin(opt->generator_filename.c_str());
    if (fin.is_open())
    {
      this->run(gen, tc, fin, tables);
    }
    else
    {
      wxMessageBox("cannot find " + opt->generator_filename, "ERROR");
    }
  }
  catch (const std::exception& ex)
  {
      wxMessageBox(ex.what(), "ERROR");
  }
}

void aqTestDatabases::OnPaint(wxPaintEvent& evt)
{
  this->refresh();
  evt.Skip();
}

void aqTestDatabases::refresh(bool keep)
{
  if (lvTables->GetColumnCount() >= 2)
  {
    auto w = (lvTables->GetSize().GetWidth() * 2 / 3) / (lvTables->GetColumnCount() - 1);
    for (size_t i = 0; i < (lvTables->GetColumnCount() - 1); i++)
    {
      lvTables->SetColumnWidth(i, w);
    }
    auto wprogress = lvTables->GetSize().GetWidth() / 3;
    auto i = lvTables->GetColumnCount() - 1;
    lvTables->SetColumnWidth(i, wprogress);
  }
}

// ------------------------------------------------------------------------------
// ------------------------------------------------------------------------------

aqTestQueries::aqTestQueries(wxWindow * parent)
  : wxPanel(parent, wxID_ANY)
{
  std::ifstream fin("E:/AQPython/db_algoquest/query_2_tables.gen");
  // std::ifstream fin("E:/AQPython/db_algoquest/query_3_tables_order.gen");
  std::string genQuery;
  QueryGenerator::ops_t ops;
  QueryGenerator::idents_t idents;
  QueryGenerator::parse(fin, genQuery, ops, idents);
  
  wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
  wxSplitterWindow * splitter1 = new wxSplitterWindow(this, wxID_ANY);
  splitter1->SetWindowStyle(wxNO_BORDER);
  splitter1->SetMinimumPaneSize(100);

  wxPanel * infos = new wxPanel(splitter1, wxID_ANY);
  wxBoxSizer * binfos = new wxBoxSizer(wxHORIZONTAL);
  
  wxListView * lv = new wxListView(infos, wxID_ANY);
  size_t c = 0;
  for (auto& op : ops)
  {
    size_t l = 0;
    lv->AppendColumn(_(op.first));
    for (auto& value : op.second)
    {
      assert(l <= lv->GetItemCount());
      if (l == lv->GetItemCount())
      {
        lv->InsertItem(lv->GetItemCount(), _(""));
      }
      lv->SetItem(l, c, _(value));
      l += 1;
    }
    c += 1;
  }
  
  size_t l = 0;
  lv->AppendColumn("Variables");
  for (auto& ident : idents)
  {
    assert(l <= lv->GetItemCount());
    if (l == lv->GetItemCount())
    {
      lv->InsertItem(lv->GetItemCount(), _(""));
    }
    lv->SetItem(l, c, _(ident));
    l += 1;
  }

  binfos->Add(lv, 1, wxALL | wxEXPAND, 0);

  infos->SetSizer(binfos);

  wxPanel * queriesPanel = new wxPanel(splitter1, wxID_ANY);
  wxBoxSizer * queriesBox = new wxBoxSizer(wxHORIZONTAL);
  wxSplitterWindow * splitter2 = new wxSplitterWindow(queriesPanel, wxID_ANY);
  splitter2->SetWindowStyle(wxNO_BORDER);
  splitter2->SetMinimumPaneSize(100);
  wxTextCtrl * qgTC = new wxTextCtrl(splitter2, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
  wxTextCtrl * aqTC = new wxTextCtrl(splitter2, wxID_ANY, _T(""), wxDefaultPosition, wxDefaultSize, wxTE_MULTILINE | wxTE_RICH);
  splitter2->SplitHorizontally(qgTC, aqTC);
  queriesBox->Add(splitter2, 1, wxALL | wxEXPAND, 0);
  queriesPanel->SetSizer(queriesBox);
  
  qgTC->AppendText(genQuery);
  
  std::string aqlQuery;
  QueryGenerator gen(genQuery, ops, idents);
  while ((aqlQuery = gen.next()) != "")
  {
    //aq::core::SelectStatement ss;
    //aq::parser::parse(aqlQuery, ss);
    //textCtrl->AppendText(ss.to_string(aq::core::SelectStatement::output_t::AQL));
    aqTC->AppendText(aqlQuery);
    aqTC->AppendText("\n");
  }
  aqAQLSyntaxColor sc;
  sc.colorize(aqTC);

  aqTC->ShowPosition(0);

  box->Add(splitter1, 1, wxALL | wxEXPAND, 0);
  splitter1->SplitHorizontally(infos, queriesPanel);
  this->SetSizer(box);
  this->Show();

  //DatabaseGenerator db_gen(3, 10, 10, 20, DatabaseGenerator::mode_t::RANDOM);
  //db_gen.generate(this);

}
