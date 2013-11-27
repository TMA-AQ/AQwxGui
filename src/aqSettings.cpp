#include "aqSettings.h"
#include <fstream>
#include <boost/property_tree/xml_parser.hpp>

namespace aq {
namespace gui {

  aqSettings::aqSettings(wxFrame * parent, const std::string& _xmlCfgFile)
    : wxPanel(parent), xmlCfgFile(_xmlCfgFile)
  {
    wxBoxSizer * bs = new wxBoxSizer(wxVERTICAL);
    
    wxPanel * pan1 = new wxPanel(this, wxID_ANY);
    wxGridSizer * gs1 = new wxGridSizer(2, 10, 10);
    this->AddSettings(pan1, gs1);
    pan1->SetBackgroundColour(wxColour(220, 230, 250));
    pan1->SetSizer(gs1);

    wxPanel * pan2 = new wxPanel(this, wxID_ANY);
    wxBoxSizer * bs2 = new wxBoxSizer(wxHORIZONTAL);
    wxButton * bReset = new wxButton(pan2, settings_button::BUTTON_RESET, _("Reset"));
    wxButton * bCancel = new wxButton(pan2, settings_button::BUTTON_CANCEL, _("Cancel"));
    wxButton * bApply = new wxButton(pan2, settings_button::BUTTON_APPLY, _("Apply"));
    bs2->Add(bReset, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM | wxRIGHT | wxBOTTOM, 10);
    bs2->Add(bCancel, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM | wxRIGHT | wxBOTTOM, 10);
    bs2->Add(bApply, 0, wxALIGN_RIGHT | wxALIGN_BOTTOM | wxRIGHT | wxBOTTOM, 10);
    pan2->SetBackgroundColour(wxColour(120, 130, 150));
    pan2->SetSizer(bs2);

    this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &aqSettings::OnReset, this, settings_button::BUTTON_RESET);
    this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &aqSettings::OnCancel, this, settings_button::BUTTON_CANCEL);
    this->Bind(wxEVT_COMMAND_BUTTON_CLICKED, &aqSettings::OnApply, this, settings_button::BUTTON_APPLY);

    bs->Add(pan1, 1, wxALL, 10);
    bs->Add(pan2, 0, wxALL | wxALIGN_RIGHT, 0);

    this->SetSizer(bs);
    bs->SetSizeHints(this);
  }
  
  void aqSettings::AddSettings(wxPanel * pan, wxGridSizer * gs)
  {
    std::ifstream fin(this->xmlCfgFile.c_str());
    boost::property_tree::ptree pt;
    boost::property_tree::xml_parser::read_xml(fin, pt);
    
    std::string key, value;
    for (auto& c : pt.get_child("variables"))
    {
      key = c.second.get<std::string>("key");
      value = c.second.get<std::string>("value");
      wxStaticText * kst = new wxStaticText(pan, wxID_ANY, _(key.c_str()));
      wxTextCtrl * vtc = new wxTextCtrl(pan, wxID_ANY, _(value.c_str()));
      gs->Add(kst, 0, wxALL, 0);
      gs->Add(vtc, 1, wxALL | wxALIGN_RIGHT, 0);
      vars.insert(std::make_pair(key, vtc));
    }
  }

  void aqSettings::OnReset(wxCommandEvent& WXUNUSED(evt))
  {
  }

  void aqSettings::OnCancel(wxCommandEvent& WXUNUSED(evt))
  {
    this->GetParent()->Hide();
  }

  void aqSettings::OnApply(wxCommandEvent& WXUNUSED(evt))
  {
    this->GetParent()->Hide();
  }

}
}