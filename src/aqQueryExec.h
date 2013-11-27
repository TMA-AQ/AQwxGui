#ifndef __AQ_QUERY_EXEC_H__
#define __AQ_QUERY_EXEC_H__

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>
#include <wx/listctrl.h>

namespace aq {
namespace gui {

  class aqQueryExec : public wxPanel
  {
  public:
    aqQueryExec(wxWindow * parent);
    void setDatabase(const std::string& _database) { this->database = _database; }
  protected:
    void OnRun(wxCommandEvent& e);
    
  private:
    wxSplitterWindow * splitter;
    wxTextCtrl * sqlQuery;
    wxListView * result;
    wxPanel * pan1;
    wxPanel * pan2;
    std::string root;
    std::string database;
  };

}
}

#endif