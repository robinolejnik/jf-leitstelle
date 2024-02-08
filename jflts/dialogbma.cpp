#include "dialogbma.h"
#include "ui_dialogbma.h"
#include <QTimer>

DialogBMA::DialogBMA(QWidget *parent) : QDialog(parent), ui(new Ui::DialogBMA) {
    QProcess::execute("lautsprecher_leitstelle.exe");

    m_player = new QMediaPlayer(this);
    m_player->setMedia(QUrl("qrc:/bma.wav"));

    connect(m_player, &QMediaPlayer::stateChanged, this, [this](QMediaPlayer::State state) {
        if(state==QMediaPlayer::State::StoppedState) {
            m_player->setMedia(QUrl());
            m_player->setMedia(QUrl("qrc:/bma.wav"));
            QTimer::singleShot(5000, m_player, SLOT(play()));
        }
    });

    ui->setupUi(this);
    m_player->play();
}

void DialogBMA::setEinsatz(Einsatz &e) {
    ui->label_strasse->setText(e.strasse + " " + e.hausnummer);
    ui->label_objekt->setText(e.objekt);
}
