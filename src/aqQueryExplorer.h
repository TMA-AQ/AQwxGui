#ifndef __AQ_GUI_QUERY_EXPLORER_H__
#define __AQ_GUI_QUERY_EXPLORER_H__

#include "aqQueryExec.h"
#include <wx/listctrl.h>
#include <string>

namespace aq {
namespace gui {

  class aqQueryExplorer : public wxListCtrl
  {
  public:
    aqQueryExplorer(wxWindow * parent, const std::string& _filename, aqQueryExec * _qe);
  private:
    void OnSelected(wxCommandEvent& evt);
    void OnRightClick(wxCommandEvent& evt);
    std::string filename;
    aqQueryExec * qe;
  };

}
}

#endif