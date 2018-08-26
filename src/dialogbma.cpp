#include "dialogbma.h"
#include "ui_dialogbma.h"

DialogBMA::DialogBMA(QWidget *parent) : QDialog(parent), ui(new Ui::DialogBMA) {
    QProcess::execute("lautsprecher_leitstelle.exe");
    player = new QMediaPlayer();
    playlist = new QMediaPlaylist();
    playlist->setPlaybackMode(QMediaPlaylist::Loop);
    playlist->addMedia(QUrl("qrc:/bma.wav"));
    player->setPlaylist(playlist);
    ui->setupUi(this);
    player->play();
}

void DialogBMA::setEinsatz(Einsatz &e) {
    ui->label_strasse->setText(e.strasse + " " + e.hausnummer);
    ui->label_objekt->setText(e.objekt);
}
DialogBMA::~DialogBMA() {
    player->stop();
    delete playlist;
    delete player;
    delete ui;
}
