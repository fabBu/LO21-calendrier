#include <QApplication>
#include <QMainWindow>
#include <QDebug>


#include <iostream>
#include "mainwindow.h"

int main(int argc, char* argv[])
{

    QApplication app(argc, argv);

    try
    {
        MainWindow *mw = new MainWindow();
        mw->show();
    }
    catch(CalendarException e)
    {
        qDebug()<<(e.getInfo().toStdString().c_str());
    }



    return app.exec();

}
