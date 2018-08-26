#ifndef EINSATZFORM_H
#define EINSATZFORM_H

#include <QWidget>
#include <QList>
#include <QListWidgetItem>
#include <QFile>
#include <QTimer>
#include <QTime>
#include <QUdpSocket>
#include <QNetworkDatagram>
#include <QMessageBox>
#include <QTextStream>
#include <QJsonDocument>
#include <QJsonObject>
#include "einsatz.h"
#include "touchview.h"
#include "karte.h"
#include "dialogbma.h"

namespace Ui {
    class EinsatzForm;
}

class EinsatzForm : public QWidget {
    Q_OBJECT

signals:
    void takeMapScreenshot();
    void resetViews();

public:
    explicit EinsatzForm(QWidget *parent = 0);
    void setTouchView(TouchView *t);
    void setKarte(Karte *k);
    ~EinsatzForm();

public slots:
    void neuerEinsatz() { on_pushButton_reset_clicked(); }
    void EinsatzDisponiert(unsigned int einsatznummer);

private slots:
    void on_pushButton_save_clicked();
    void on_pushButton_delete_clicked();
    void on_pushButton_reset_clicked();
    void on_listWidget_itemClicked(QListWidgetItem *current);
    void tick();
    void readPendingDatagrams();

private:
    Ui::EinsatzForm *ui;
    QTimer *checktimer;
    QUdpSocket *socket;
    TouchView *touchview;
    Karte *karte;
};

#endif // EINSATZFORM_H
