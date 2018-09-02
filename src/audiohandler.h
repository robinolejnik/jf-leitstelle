#ifndef AUDIOHANDLER_H
#define AUDIOHANDLER_H

#include <QObject>
#include <QUrl>
#include <QMediaService>
#include <QMediaPlayer>
#include <QMediaPlaylist>
#include <QAudioOutputSelectorControl>
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
    void playFile(OutputDevice output, QUrl url, bool clearBefore = true, bool autoPlay = true);
    void resetPlayer(OutputDevice output);

private:
    QMediaPlayer *getPlayerFromDevice(OutputDevice output);

    QMediaPlayer *player_headset;
    QMediaPlayer *player_hoerer;
    QMediaPlayer *player_funk;
    QMediaPlayer *player_leitstelle;
    QMediaPlayer *player_wache;

    QMediaPlaylist *playlist_headset;
    QMediaPlaylist *playlist_hoerer;
    QMediaPlaylist *playlist_funk;
    QMediaPlaylist *playlist_leitstelle;
    QMediaPlaylist *playlist_wache;

    QAudioOutputSelectorControl *out_headset;
    QAudioOutputSelectorControl *out_hoerer;
    QAudioOutputSelectorControl *out_funk;
    QAudioOutputSelectorControl *out_leitstelle;
    QAudioOutputSelectorControl *out_wache;
};

#endif // AUDIOHANDLER_H
