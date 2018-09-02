#include "mainwindow.h"
#include <QApplication>

int main(int argc, char *argv[])
{
    QCoreApplication::setOrganizationName("RobinOlejnik.de");
    QCoreApplication::setOrganizationDomain("robinolejnik.de");
    QCoreApplication::setApplicationName("JugendfeuerwehrLeitstelle");
    QCoreApplication::setApplicationVersion("1");

    QApplication a(argc, argv);
    MainWindow w;
    w.show();

    return a.exec();
}
