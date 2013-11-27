#ifndef __GUI_AQ_SETTINGS_H__
#define __GUI_AQ_SETTINGS_H__

#include <wx/wx.h>
#include <map>

namespace aq {
namespace gui {

  class aqSettings : public wxPanel
  {
  public:
    aqSettings(wxFrame * parent, const std::string& _xmlCfgFile);
  
  private:

    void AddSettings(wxPanel * pan, wxGridSizer * gs);

    void OnReset(wxCommandEvent&);
    void OnCancel(wxCommandEvent&);
    void OnApply(wxCommandEvent&);

    enum settings_button
    {
      BUTTON_RESET,
      BUTTON_CANCEL,
      BUTTON_APPLY,
    };

    const std::string& xmlCfgFile;
    std::map<std::string, wxTextCtrl*> vars;
  };

}
}

#endif