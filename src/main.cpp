#include "aqMainFrame.h"

namespace aq {
namespace gui {

class aqGui: public wxApp
{
  virtual bool OnInit();
};

}
}

using namespace aq::gui;

IMPLEMENT_APP(aqGui);

bool aqGui::OnInit()
{
  //
  // TODO : initialize logger

  aqMainFrame * frame = new aqMainFrame( _("AlgoQuest"), wxPoint(10, 10), wxSize(1200, 960) );
  frame->Show(true);
  SetTopWindow(frame);
  return true;
}
