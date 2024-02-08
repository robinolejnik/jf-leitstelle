#ifndef PAGINGSYSTEM_H
#define PAGINGSYSTEM_H

#include <QtWebSockets/QWebSocket>
#include <QTimer>
#include <QObject>

class PagingSystem : public QObject {
    Q_OBJECT
public:
    explicit PagingSystem(QObject *parent = nullptr);

    void startPage(int ric, QString text, uint8_t function = 0);

private slots:
    void sendIdleFrame();
    void onConnected();
    void onDisconnected();
    void onTextMessageReceived(QString message);

private:
    QUrl m_url;
    QWebSocket m_webSocket;
    QTimer* m_idleFrameTimer;

signals:
    void pagingTransmitterConnected();
    void pagingTransmitterDisconnected();
};

#endif // PAGINGSYSTEM_H
