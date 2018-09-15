#include "audiohandler.h"

AudioHandler::AudioHandler(QObject *parent) : QObject(parent) {
    player_funk = new QMediaPlayer(this);
    player_headset = new QMediaPlayer(this);
    player_hoerer = new QMediaPlayer(this);
    player_leitstelle = new QMediaPlayer(this);
    player_wache = new QMediaPlayer(this);

    playlist_funk = new QMediaPlaylist(this);
    playlist_headset = new QMediaPlaylist(this);
    playlist_hoerer = new QMediaPlaylist(this);
    playlist_leitstelle = new QMediaPlaylist(this);
    playlist_wache = new QMediaPlaylist(this);

    player_funk->setPlaylist(playlist_funk);
    player_headset->setPlaylist(playlist_headset);
    player_hoerer->setPlaylist(playlist_hoerer);
    player_leitstelle->setPlaylist(playlist_leitstelle);
    player_wache->setPlaylist(playlist_wache);

    out_funk = qobject_cast<QAudioOutputSelectorControl *>(player_funk->service()->requestControl(QAudioOutputSelectorControl_iid));
    out_headset = qobject_cast<QAudioOutputSelectorControl *>(player_headset->service()->requestControl(QAudioOutputSelectorControl_iid));
    out_hoerer = qobject_cast<QAudioOutputSelectorControl *>(player_hoerer->service()->requestControl(QAudioOutputSelectorControl_iid));
    out_leitstelle = qobject_cast<QAudioOutputSelectorControl *>(player_leitstelle->service()->requestControl(QAudioOutputSelectorControl_iid));
    out_wache = qobject_cast<QAudioOutputSelectorControl *>(player_wache->service()->requestControl(QAudioOutputSelectorControl_iid));

    QSettings settings;
    out_funk->setActiveOutput(settings.value("funk_ausgang").toString());
    out_headset->setActiveOutput(settings.value("headset_lautsprecher").toString());
    out_hoerer->setActiveOutput(settings.value("hoerer_lautsprecher").toString());
    out_leitstelle->setActiveOutput(settings.value("lautsprecher_leitstelle").toString());
    out_wache->setActiveOutput(settings.value("lautsprecher_wache").toString());

    player_funk->service()->releaseControl(out_funk);
    player_headset->service()->releaseControl(out_headset);
    player_hoerer->service()->releaseControl(out_hoerer);
    player_leitstelle->service()->releaseControl(out_leitstelle);
    player_wache->service()->releaseControl(out_wache);
}

QMediaPlayer *AudioHandler::getPlayerFromDevice(OutputDevice output) {
    QMediaPlayer *player = player_leitstelle;
    switch(output) {
    case AusgangFunk:
        player = player_funk;
        break;
    case LautsprecherHeadset:
        player = player_headset;
        break;
    case LautsprecherHoerer:
        player = player_hoerer;
        break;
    case LautsprecherLeitstelle:
        player = player_leitstelle;
        break;
    case LautsprecherWache:
        player = player_wache;
        break;
    }
    return player;
}

void AudioHandler::resetPlayer(OutputDevice output) {
    QMediaPlayer *player = getPlayerFromDevice(output);
    player->stop();
    player->playlist()->clear();
}

void AudioHandler::playFile(OutputDevice output, QUrl url, bool clearBefore, bool autoPlay) {
    QMediaPlayer *player = getPlayerFromDevice(output);

    if(clearBefore) {
        player->stop();
        player->playlist()->clear();
    }
    player->playlist()->addMedia(url);
    if(autoPlay) {
        player->play();
    }
}
