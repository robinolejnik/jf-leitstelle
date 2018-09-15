#include "durchsage.h"

Durchsage::Durchsage(QObject *parent) : QObject(parent) {
    player = new QMediaPlayer();
    recorder = new QAudioRecorder();
    filename = QFileInfo(filename_rel).absoluteFilePath();
}

void Durchsage::record() {
    player->stop();
    QFile file(filename);
    file.open(QFile::Truncate | QFile::WriteOnly);
    delete recorder;
    recorder = new QAudioRecorder();
    recorder->setOutputLocation(QUrl::fromLocalFile(filename));
    player = new QMediaPlayer();
    recorder->record();
}

void Durchsage::stop() {
    recorder->stop();
    player->stop();
}

void Durchsage::play() {
    player->setMedia(QUrl::fromLocalFile(filename));
    player->play();
}

Durchsage::~Durchsage() {
    delete player;
    delete recorder;
}
