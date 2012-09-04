#pragma once

// racsqt
#include "ui_mainwindow.h"
// Qt
#include <QtGui/QMainWindow>
// boost
#include <boost/thread.hpp>
#include <boost/filesystem/path.hpp>


/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A
class MyPenLogAnalyzerMain : public QMainWindow, private Ui::MainWindow
{
    Q_OBJECT;
public:
    MyPenLogAnalyzerMain();
    virtual ~MyPenLogAnalyzerMain();

private slots:
    void load_images();

private:
    void load_image(const boost::filesystem::path& logfile, QLabel* pCntrl, QPixmap& pixmap);

    QPixmap pixmap_[3];
};
/////////1/////////2/////////3/////////4/////////5/////////6/////////7/////////8/////////9/////////A


