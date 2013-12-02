#ifndef __AQ_GUI_LOGS_FRAME_H__
#define __AQ_GUI_LOGS_FRAME_H__

#include <wx/wx.h>

namespace aq {
namespace gui {

  class aqLogsFrame : public wxFrame
  {

  public:
    aqLogsFrame(wxWindow * parent);
  
  private:
    void OnClose(wxCloseEvent& event);
    
    wxTextCtrl * logs;
    wxStreamToTextRedirector * redirect;
  };

}
}

#endif