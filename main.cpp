#include "mainwindow.h"
#include "programmationediteur.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    ProgrammationEditeur ae(ProgrammationManager::getInstance());
    ae.show();

    return a.exec();
}
