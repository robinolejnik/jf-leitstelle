#include <QApplication>
#include <QSettings>
#include <QtDebug>
#include <stdio.h>
#include "einsatzform.h"
#include "touchview.h"

#include "karte.h"

int main(int argc, char *argv[]) {
    QCoreApplication::setOrganizationName("RobinOlejnik.com");
    QCoreApplication::setOrganizationDomain("robinolejnik.com");
    QCoreApplication::setApplicationName("JugendfeuerwehrLeitstelle");
    QCoreApplication::setApplicationVersion("1.1.0");

    QSettings settings("config/config.ini", QSettings::IniFormat);
    qInfo() << "RobinOlejnik.com Jugendfeuerwehr Leitstelle";
    qInfo() << "build" << __DATE__ << __TIME__;
    qInfo() << "Settings file:" << settings.fileName();

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
