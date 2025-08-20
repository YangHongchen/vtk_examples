// #include "TcpClient.h"
// #include <QDebug>


// TcpClient::TcpClient (const QString &host, quint16 port, QObject *parent)
//     : QObject (parent), m_host (host), m_port (port), m_socket (new QTcpSocket (this))
// {

//     connect (m_socket, &QTcpSocket::connected, this, &TcpClient::onConnected);
//     connect (m_socket, &QTcpSocket::disconnected, this, &TcpClient::onDisconnected);
//     connect (m_socket, &QTcpSocket::readyRead, this, &TcpClient::onReadyRead);
//     connect (m_socket, QOverload<QAbstractSocket::SocketError>::of (&QTcpSocket::errorOccurred),
//              this, &TcpClient::onError);

//     m_heartbeatTimer.setInterval (HEARTBEAT_INTERVAL_MS);
//     connect (&m_heartbeatTimer, &QTimer::timeout, this, &TcpClient::onHeartbeatTimeout);

//     m_reconnectTimer.setInterval (3000);
//     m_reconnectTimer.setSingleShot (true);
//     connect (&m_reconnectTimer, &QTimer::timeout, this, &TcpClient::onReconnectTimeout);
// }

// void TcpClient::connectToServer()
// {
//     if (m_socket->state() == QAbstractSocket::ConnectedState)
//         return;

//     qDebug() << "[TcpClient] Connecting to" << m_host << ":" << m_port;
//     m_socket->connectToHost (m_host, m_port);
// }

// void TcpClient::disconnectFromServer()
// {
//     stopHeartbeat();
//     m_socket->disconnectFromHost();
// }

// void TcpClient::sendMessage (const QString &message)
// {
//     if (m_socket->state() == QAbstractSocket::ConnectedState)
//     {
//         m_socket->write (message.toUtf8());
//     }
// }

// void TcpClient::onConnected()
// {
//     qDebug() << "[TcpClient] Connected.";
//     emit connected();
//     startHeartbeat();
// }

// void TcpClient::onDisconnected()
// {
//     qDebug() << "[TcpClient] Disconnected.";
//     emit disconnected();
//     stopHeartbeat();
//     startReconnect();
// }

// void TcpClient::onReadyRead()
// {
//     const QString message = QString::fromUtf8 (m_socket->readAll());
//     qDebug() << "[TcpClient] Received:" << message;
//     emit messageReceived (message);
// }

// void TcpClient::onError (QAbstractSocket::SocketError socketError)
// {
//     Q_UNUSED (socketError);
//     QString errMsg = m_socket->errorString();
//     qWarning() << "[TcpClient] Error:" << errMsg;
//     emit errorOccurred (errMsg);
// }

// void TcpClient::onHeartbeatTimeout()
// {
//     if (m_socket->state() == QAbstractSocket::ConnectedState)
//     {
//         sendMessage (HEARTBEAT_MESSAGE);
//     }
// }

// void TcpClient::onReconnectTimeout()
// {
//     connectToServer();
// }

// void TcpClient::startHeartbeat()
// {
//     if (!m_heartbeatTimer.isActive())
//         m_heartbeatTimer.start();
// }

// void TcpClient::stopHeartbeat()
// {
//     m_heartbeatTimer.stop();
// }

// void TcpClient::startReconnect()
// {
//     if (!m_reconnectTimer.isActive())
//         m_reconnectTimer.start();
// }
