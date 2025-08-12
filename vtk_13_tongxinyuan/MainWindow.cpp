#include "MainWindow.h"
#include <QQmlContext>
#include <QQuickWidget>
#include <QQuickItem>
#include <QJsonArray>
#include <QJsonParseError>
#include "socket/TcpClient.h"
#include "chart/TrackPlotter.h"

MainWindow::MainWindow (QWidget *parent) : QMainWindow{parent}
{
    // m_qmlWidget = new QQuickWidget (this);
    // m_qmlWidget->setResizeMode (QQuickWidget::SizeRootObjectToView);
    // m_qmlWidget->setSource (QUrl ("qrc:/assets/qml/Main.qml"));


    // m_occlusionWidget = new OcclusionWidget (this);
    // m_occlusionWidget->loadUpperModel ("c:/stl/1.stl");
    // m_occlusionWidget->loadLowerModel ("c:/stl/2.stl");


    // m_condyleTrackWidget = new CondyleTrackWidget (this);

    m_trackPlotter = new TrackPlotter();

    // QQuickItem *qmlRoot = m_qmlWidget->rootObject();
    // if (!qmlRoot)
    // {
    //     qCritical() << "QML根对象未创建！";
    //     return;
    // }

    // setCentralWidget (m_trackPlotter);

    // initSocketClient();

}

MainWindow::~MainWindow() {}

void MainWindow::initSocketClient()
{
    auto *client = new TcpClient ("127.0.0.1", 10086, this);
    connect (client, &TcpClient::connected, [] { qDebug() << "连接成功ooo"; });
    connect (client, &TcpClient::messageReceived, this, &MainWindow::handleMesurDataUpdated);
    connect (client, &TcpClient::errorOccurred, [] (const QString & err)
    {
        qWarning() << "错误:" << err;
    });
    client->connectToServer();
}

void MainWindow::handleMesurDataUpdated (const QString &msg)
{
    // 解析JSON字符串
    QJsonParseError parseError;
    QJsonDocument doc = QJsonDocument::fromJson (msg.toUtf8(), &parseError);
    if (parseError.error != QJsonParseError::NoError)
    {
        qWarning() << "JSON解析错误:" << parseError.errorString();
        return;
    }

    if (!doc.isObject())
    {
        qWarning() << "收到的数据不是JSON对象";
        return;
    }

    QJsonObject dataObj = doc.object();

    // 调用OcclusionWidget的updateFromData驱动咬合
    if (m_occlusionWidget)
    {
        m_occlusionWidget->updateFromData (dataObj);
    }
    else
    {
        qWarning() << "m_occlusionWidget为空，无法驱动更新";
    }

}
