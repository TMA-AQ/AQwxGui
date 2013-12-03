#ifndef __AQ_GUI_SYNTAX_COLOR_H__
#define __AQ_GUI_SYNTAX_COLOR_H__

#include <string>
#include <vector>
#include <wx/textctrl.h>

namespace aq {
namespace gui {

  template <class T>
  class aqSyntaxColor
  {
  public:
    void colorize(wxTextCtrl * tc) const
    {
      auto style = tc->GetDefaultStyle();
      for (const auto& sc : syntax)
      {
        const auto& color = sc.first;
        const auto& text = tc->GetValue();
        for (const auto& v : sc.second)
        {
          std::string::size_type pos = text.find(v);
          while (pos != std::string::npos)
          {
            style.SetTextColour(color);
            tc->SetStyle(pos, pos + v.size(), style);
            pos = text.find(v, pos + v.size());
          }
        }
      }
    }
  protected:
    static bool initialized;
    static std::vector<std::pair<wxColor, std::vector<std::string> > > syntax;
  };

  template <class T> bool aqSyntaxColor<T>::initialized(false);
  template <class T> std::vector<std::pair<wxColor, std::vector<std::string> > > aqSyntaxColor<T>::syntax;

  class aqSQLSyntaxColor : public aqSyntaxColor<aqSQLSyntaxColor>
  {
  public:
    aqSQLSyntaxColor();
  };
  
  class aqAQLSyntaxColor : public aqSyntaxColor<aqAQLSyntaxColor>
  {
  public:
    aqAQLSyntaxColor();
  };

}
}

#endif