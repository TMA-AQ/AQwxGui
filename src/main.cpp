#include "aqMainFrame.h"
#include "aqTestDatabases.h"
#include <aq/verbs/VerbFactory.h>
#include <iostream>
#include <boost/program_options.hpp>

namespace po = boost::program_options;

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
  int parseOptions(std::ostream& os);
  void benchmark();
  boost::shared_ptr<TestCase::opt_t> opt;
  bool launch_benchmark;
};

}
}

using namespace aq::gui;

IMPLEMENT_APP(aqGui);

bool aqGui::OnInit()
{
  int rc = 0;
  std::ostringstream oss;
  rc = this->parseOptions(oss);

  if (rc == EXIT_FAILURE)
  {
    wxMessageDialog dialog(nullptr, _(oss.str()), _("Error"), wxOK | wxICON_ERROR | wxCENTRE);
    dialog.ShowModal();
    return false;
  }

  if (oss.str() != "")
  {
    wxFrame * frame = new wxFrame(nullptr, wxID_ANY, _("AlgoQuest Infos"));
    wxPanel * pan = new wxPanel(frame, wxID_ANY);
    wxStaticText * st = new wxStaticText(pan, wxID_ANY, oss.str());

    wxSizerFlags flags;
    flags.Center();

    wxBoxSizer * bs = new wxBoxSizer(wxVERTICAL);
    bs->Add(st, flags);
    pan->SetSizer(bs);
    bs->SetSizeHints(pan);
    bs->Fit(pan);

    bs = new wxBoxSizer(wxHORIZONTAL);
    bs->Add(pan, flags.Expand().Proportion(1));
    frame->SetSizer(bs);
    bs->SetSizeHints(frame);
    bs->Fit(frame);

    frame->SetIcon(wxIcon("AQ.ico", wxBITMAP_TYPE_ICO));

    frame->Show();
    SetTopWindow(frame);

    return true;
  }

  aq::Logger::getInstance().setLevel(AQ_LOG_DEBUG);
  aq::Logger::getInstance().redirectToStream(std::cout);

  aq::verb::VerbFactory::GetInstance().setBuilder(new VerbBuilder);

  if (this->launch_benchmark)
  {
    this->benchmark();
  }
  else
  {
    aqMainFrame * frame = new aqMainFrame( _("AlgoQuest"), wxPoint(10, 10), wxSize(1200, 960) );
    frame->Show(true);
  }

  return true;
}

int aqGui::parseOptions(std::ostream& os)
{
  try
  {
		aq::Settings settings; // FIXME
    std::string propertiesFile;
    
		// log options
		std::string ident;
		unsigned int level;
		bool lock_mode = false;
		bool date_mode = false;
		bool pid_mode = false;
    
    this->opt.reset(new TestCase::opt_t);
    
    //
    // Get AlgoQuest HOME
    char * s = ::getenv("AQ_HOME");
    if (s != NULL)
      opt->aq_path = s;

    //
    // if aq.ini exists in current directory, use it as default settings
    settings.iniFile = "aq.ini";
    boost::filesystem::path iniFile(settings.iniFile);
    if (boost::filesystem::exists(iniFile))
    {
      settings.load(settings.iniFile);
    }

    //
    // look for properties file in args
    for (size_t i = 1; i < argc; i++)
    {
      //
      // read ini file
      if ((strcmp(argv[i], "-s") == 0) || (strcmp(argv[i], "--settings") == 0))
      {
        if ((i + 1) < argc)
        {
          propertiesFile = argv[i+1];
          settings.load(propertiesFile);
        }
      }

    }

    //
    // command line arguments are prior to settings file
    po::options_description all("Allowed options");
		all.add_options()
			("help,h", "produce help message")
      ("settings,s", po::value<std::string>(&propertiesFile), "")
      ("benchmark,b", po::bool_switch(&launch_benchmark), "")
      ;

    po::options_description log_options("Logging");
    log_options.add_options()
			("log-level,v", po::value<unsigned int>(&level)->default_value(AQ_LOG_WARNING), "CRITICAL(2), ERROR(3), WARNING(4), NOTICE(5), INFO(6), DEBUG(7)")
			("log-lock", po::bool_switch(&lock_mode), "for multithread program")
			("log-date", po::bool_switch(&date_mode), "add date to log")
			("log-pid", po::bool_switch(&pid_mode), "add thread id to log")
			("log-ident", po::value<std::string>(&ident)->default_value("aq_query_resolver"), "")
			;

    po::options_description algoquest("AlgoQuest");
    algoquest.add_options()
      ("aq-home,r", po::value<std::string>(&opt->aq_path)->default_value(opt->aq_path), "set AQ Home (AQ_HOME environment variable)")
      ("aq-name,n", po::value<std::string>(&opt->aq_name)->default_value(opt->aq_name), "")
      ("aq-engine,e", po::value<std::string>(&settings.aqEngine))
			("worker,w", po::value<size_t>(&settings.process_thread), "number of thread assigned to resolve the bunch of sql queries")
			("paralell,p", po::value<size_t>(&settings.process_thread)->default_value(settings.process_thread), "number of thread assigned resolve one sql queries")
      ;

    po::options_description mysql("MySQL");
    mysql.add_options()
      ("mysql-host", po::value<std::string>(&opt->mysql_host)->default_value(opt->mysql_host), "")
      ("mysql-user", po::value<std::string>(&opt->mysql_user)->default_value(opt->mysql_user), "")
      ("mysql-pass", po::value<std::string>(&opt->mysql_pass)->default_value(opt->mysql_pass), "")
			("mysql-name", po::value<std::string>(&opt->mysql_name)->default_value(opt->mysql_name), "")
      ;


    po::positional_options_description positionalOptions; 
    positionalOptions.add("aq-name", -1); 

    all.add(log_options).add(algoquest).add(mysql);

		po::variables_map vm;
		po::store(po::command_line_parser(argc, argv).options(all).positional(positionalOptions).run(), vm);
		po::notify(vm);    

		if (vm.count("help"))
		{
			os << all << "\n";
		}
  }
  catch (const boost::program_options::error& error)
  {
    os << error.what();
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}

void aqGui::benchmark()
{
  //// algoquest options
  //opt->aq_name = "algoquest_tmp";
  //opt->aq_path = "E:/AQ_DB/";

  //// mysql options
  //opt->mysql_host = "localhost";
  //opt->mysql_name = "algoquest";
  //opt->mysql_user = "tma";
  //opt->mysql_pass = "AlgoQuest";

  //// generation options
  //opt->generator_filename = "queries.gen";
  //opt->max_value = 1100;
  //opt->min_value = 1000;
  //opt->nb_rows = 10;
  //opt->nb_tables = 2;
  //opt->point_mode = DatabaseGenerator::point_mode_t::MIN_MAX;
  //opt->gen_mode = DatabaseGenerator::gen_mode_t::INTERSECT;
  //opt->value_mode = DatabaseGenerator::value_mode_t::RANDOM;

  wxFrame * frame = new wxFrame(nullptr, wxID_ANY, _("AlgoQuest Benchmark"), wxDefaultPosition, wxSize(1080, 720));
  wxBoxSizer * box = new wxBoxSizer(wxVERTICAL);
  aqTestDatabases * testDB = new aqTestDatabases(frame, opt);
  box->Add(testDB, 1, wxEXPAND, 0);
  frame->SetSizer(box);
  frame->SetIcon(wxIcon("AQ.ico", wxBITMAP_TYPE_ICO));
  frame->Show();

  SetTopWindow(frame);
}