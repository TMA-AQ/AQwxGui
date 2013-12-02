#ifndef __AQ_GUI_QUERY_ANALYZER_H__
#define __AQ_GUI_QUERY_ANALYZER_H__

#include <wx/wx.h>
#include <wx/splitter.h>
#include <wx/textctrl.h>

#include <aq/AQLQuery.h>
#include <aq/parser/SQLParser.h>

namespace aq {
  namespace gui {

    class aqQueryAnalyzer : public wxPanel
    {
    public:
      aqQueryAnalyzer(wxWindow * parent, const std::string & query, aq::core::SelectStatement::output_t type = aq::core::SelectStatement::output_t::SQL);
      ~aqQueryAnalyzer();
    private:
      aq::tnode * tree;
    };

  }
}

#endif