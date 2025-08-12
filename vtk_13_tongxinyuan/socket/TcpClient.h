#ifndef TCPCLIENT_H
#define TCPCLIENT_H

#include <QAbstractSocket>
#include <QObject>
#include <QTcpSocket>
#include <QTimer>

class TcpClient : public QObject {
    Q_OBJECT
  public:
    explicit TcpClient (const QString &host, quint16 port, QObject *parent = nullptr);
    void connectToServer();
    void disconnectFromServer();
    void sendMessage (const QString &message);

  signals:
    void connected();
    void disconnected();
    void messageReceived (const QString &message);
    void errorOccurred (const QString &error);

  private slots:
    void onConnected();
    void onDisconnected();
    void onReadyRead();
    void onError (QAbstractSocket::SocketError socketError);
    void onHeartbeatTimeout();
    void onReconnectTimeout();

  private:
    void startHeartbeat();
    void stopHeartbeat();
    void startReconnect();

  private:
    QTcpSocket *m_socket;
    QString m_host;
    quint16 m_port;

    QTimer m_heartbeatTimer;
    QTimer m_reconnectTimer;

    const int HEARTBEAT_INTERVAL_MS = 5000;
    const QString HEARTBEAT_MESSAGE = "PING";
};

#endif // TCPCLIENT_H
