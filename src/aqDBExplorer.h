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
    enum root_menu_t
    {
      ID_Add_Database = 1,
    };

    enum db_menu_t
    {
      ID_Connect = 1,
      ID_Import,
      ID_Export,
      ID_Check,
    };
  public:
    aqDBExplorer(wxWindow * parent, aqMainFrame * _mf, aqQueryExec * _qe);
  private:
    void addDatabase(const aq::Database& db);
    void OnMouseClick(wxTreeEvent& evt);
    void OnRootPopupClick(wxCommandEvent& evt);
    void OnDBPopupClick(wxCommandEvent& evt);
    aqMainFrame * mf;
    aqQueryExec * qe;
    std::string root;
    wxTreeItemId selectedItem;
  };

}
}

#endif