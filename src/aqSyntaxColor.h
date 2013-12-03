#ifndef __AQ_GUI_SYNTAX_COLOR_H__
#define __AQ_GUI_SYNTAX_COLOR_H__

#include <string>
#include <vector>
#include <wx/textctrl.h>
#include <boost/algorithm/string/find.hpp>

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
        const auto& text = tc->GetValue().ToStdString();
        for (const auto& v : sc.second)
        { 
          typedef const boost::iterator_range<std::string::const_iterator> StringRange;
          auto range = boost::ifind_first(StringRange(text.begin(), text.end()), StringRange(v.begin(), v.end()));
          while (range)
          {
            auto pos = std::distance(text.begin(), range.begin());
            style.SetTextColour(color);
            tc->SetStyle(pos, pos + v.size(), style);
            range = boost::ifind_first(StringRange(text.begin() + pos + v.size(), text.end()), StringRange(v.begin(), v.end()));
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