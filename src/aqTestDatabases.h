#ifndef __AQ_GUI_TESTING_H__
#define __AQ_GUI_TESTING_H__

#include <wx/wx.h>
#include <wx/listctrl.h>
#include <aq/QueryGenerator.h>
#include <aq/DatabaseHelper.h>
#include <aq/TestRunner.h>

#include <wx/splitter.h>
#include <wx/listctrl.h>
#include <wx/spinctrl.h>
#include <wx/combo.h>
#include <wx/gauge.h>

namespace aq {
namespace gui {

  class aqTestDatabases : public wxPanel, public DatabaseGenerator::handle_t
  {
  public:
    aqTestDatabases(wxWindow * parent, std::shared_ptr<aq::TestCase::opt_t> opt);
    void run(boost::shared_ptr<aq::DatabaseGenerator> gen, boost::shared_ptr<aq::TestCase> tc, std::istream& is, const std::list<std::string>& tables);
    void push(const aq::DatabaseGenerator::handle_t::tables_t& tables);
  protected:
    void OnReset(wxCommandEvent& evt);
    void OnSave(wxCommandEvent& evt);
    void OnOk(wxCommandEvent& evt);
    void OnPaint(wxPaintEvent& evt);
    void refresh(bool keep = true);
  private:
    wxSplitterWindow * splitter;
    wxPanel * config;
    wxListView * lvTables;
    boost::shared_ptr<aq::TestCase> cmpQueries;
    std::list<boost::shared_ptr<QueryGenerator> > queriesGenertors;
    std::shared_ptr<TestCase::opt_t> opt;
    
    wxComboCtrl * qfName;
    wxSpinCtrl * nbTables;
    wxSpinCtrl * nbRows;
    wxSpinCtrl * minValues;
    wxSpinCtrl * maxValues;

    wxRadioButton * point_mode1;
    wxRadioButton * point_mode2;
    
    wxRadioButton * gen_mode3;
    wxRadioButton * gen_mode4;

    wxRadioButton * value_mode5;
    wxRadioButton * value_mode6;
    wxRadioButton * value_mode7;

    wxTextCtrl * aqDBPath;
    wxTextCtrl * aqDBName;

    wxTextCtrl * mysqlDBHost;
    wxTextCtrl * mysqlDBUser;
    wxTextCtrl * mysqlDBPass;
    wxTextCtrl * mysqlDBName;

    wxGauge * progressDatabases;
    wxGauge * progressQueries;
  };

  class aqTestQueries : public wxPanel
  {
  public:
    aqTestQueries(wxWindow * parent);
  private:
    wxListView * lvQueries;
  };

}
}

#endif