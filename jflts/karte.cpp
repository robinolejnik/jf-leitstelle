#include "karte.h"

Karte::Karte(QWidget *parent) : QWidget(parent) {
    view = new QQuickWidget;
    view->setSource(QUrl("qrc:/map.qml"));
    view->setResizeMode(QQuickWidget::SizeRootObjectToView);
    view->showMaximized();
    view->setWindowTitle("Karte");
    QObject *obj = (QObject*)(view->rootObject());
    connect(this, SIGNAL(updateGeoData(QVariant)), obj, SLOT(updateGeoData(QVariant)));
    connect(this, SIGNAL(signalResetMap()), obj, SLOT(signalResetMap()));
}

void Karte::setEinsatz(Einsatz &e) {
    emit updateGeoData(QVariant(e.strasse + " " + e.hausnummer + " " + e.ort));
}

void Karte::resetMap() {
    emit signalResetMap();
}

void Karte::takeScreenshot() {
    view->grab().save("map.png");
}

Karte::~Karte() {

}
