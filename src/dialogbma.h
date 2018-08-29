#ifndef DIALOGBMA_H
#define DIALOGBMA_H

#include <QDialog>
#include <QProcess>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include "einsatz.h"

namespace Ui {
    class DialogBMA;
}

class DialogBMA : public QDialog {
    Q_OBJECT

public:
    explicit DialogBMA(QWidget *parent = nullptr);
    void setEinsatz(Einsatz &e);
    ~DialogBMA();

private:
    Ui::DialogBMA *ui;
    QMediaPlayer *player;
    QMediaPlaylist *playlist;
};

#endif // DIALOGBMA_H
