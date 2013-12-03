#include "aqDBExplorer.h"
#include "aqMainFrame.h"
#include <aq/Database.h>
#include <boost/filesystem.hpp>

namespace aq {
namespace gui {

  struct aqTreeItemData : public wxTreeItemData
  {
    std::string name;
  };

  aqDBExplorer::aqDBExplorer(wxWindow * parent, aqMainFrame * _mf, aqQueryExec * _qe)
    : wxTreeCtrl(parent, wxID_ANY), mf(_mf), qe(_qe), selectedItem(nullptr)
  {
    
    this->AddRoot(_("Databases"));

    char * s = ::getenv("AQ_HOME");
    if (s != nullptr)
    {
      this->root = s;
      boost::filesystem::path p(s);
      if (boost::filesystem::exists(p) && boost::filesystem::is_directory(p))
      {
        boost::filesystem::directory_iterator end_it;
        for (boost::filesystem::directory_iterator it(p); it != end_it; ++it)
        {
          if (boost::filesystem::is_directory(it->status()))
          {
            aq::Database db(it->path().string());
            if (db.isValid())
            {
              this->addDatabase(db);
            }
          }
        }
      }
    }
    
    this->Bind(wxEVT_TREE_ITEM_RIGHT_CLICK, &aqDBExplorer::OnMouseClick, this, wxID_ANY);

    this->Expand(this->GetRootItem());

  }

  void aqDBExplorer::addDatabase(const aq::Database& db)
  {
    wxTreeItemId idDB, idTable;
    const auto& desc = db.getBaseDesc();
    idDB = this->AppendItem(this->GetRootItem(), _(desc.name.c_str()));
    aqTreeItemData * data = new aqTreeItemData();
    data->name = desc.name;
    this->SetItemData(idDB, data);

    for (auto& table : desc.table)
    {
      idTable = this->AppendItem(idDB, _(table.name.c_str()));
      for (auto& col : table.colonne)
      {
        this->AppendItem(idTable, _(col.name.c_str()));
      }
    }
  }

  void aqDBExplorer::OnMouseClick(wxTreeEvent& evt)
  {
    aqTreeItemData * data = dynamic_cast<aqTreeItemData*>(this->GetItemData(evt.GetItem()));
    if (evt.GetItem().GetID() == this->GetRootItem().GetID())
    {
      wxMenu menu;
      menu.SetClientData(&evt);
      menu.Append(root_menu_t::ID_Add_Database, _("Add Database"));
      menu.Bind(wxEVT_COMMAND_MENU_SELECTED, &aqDBExplorer::OnRootPopupClick, this, wxID_ANY);
      this->PopupMenu(&menu);
    }
    else if (data != nullptr)
    {
      wxMenu menu;
      menu.SetClientData(&evt);
      menu.Append(db_menu_t::ID_Connect, _("Connect"));
      menu.Append(db_menu_t::ID_Import, _("Import"));
      menu.Append(db_menu_t::ID_Export, _("Export"));
      menu.Append(db_menu_t::ID_Check, _("Check"));
      menu.Bind(wxEVT_COMMAND_MENU_SELECTED, &aqDBExplorer::OnDBPopupClick, this, wxID_ANY);
      this->PopupMenu(&menu);
      if (this->selectedItem != nullptr)
      {
        this->SelectItem(this->selectedItem);
        this->SetItemBold(this->selectedItem);
      }
    }
  }
  
  void aqDBExplorer::OnRootPopupClick(wxCommandEvent& evt)
  {
    wxMenu * menu = static_cast<wxMenu*>(evt.GetEventObject());
    wxTreeEvent * treeEvt = static_cast<wxTreeEvent*>(menu->GetClientData());
    wxMessageBox(_("NOT IMPLEMENTED"), _("AlgoQuest System"), wxOK | wxICON_INFORMATION, this);
  }
  
  void aqDBExplorer::OnDBPopupClick(wxCommandEvent& evt)
  {
    wxMenu * menu = static_cast<wxMenu*>(evt.GetEventObject());
    wxTreeEvent * treeEvt = static_cast<wxTreeEvent*>(menu->GetClientData());
    aqTreeItemData * data = static_cast<aqTreeItemData*>(this->GetItemData(treeEvt->GetItem()));
    assert(data != nullptr);
    switch(evt.GetId())
    {
    case db_menu_t::ID_Connect:
      qe->setDatabase(data->name);
      this->mf->setStatusBar("Connected to " + data->name);
      if (this->selectedItem != nullptr)
      {
        this->SetItemBold(this->selectedItem, false);
      }
      selectedItem = treeEvt->GetItem();
      break;
    case db_menu_t::ID_Check:
    case db_menu_t::ID_Export:
    case db_menu_t::ID_Import:
      wxMessageBox(_("NOT IMPLEMENTED"), _("AlgoQuest System"), wxOK | wxICON_INFORMATION, this);
      break;
    }
  }

}
}
