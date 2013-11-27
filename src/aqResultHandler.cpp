#include "aqResultHandler.h"

using namespace aq::gui;

aqResultHandler::aqResultHandler(wxListView& _results)
  : results(_results), count(0)
{
}

int aqResultHandler::process(std::vector<aq::Row>& rows) 
{
  for (auto& row : rows) 
  { 
    // columns names
    if (this->results.GetItemCount() == 0)
    {
      for (aq::Row::row_t::const_reverse_iterator it = row.computedRow.rbegin(); it != row.computedRow.rend(); ++it)
      {
        auto& row_item = *it;
        if (row_item.displayed)
        {
          size_t size = row_item.tableName != "" ? row_item.tableName.size() + 1 : 0;
          size += row_item.columnName.size();
          std::string s = (row_item.tableName != "") ? row_item.tableName + "." + row_item.columnName : row_item.columnName;
          this->results.AppendColumn(_(s));
        }
      }
    }

    if (row.completed)
    {
      size_t c = 0;
      std::string value;
      for (aq::Row::row_t::const_reverse_iterator it = row.computedRow.rbegin(); it != row.computedRow.rend(); ++it)
      {
        if ((*it).null)
        {
          value = "NULL";
        }
        else
        {
          switch((*it).type)
          {
          case aq::ColumnType::COL_TYPE_INT:
            value = boost::get<aq::ColumnItem<int32_t> >((*it).item).toString();
            break;
          case aq::ColumnType::COL_TYPE_DOUBLE:
            value = boost::get<aq::ColumnItem<double> >((*it).item).toString();
            break;
          case aq::ColumnType::COL_TYPE_DATE:
          case aq::ColumnType::COL_TYPE_BIG_INT:
            value = boost::get<aq::ColumnItem<int64_t> >((*it).item).toString();
            break;
          case aq::ColumnType::COL_TYPE_VARCHAR:
            value = boost::get<aq::ColumnItem<char*> >((*it).item).toString();
            break;
          }
        }
        if (c == 0)
        {
          results.InsertItem(this->count, _(value));
        }
        else
        {
          results.SetItem(this->count, c, _(value));
        }
        c += 1;
      }
      this->count += 1;
    }
  }

  return 0;
}

aq::RowProcess_Intf * aqResultHandler::clone() 
{
  assert(false);
  return nullptr;
}

const std::vector<aq::Column::Ptr>& aqResultHandler::getColumns() const 
{
  return this->columns;
}

void aqResultHandler::setColumn(std::vector<aq::Column::Ptr> _columns) 
{
  this->columns = _columns;
}

unsigned int aqResultHandler::getTotalCount() const 
{
  return this->count;
}