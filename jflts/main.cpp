#include <QApplication>
#include <QSettings>
#include <QtDebug>
#include <stdio.h>
#include "einsatzform.h"
#include "touchview.h"

#include "karte.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("RobinOlejnik.de");
    QCoreApplication::setOrganizationDomain("robinolejnik.de");
    QCoreApplication::setApplicationName("JugendfeuerwehrLeitstelle");
    QCoreApplication::setApplicationVersion("1");

    QSettings settings("config/config.ini", QSettings::IniFormat);
    qInfo() << "build" << __DATE__ << __TIME__;

    QApplication a(argc, argv);

    TouchView t;
    Karte k;
    EinsatzForm e;
    e.setKarte(&k);
    e.setTouchView(&t);
    e.showMaximized();
    t.showMaximized();

    return a.exec();
}
