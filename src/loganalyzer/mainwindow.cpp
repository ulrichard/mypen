
// racsqt
#include "mainwindow.h"
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QPlainTextEdit>
#include <QMessageBox>
//boost
#include <boost/tuple/tuple.hpp>
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/timer.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/format.hpp>
#include <boost/regex.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/phoenix1_actor.hpp>
#include <boost/spirit/include/phoenix1_statements.hpp>
using namespace boost::spirit::classic;
using namespace phoenix;

namespace bfs  = boost::filesystem;
namespace brng = boost::range;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
RACSQTMain::RACSQTMain()
    : QMainWindow()
{
	setupUi(this);
    setCentralWidget(centralwidget);

    const bfs::path logdir(bfs::path(__FILE__).parent_path().parent_path().parent_path() / "logs");
    assert(bfs::exists(logdir));

    load_image(logdir / "line_parallel.out", img1);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
RACSQTMain::~RACSQTMain()
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void RACSQTMain::load_image(const bfs::path& logfile, QLabel* pCntrl)
{
    bfs::ifstream ifs(logfile);
    std::vector<uint8_t> values, tmpval;

    // regex for coarse filtering
    boost::regex regx("\\d\\d\\d\\d\\d\\d\\d\\d\\:.\\d\\d");
    boost::smatch regxmatch;

    //grammar for value extraction
    typedef rule<> rule_t;

	rule_t datafirst  = str_p("00000000:") >> *uint_p;
	rule_t datasucc   = uint_p >> ":" >> +uint_p[push_back_a(tmpval)];
    rule_t dataline   = datafirst || datasucc;

    while(!ifs.eof())
    {
        std::string line;
        std::getline(ifs, line);
        if(!boost::regex_search(line, regxmatch, regx))
            continue;

        parse_info<> info = parse(line.c_str(), dataline, space_p);
        if(!info.hit)
            continue;
        if(info.length < line.length() - 1)
            continue;

        brng::copy(tmpval, std::back_inserter(values));
        tmpval.clear();

    }

    std::stringstream sstr;
    sstr << values.size() << " values read.";
    statustext->setText(sstr.str().c_str());

    QByteArray ba;
    // Header
    sstr.str("");

    const size_t height = 64;
    const size_t width = values.size() / height;

    sstr << "P5 " << width << " " << height << " " << 255 << "\n";
    ba.append(sstr.str().c_str());

    BOOST_FOREACH(uint8_t vv, values)
        ba.append(vv);
    const bool goodLoad = pixmap_.loadFromData(ba, "pgm");


     pCntrl->setPixmap(pixmap_);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A




