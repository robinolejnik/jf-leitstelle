#ifndef TOUCHVIEW_H
#define TOUCHVIEW_H

#include <QWidget>
#include <QTimer>
#include <QTime>
#include <QDate>
#include <QSettings>
#include <QFile>
#include <QDir>
#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QTextStream>
#include <QDesktopServices>
#include <QFileSystemWatcher>
#include <QListWidgetItem>
#include <QProcess>
#include "einsatz.h"
#include "printer.h"
#include "durchsage.h"
#include "audiohandler.h"

namespace Ui {
    class TouchView;
}

class TouchView : public QWidget {
    Q_OBJECT

signals:
    void neuerEinsatz();
    void EinsatzDisponiert(unsigned int);

public:
    explicit TouchView(QWidget *parent = nullptr);
    void setEinsatz(Einsatz &e);
    void undisponierte_einsaetze();
    ~TouchView();

private slots:
    void on_toolButton_tel_recordings_clicked();
    void on_toolButton_melder_clicked();
    void on_toolButton_wachalarm_clicked();
    void showTime();
    void on_listWidget_tel_currentItemChanged(QListWidgetItem *, QListWidgetItem *);
    void on_pushButton_tel_play_clicked();
    void on_pushButton_tel_stop_clicked();
    void on_pushButton_tel_delete_clicked();
    void on_pushButton_tel_refresh_clicked();
    void on_pushButton_kurzwahl_1_clicked();
    void on_pushButton_kurzwahl_2_clicked();
    void on_pushButton_kurzwahl_3_clicked();
    void on_pushButton_kurzwahl_4_clicked();
    void on_toolButton_tel_abheben_clicked();
    void on_pushButton_tel_trennen_clicked();
    void on_pushButton_melder_senden_clicked();
    void on_pushButton_aufnahme_clicked();
    void on_toolButton_play_clicked();
    void on_toolButton_play_feuergong_clicked();
    void on_toolButton_play_rettungsgong_clicked();
    void on_toolButton_play_einzelfahrzeug_clicked();
    void on_toolButton_play_durchsage_clicked();
    void on_toolButton_feuergong_clicked();
    void on_toolButton_rettungsgong_clicked();
    void on_toolButton_einzelfahrzeug_clicked();
    void on_toolButton_durchsage_clicked();
    void on_pushButton_aufnahme_anhoeren_clicked();
    void on_toolButton_tel_clicked();
    void on_toolButton_leitstelle_clicked();
    void on_toolButton_play_feuergong_2_clicked();
    void on_toolButton_play_rettungsgong_2_clicked();
    void on_toolButton_play_einzelfahrzeug_2_clicked();
    void on_toolButton_play_durchsage_2_clicked();
    void on_pushButton_aufnahme_2_clicked();
    void on_pushButton_aufnahme_anhoeren_2_clicked();
    void on_pushButton_play_2_clicked();
    void on_toolButton_leitstelle_2_clicked();
    void on_toolButton_alarmieren_clicked();
    void on_pushButton_einzelausdruck_clicked();

public slots:
    void resetView();

private:
    Durchsage *durchsage;
    QTimer *timer;
    Einsatz einsatz;
    Ui::TouchView *ui;
    AudioHandler *audiohandler;
};

#endif // TOUCHVIEW_H
