#ifndef DURCHSAGE_H
#define DURCHSAGE_H

#include <QObject>
#include <QUrl>
#include <QSettings>
#include <QFile>
#include <QFileInfo>
#include <QAudioRecorder>
#include <QMediaPlayer>

class Durchsage : public QObject {
    Q_OBJECT

private:
    QAudioRecorder *recorder;
    QMediaPlayer *player;
public:
    explicit Durchsage(QObject *parent = nullptr);
    void record();
    void stop();
    void play();

    const QString filename_rel = "temp.wav";
    QString filename;

    ~Durchsage();
};

#endif // DURCHSAGE_H
