
// racsqt
#include "mainwindow.h"
// Qt
#include <QtGui/QCheckBox>
#include <QtGui/QPlainTextEdit>
#include <QMessageBox>
//boost
#include <boost/foreach.hpp>
#include <boost/filesystem.hpp>
#include <boost/filesystem/fstream.hpp>
#include <boost/range/algorithm.hpp>
#include <boost/lexical_cast.hpp>
#include <boost/bind.hpp>
#include <boost/spirit/include/classic_core.hpp>
#include <boost/spirit/include/classic_attribute.hpp>
#include <boost/spirit/include/classic_symbols.hpp>
#include <boost/spirit/include/phoenix1_actor.hpp>
#include <boost/spirit/include/phoenix1_statements.hpp>
#include <boost/spirit/home/classic/utility/loops.hpp>
#include <boost/spirit/include/classic_file_iterator.hpp>
// std lib
#include <iostream>

using namespace boost::spirit::classic;
using namespace phoenix;

namespace bfs  = boost::filesystem;
namespace brng = boost::range;

/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
MyPenLogAnalyzerMain::MyPenLogAnalyzerMain()
    : QMainWindow()
{
	setupUi(this);
    setCentralWidget(centralwidget);

    load_images();

    connect(spinHeight,   SIGNAL(valueChanged(int)),  this, SLOT(load_images()));
    connect(spinWidth,    SIGNAL(valueChanged(int)),  this, SLOT(load_images()));
    connect(spinHdr,      SIGNAL(valueChanged(int)),  this, SLOT(load_images()));
    connect(cbContinuous, SIGNAL(toggled(bool)),      this, SLOT(load_images()));

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
MyPenLogAnalyzerMain::~MyPenLogAnalyzerMain()
{

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void MyPenLogAnalyzerMain::load_images()
{
    const bfs::path logdir(bfs::path(__FILE__).parent_path().parent_path().parent_path() / "logs");
    assert(bfs::exists(logdir));

    load_image(logdir / "line_parallel.out", img1, pixmap_[0]);
    load_image(logdir / "line_perp.out",     img2, pixmap_[1]);
    load_image(logdir / "refnr.out",         img3, pixmap_[2]);
    load_image(logdir / "squares.out",       img4, pixmap_[2]);
}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
struct bulk_end
{
    bulk_end(std::vector<uint8_t>& values, std::vector<uint8_t>& tmpval, const bool conti, const size_t blksize, const bool logging)
        : values_(values), tmpval_(tmpval), continuous_(conti), blksize_(blksize), logging_(logging) { }

    template<class iterT>
    void operator()(iterT begin, iterT end) const
    {
		if(values_.empty() && tmpval_.size() < 20)
			return;
        if(logging_)
            std::cout << "bulk with " << tmpval_.size() << std::endl;
		if(continuous_)
            brng::copy(tmpval_, std::back_inserter(values_));
        else
        {
            while(tmpval_.size() < blksize_)
                tmpval_.push_back(0xFF);
            std::copy(tmpval_.begin(), tmpval_.begin() + blksize_, std::back_inserter(values_));
        }

		tmpval_.clear();
    }
private:
    std::vector<uint8_t>& values_;
	std::vector<uint8_t>& tmpval_;
	const bool            continuous_;
	const size_t          blksize_;
	const bool            logging_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
void MyPenLogAnalyzerMain::load_image(const bfs::path& logfile, QLabel* pCntrl, QPixmap& pixmap)
{
    if(cbLog->isChecked())
        std::cout << "load_image " << logfile.string() << std::endl;

    std::vector<uint8_t> values, tmpval;

    file_iterator<> fbeg(logfile.string());
    file_iterator<> fend = fbeg.make_end();
    typedef file_iterator<char> iterator_t;
    typedef scanner<iterator_t>     scanner_t;
    typedef rule <scanner_t>        rule_t;

    //grammar for value extraction
    const size_t BYTES_PER_LINE = 16;
    rule_t datafirst  = space_p >> str_p("00000000:") >> *(space_p >> hex_p) >> eol_p;
	rule_t datasecond = space_p >> str_p("00000010:") >> repeat_p(spinHdr->value() - 16)[space_p >> hex_p] >> *(space_p >> hex_p[push_back_a(tmpval)]) >> eol_p;
	rule_t datasucc   = space_p >> hex_p >> ":" >> +(space_p >> hex_p[push_back_a(tmpval)]) >> eol_p;
    rule_t bulkread   = (datafirst >> datasecond >> *datasucc)[bulk_end(values, tmpval, cbContinuous->isChecked(), spinHeight->value(), cbLog->isChecked())];
    rule_t skipline   = *print_p >> eol_p;
    rule_t dumpfile   = +(bulkread || skipline);



    parse_info<iterator_t> info = parse(fbeg, fend, dumpfile);



    std::stringstream sstr;
    sstr << values.size() << " values read.";
    statustext->setText(sstr.str().c_str());

    QByteArray ba;
    // Header
    sstr.str("");

	const size_t stepwid = spinWidth->value();
    const size_t height = spinHeight->value() / stepwid;
    const size_t width = values.size() / height;


    sstr << "P5 " << width * stepwid << " " << height / stepwid << " " << 255 << "\n";
    ba.append(sstr.str().c_str());


	for(size_t h=0; h<height; ++h)
	    for(size_t w=0; w<width; ++w)
		{
			const size_t slice = w / stepwid;
            ba.append(values[slice * stepwid * height + w % stepwid + h * stepwid]);
		}

    const bool goodLoad = pixmap.loadFromData(ba, "pgm");

	pixmap.save(bfs::change_extension(logfile, ".png").string().c_str());
    pCntrl->setPixmap(pixmap);

}
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A




