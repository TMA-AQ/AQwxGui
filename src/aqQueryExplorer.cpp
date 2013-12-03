#include "aqQueryExplorer.h"
#include <boost/algorithm/string/trim.hpp>
#include <boost/filesystem.hpp>
#include <boost/property_tree/xml_parser.hpp>

using namespace aq::gui;

aqQueryExplorer::aqQueryExplorer(wxWindow * parent, const std::string& _filename, aqQueryExec * _qe)
  : 
  wxListCtrl(parent, wxID_ANY, wxDefaultPosition, wxDefaultSize, wxLC_REPORT | wxLC_SINGLE_SEL | wxLC_NO_HEADER), 
  filename(_filename), qe(_qe)
{
  if (boost::filesystem::exists(boost::filesystem::path(this->filename)))
  {
    std::ifstream fin(this->filename.c_str());
    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(fin, pt);

    wxListItem col;
    col.SetText("Query");
    this->InsertColumn(0, col);

    long id = 0;
    std::string query;
    for (auto& c : pt.get_child("queries"))
    {
      wxListItem item;
      try
      {
        id = c.second.get<long>("id");
        query = c.second.get<std::string>("value");
        boost::trim(query);
        item.SetId(id);
        item.SetText(query);
        this->InsertItem(item);
      }
      catch (const std::exception& ex)
      {
        wxMessageBox(_(ex.what()), _("SQL AlgoQuest Parser Error"), wxOK | wxICON_INFORMATION, nullptr);
      }
    }
  }

  // this->Bind(wxEVT_HTML_CELL_CLICKED, &aqQueryExplorer::OnSelected, this, wxID_ANY);
  this->Bind(wxEVT_LIST_ITEM_SELECTED, &aqQueryExplorer::OnSelected, this, wxID_ANY);

}

void aqQueryExplorer::OnSelected(wxCommandEvent& WXUNUSED(evt))
{
  if (qe != nullptr)
  {
    //auto id = this->GetSelection();
    //qe->setQuery(this->GetString(id).ToStdString());

    wxListItem item;
    item.m_col = 0;
    item.m_itemId = this->GetNextItem(-1, wxLIST_NEXT_ALL, wxLIST_STATE_SELECTED);
    item.m_mask = wxLIST_MASK_TEXT;
    if (this->GetItem(item))
    {
      qe->setQuery(item.m_text.ToStdString());
    }
  }
}

void aqQueryExplorer::OnRightClick(wxCommandEvent& WXUNUSED(evt))
{
}