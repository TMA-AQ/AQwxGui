#include "aqSyntaxColor.h"

using namespace aq::gui;

aqSQLSyntaxColor::aqSQLSyntaxColor()
{
  if (!initialized) // FIXME : not thread-safe
  {
    wxColourDatabase cdb;

    std::vector<std::string> mainSQLVerbs, secondSQLVerbs;
    std::string sql_mvs[] = { "SELECT ", "FROM ", "WHERE ", "GROUP BY ", "ORDER BY ", "PARTITION BY", "OVER " }; // VISUAL do not support initializer list
    for (auto& v : sql_mvs)
    {
      mainSQLVerbs.push_back(v);
    }
    std::string sql_svs[] = { "AND", "OR", "IN", "COUNT", "MAX", "MIN", "AVG", "SUM" };
    for (auto& v : sql_svs)
    {
      secondSQLVerbs.push_back(v);
    }

    syntax.push_back(std::make_pair(cdb.Find(_("FOREST GREEN")), secondSQLVerbs));
    syntax.push_back(std::make_pair(cdb.Find(_("BLUE")), mainSQLVerbs));
    
    initialized = true;
  }
}

aqAQLSyntaxColor::aqAQLSyntaxColor()
{
  if (!initialized) // FIXME : not thread-safe
  {
    wxColourDatabase cdb;

    std::vector<std::string> mainAQLVerbs, secondAQLVerbs, joinAQLVerbs, opAQLVerbs;
    std::string aql_mvs[] = { "SELECT", "FROM ", "WHERE ", "GROUP ", "ORDER " };
    for (auto& v : aql_mvs)
    {
      mainAQLVerbs.push_back(v);
    }
    std::string aql_svs[] = { "AND", "IN " };
    for (auto& v : aql_svs)
    {
      secondAQLVerbs.push_back(v);
    }
    std::string aql_joinvs[] = { "K_INNER ", "K_OUTER " };
    for (auto& v : aql_joinvs)
    {
      joinAQLVerbs.push_back(v);
    }
    std::string aql_opvs[] = { "K_JEQ ", "K_JNO ", "K_JAUTO ", "K_JINF ", "K_JIEQ ", "K_JSUP ", "K_JSEQ " };
    for (auto& v : aql_opvs)
    {
      opAQLVerbs.push_back(v);
    }

    syntax.push_back(std::make_pair(cdb.Find(_("BLUE")), mainAQLVerbs));
    syntax.push_back(std::make_pair(cdb.Find(_("FOREST GREEN")), secondAQLVerbs));
    syntax.push_back(std::make_pair(cdb.Find(_("PURPLE")), joinAQLVerbs));
    syntax.push_back(std::make_pair(cdb.Find(_("RED")), opAQLVerbs));
 
    initialized = true;
  }
}