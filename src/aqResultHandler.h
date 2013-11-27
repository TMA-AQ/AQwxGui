#ifndef __AQ_RESULT_HANDLER_H__
#define __AQ_RESULT_HANDLER_H__

#include <aq/RowWritter_Intf.h>
#include <wx/listctrl.h>

namespace aq {
namespace gui {

  class aqResultHandler : public aq::RowWritter_Intf
  {
  public:
    aqResultHandler(wxListView& _results);
    
    int process(std::vector<Row>& rows) override;
    RowProcess_Intf * clone() override;

    const std::vector<Column::Ptr>& getColumns() const override;
    void setColumn(std::vector<Column::Ptr> _columns) override;
    unsigned int getTotalCount() const override;

  private:
    std::vector<Column::Ptr> columns;
    size_t count;
    wxListView& results;
  };

}
}

#endif
