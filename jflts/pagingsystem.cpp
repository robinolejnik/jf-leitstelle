#include "pagingsystem.h"
#include <QJsonObject>
#include <QJsonDocument>
#include <QSettings>
#include <QtDebug>

PagingSystem::PagingSystem(QObject *parent) : QObject(parent) {
    connect(&m_webSocket, &QWebSocket::connected, this, &PagingSystem::onConnected);
    connect(&m_webSocket, &QWebSocket::disconnected, this, &PagingSystem::onDisconnected);

    QSettings settings("config/config.ini", QSettings::IniFormat);
    settings.beginGroup("paging");
    QString urlString = settings.value("unipager_url").toString();
    m_url = QUrl::fromUserInput(urlString);

    if(!m_url.isValid()) {
        return;
    }

    m_idleFrameTimer = new QTimer(this);

    qInfo() << "Connecting to paging transmitter on" << urlString;
    m_webSocket.open(m_url);
}

void PagingSystem::startPage(int ric, QString text, uint8_t function) {
    if(m_webSocket.isValid()) {
        QJsonObject message;
        message.insert("ric", ric);
        message.insert("speed", 1200);
        message.insert("type", "alphanum");
        message.insert("func", function);
        message.insert("data", text);

        QJsonObject sendMessage;
        sendMessage.insert("id", "test");
        sendMessage.insert("protocol", "pocsag");
        sendMessage.insert("priority", 1);
        sendMessage.insert("origin", "jf-leitstelle.local");
        sendMessage.insert("message", message);

        QJsonObject wsMessage;
        wsMessage.insert("SendMessage", sendMessage);

        QJsonDocument document;
        document.setObject(wsMessage);

        m_webSocket.sendTextMessage(document.toJson(QJsonDocument::Compact));
        qInfo() << "Starting page to ric" << ric << ":" << text;
    }
    else if(m_url.isValid()) {
        qWarning() << "Page failed: websocket invalid, trying to reconnect";
        m_webSocket.disconnect();
        QTimer::singleShot(1000, this, SLOT(onDisconnected()));
    }
}

void PagingSystem::sendIdleFrame() {
    if(m_webSocket.isValid()) {
        QJsonObject message;
        message.insert("ric", 1000);
        message.insert("speed", 1200);
        message.insert("type", "numeric");

        QJsonObject sendMessage;
        sendMessage.insert("id", "test");
        sendMessage.insert("protocol", "pocsag");
        sendMessage.insert("priority", 2);
        sendMessage.insert("origin", "jf-leitstelle.local");
        sendMessage.insert("message", message);

        QJsonObject wsMessage;
        wsMessage.insert("SendMessage", sendMessage);

        QJsonDocument document;
        document.setObject(wsMessage);

        m_webSocket.sendTextMessage(document.toJson(QJsonDocument::Compact));
    }
    else if(m_url.isValid()) {
        qWarning() << "Sending idle frame failed: websocket invalid, trying to reconnect";
        m_webSocket.disconnect();
        QTimer::singleShot(1000, this, SLOT(onDisconnected()));
    }
}

void PagingSystem::onConnected() {
    connect(&m_webSocket, &QWebSocket::textMessageReceived, this, &PagingSystem::onTextMessageReceived);

    QJsonObject authenticateRequest;
    authenticateRequest.insert("Authenticate", "");
    m_webSocket.sendTextMessage(QJsonDocument(authenticateRequest).toJson(QJsonDocument::Compact));

    m_idleFrameTimer->start(60000);
    connect(m_idleFrameTimer, SIGNAL(timeout()), this, SLOT(sendIdleFrame()));

    qInfo() << "websocket connected";
    emit pagingTransmitterConnected();
}

void PagingSystem::onDisconnected() {
    disconnect(&m_webSocket, &QWebSocket::textMessageReceived, this, &PagingSystem::onTextMessageReceived);

    m_idleFrameTimer->stop();
    disconnect(m_idleFrameTimer, SIGNAL(timeout()), this, SLOT(sendIdleFrame()));

    qWarning() << "websocket disconnected";
    emit pagingTransmitterDisconnected();

    if(!m_webSocket.isValid()) {
        m_webSocket.open(m_url);
    }
}

void PagingSystem::onTextMessageReceived(QString message) {
    Q_UNUSED(message)
}
