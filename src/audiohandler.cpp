#include "audiohandler.h"

AudioHandler::AudioHandler(QObject *parent) : QObject(parent)
{

}

void AudioHandler::handleAudioOutput(OutputDevice output, bool clearBefore, bool autoPlay) {
    getDeviceStringFromDevice(output);
}

QString AudioHandler::getDeviceStringFromDevice(OutputDevice device) {
    QSettings settings;
    switch(device) {
    case AusgangFunk:
        return settings.value("funk_ausgang").toString();
     case LautsprecherHeadset:
        return settings.value("headset_lautsprecher").toString();
     case LautsprecherHoerer:
        return settings.value("hoerer_lautsprecher").toString();
     case LautsprecherLeitstelle:
        return settings.value("lautsprecher_leitstelle").toString();
     case LautsprecherWache:
        return settings.value("lautsprecher_wache").toString();
    }
}

QString AudioHandler::getDeviceStringFromDevice(InputDevice device) {
    QSettings settings;
    switch(device) {
    case EingangFunk:
        return settings.value("funk_eingang").toString();
    case MikrofonHeadset:
        return settings.value("headset_mikrofon").toString();
    case MikrofonHoerer:
        return settings.value("hoerer_mikrofon").toString();
    }
}
