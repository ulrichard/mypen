#pragma once

// racsqt
#include "ui_mainwindow.h"
// Qt
#include <QtGui/QMainWindow>
// boost
#include <boost/thread.hpp>
#include <boost/filesystem/path.hpp>


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class RACSQTMain : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT;
public:
    RACSQTMain();
    virtual ~RACSQTMain();

private slots:

private:
    void load_image(const boost::filesystem::path& logfile, QLabel* pCntrl);

    QPixmap pixmap_;
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A


