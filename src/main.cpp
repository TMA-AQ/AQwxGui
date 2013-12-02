#include "aqMainFrame.h"
#include <iostream>

int yyerror(char const * error)
{
  wxMessageBox(_(error), _("SQL AlgoQuest Parser Error"), wxOK | wxICON_INFORMATION, nullptr);
  return 0;
}

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
  aqMainFrame * frame = new aqMainFrame( _("AlgoQuest"), wxPoint(10, 10), wxSize(1200, 960) );
  frame->Show(true);
  SetTopWindow(frame);
  return true;
}
