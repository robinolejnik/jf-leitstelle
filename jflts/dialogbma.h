#ifndef DIALOGBMA_H
#define DIALOGBMA_H

#include <QDialog>
#include <QProcess>
#include <QMediaPlayer>
#include "einsatz.h"

namespace Ui {
    class DialogBMA;
}

class DialogBMA : public QDialog {
    Q_OBJECT

public:
    explicit DialogBMA(QWidget *parent = nullptr);
    void setEinsatz(Einsatz &e);

private:
    Ui::DialogBMA *ui;
    QMediaPlayer *m_player;
};

#endif // DIALOGBMA_H
