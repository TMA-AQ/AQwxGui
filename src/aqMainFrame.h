#include "aqSettings.h"
#include "aqLogsFrame.h"
#include "VerbBuilder.h"
#include <wx/wx.h>

namespace aq {
namespace gui {

class aqMainFrame: public wxFrame
{
public:

  aqMainFrame(const wxString& title, const wxPoint& pos, const wxSize& size);
  ~aqMainFrame();

  void setStatusBar(const std::string& msg);

protected:
  void OnOpen(wxCommandEvent& event);
  void OnConnect(wxCommandEvent& event);
  void OnProperties(wxCommandEvent& event);
  void OnLogs(wxCommandEvent& event);
  void OnQuit(wxCommandEvent& event);
  void OnAbout(wxCommandEvent& event);

  void OnTest(wxCommandEvent& event);

private:
  wxFrame * settingsFrame;
  aqLogsFrame * logsFrame;
  aqSettings * settings;
  aq::VerbBuilder * vb;
};

enum
{
  ID_Quit = 1,
  ID_Connect,
  ID_Properties,
  ID_Logs,
  ID_Open,
  ID_About,
  ID_Test,
};

}
}
