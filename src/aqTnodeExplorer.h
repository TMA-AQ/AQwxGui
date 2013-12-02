#ifndef __AQ_GUI_TNODE_EXPLORER_H__
#define __AQ_GUI_TNODE_EXPLORER_H__

#include <wx/treectrl.h>
#include <aq/parser/SQLParser.h>

namespace aq {
namespace gui {

  class aqTnodeExplorer : public wxTreeCtrl
  {
  public:
    aqTnodeExplorer(wxWindow * parent, aq::tnode * tree);
  private:
    enum menu_t
    {
      ID_Expand,
      ID_Collapse,
      ID_Expand_All,
      ID_Collapse_All,
    };
    void addTnodes(aq::tnode * tree, wxTreeItemId id);
    void OnMouseClick(wxTreeEvent& evt);
    void OnPopupClick(wxCommandEvent& evt);
  };

}
}

#endif