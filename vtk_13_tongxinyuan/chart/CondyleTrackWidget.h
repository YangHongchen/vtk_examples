#include <QWidget>
#include <QPainter>
#include <QPixmap>

class CondyleTrackWidget : public QWidget {
  public:
    CondyleTrackWidget (QWidget *parent = nullptr) : QWidget (parent)
    {
        // 示例数据
        m_rightTrack = { {0, 0}, {1, -1}, {3, -3}, {6, -4} };
        m_leftTrack  = { {0, 0}, {-1, -1}, {-3, -3}, {-6, -4} };
    }

  protected:
    void paintEvent (QPaintEvent *) override
    {
        QPainter p (this);
        p.setRenderHint (QPainter::Antialiasing);

        int w = width() / 2;
        drawTrack (p, QRect (0, 0, w, height()), m_rightTrack, Qt::green, "Sag. Condyle Opening, right", ":/head_right.png");
        drawTrack (p, QRect (w, 0, w, height()), m_leftTrack, Qt::red, "Sag. Condyle Opening, left", ":/head_left.png");
    }

  private:
    QVector<QPointF> m_rightTrack;
    QVector<QPointF> m_leftTrack;

    void drawTrack (QPainter &p, QRect area, const QVector<QPointF> &points, QColor color, QString title, QString imgPath)
    {
        p.save();
        p.translate (area.topLeft());

        // 背景
        p.fillRect (0, 0, area.width(), area.height(), Qt::white);
        p.setPen (Qt::lightGray);
        for (int x = 0; x < area.width(); x += 40)
            p.drawLine (x, 0, x, area.height());
        for (int y = 0; y < area.height(); y += 40)
            p.drawLine (0, y, area.width(), y);

        // 标题
        p.setPen (Qt::black);
        p.drawText (10, 20, title);

        // 小头像
        QPixmap head (imgPath);
        p.drawPixmap (area.width() - 40, 10, 30, 30, head);

        // 坐标转换（假设 1 mm = 10 像素，原点在左上角）
        p.translate (area.width() / 2, area.height() / 2);
        p.scale (10, -10);

        // 画轨迹
        p.setPen (QPen (color, 0));
        for (int i = 1; i < points.size(); ++i)
            p.drawLine (points[i - 1], points[i]);

        p.restore();
    }
};
