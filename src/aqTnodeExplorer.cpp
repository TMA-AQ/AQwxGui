#include "aqTnodeExplorer.h"
#include <aq/parser/ID2Str.h>
#include <wx/menu.h>

using namespace aq::gui;

aqTnodeExplorer::aqTnodeExplorer(wxWindow * parent, aq::tnode * tree)
  : wxTreeCtrl(parent, wxID_ANY)
{
  this->AddRoot("Query");
  this->addTnodes(tree, this->GetRootItem());
  this->Expand(this->GetRootItem());
  
  this->Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &aqTnodeExplorer::OnMouseClick, this, wxID_ANY);

}

void aqTnodeExplorer::addTnodes(aq::tnode * tree, wxTreeItemId id)
{
  if (tree == nullptr)
    return;
  std::ostringstream oss;
  oss << aq::id_to_kstring(tree->getTag()) << " | " << tree->to_string();
  auto cid = this->AppendItem(id, oss.str());
  this->addTnodes(tree->left, cid);
  this->addTnodes(tree->right, cid);
  this->addTnodes(tree->next, id);
}

void aqTnodeExplorer::OnMouseClick(wxTreeEvent& evt)
{
  wxMenu menu;
  menu.SetClientData(&evt);
  menu.Append(menu_t::ID_Expand, _("Expand All Children"));
  menu.Append(menu_t::ID_Collapse, _("Collapse All Children"));
  menu.Bind(wxEVT_COMMAND_MENU_SELECTED, &aqTnodeExplorer::OnPopupClick, this, wxID_ANY);
  this->PopupMenu(&menu);
}

void aqTnodeExplorer::OnPopupClick(wxCommandEvent& evt)
{
  wxMenu * menu = static_cast<wxMenu*>(evt.GetEventObject());
  wxTreeEvent * treeEvt = static_cast<wxTreeEvent*>(menu->GetClientData());
  switch (evt.GetId())
  {
  case menu_t::ID_Expand:
    this->ExpandAllChildren(treeEvt->GetItem().GetID());
    break;
  case menu_t::ID_Collapse:
    this->CollapseAllChildren(treeEvt->GetItem().GetID());
    break;
  }
}
