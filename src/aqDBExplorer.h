#ifndef __AQ_DB_EXPLORER_H__
#define __AQ_DB_EXPLORER_H__

#include "aqQueryExec.h"
#include <wx/treectrl.h>
#include <aq/Database.h>

namespace aq {
namespace gui {

  class aqMainFrame;

  class aqDBExplorer : public wxTreeCtrl
  {
  public:
    aqDBExplorer(wxWindow * parent, aqMainFrame * _mf, aqQueryExec * _qe);
  private:
    void addDatabase(const aq::Database& db);
    void OnMouseClick(wxTreeEvent& evt);
    aqMainFrame * mf;
    aqQueryExec * qe;
    std::string root;
  };

}
}

#endif