#include <QMainWindow>
#include <QQuickWidget>
#include "vtk/OcclusionWidget.h"
#include "chart/CondyleTrackWidget.h"



class TcpClient;
class TrackPlotter;

class MainWindow : public QMainWindow {
    Q_OBJECT

  public:
    explicit MainWindow (QWidget *parent = nullptr);
    ~MainWindow();

  private:
    void initSocketClient();

  public slots:
    void handleMesurDataUpdated (const QString & msg);

  private:
    QQuickWidget *m_qmlWidget;
    TcpClient *m_tcpClient = nullptr;
    OcclusionWidget *m_occlusionWidget = nullptr;
    CondyleTrackWidget *m_condyleTrackWidget = nullptr;

    // vtk形式绘制曲线图
    TrackPlotter *m_trackPlotter;
};
