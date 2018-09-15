#ifndef KARTE_H
#define KARTE_H

#include <QWidget>
#include <QQuickItem>
#include <QQmlContext>
#include <QVariant>
#include <QtQuickWidgets/QQuickWidget>
#include "einsatz.h"

class Karte : public QWidget {
    Q_OBJECT

signals:
    void updateGeoData(QVariant);
    void signalResetMap();

public:
    explicit Karte(QWidget *parent = nullptr);
    void setEinsatz(Einsatz &e);
    ~Karte();

public slots:
    void takeScreenshot();
    void resetMap();

private:
    QQuickWidget *view;
};

#endif // KARTE_H
