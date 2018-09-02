#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <QObject>
#include <QUrl>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QSettings>

class AudioHandler : public QObject {
    Q_OBJECT
public:
    enum OutputDevice {
        LautsprecherWache,
        LautsprecherLeitstelle,
        LautsprecherHeadset,
        LautsprecherHoerer,
        AusgangFunk
    };
    Q_ENUM(OutputDevice)

    enum InputDevice {
        MikrofonHeadset,
        MikrofonHoerer,
        EingangFunk
    };
    Q_ENUM(InputDevice)

    explicit AudioHandler(QObject *parent = nullptr);
    void handleAudioOutput(OutputDevice output, bool clearBefore = true, bool autoPlay = false);

signals:

public slots:

private:
    QString getDeviceStringFromDevice(OutputDevice device);
    QString getDeviceStringFromDevice(InputDevice device);
};

#endif // AUDIOHANDLER_H
