
// #include <QApplication>
// #include <QPdfWriter>
// #include <QPainter>
// #include <QDate>
// #include <QImage>

// class ReportGenerator {
//   public:
//     ReportGenerator (const QString &filePath)
//         : pdf (filePath)
//     {
//         pdf.setPageSize (QPageSize (QPageSize::A4));
//         pdf.setResolution (300);

//         pageWidth = pdf.width();
//         pageHeight = pdf.height();


//         m_painter = new QPainter (&pdf);
//     }

//     void drawHeader (const QString &title, const QString &logoPath)
//     {
//         m_painter->setFont (QFont ("Arial", 24, QFont::Bold));
//         m_painter->drawText (QRect (50, 0, pdf.width(), 100), Qt::AlignLeft, title);

//         QImage logo (logoPath);
//         int logoH = HEADER_HEIGHT;
//         int logoW = logo.width() * logoH / logo.height(); // 按比例缩放
//         m_painter->drawImage (QRect (pageWidth - logoW - MARGIN, 0, logoW, logoH), logo);

//         drawLine (HEADER_HEIGHT + 50, 6);
//         currentY = HEADER_HEIGHT + 150;
//     }

//     void drawPatientInfo (const QString &name, const QString &birth, const QString &gender, const QString &type)
//     {
//         m_painter->setFont (QFont ("Arial", 12));
//         m_painter->drawText (MARGIN, currentY, QString ("患者姓名: %1，%2，%3").arg (name, birth, gender));
//         currentY += LINE_HEIGHT;
//         m_painter->drawText (MARGIN, currentY, "检测日期: " + QDate::currentDate().toString ("yyyy-MM-dd"));
//         currentY += LINE_HEIGHT;
//         m_painter->drawText (MARGIN, currentY, "检测类型: " + type);
//         currentY += LINE_HEIGHT + 30;

//         drawLine (currentY, 2);
//         currentY += 50;
//     }

//     void drawChartSection (const QString &sectionTitle, const QString &leftImagePath, const QString &rightImagePath)
//     {
//         m_painter->setFont (QFont ("Arial", 14, QFont::Bold));
//         m_painter->drawText (QRect (MARGIN, currentY, pageWidth - 2 * MARGIN, 50), Qt::AlignCenter, sectionTitle);
//         currentY += 80;

//         QImage left (leftImagePath);
//         QImage right (rightImagePath);

//         // 每张图横向占一半（减去间距）
//         int availableWidth = (pageWidth - 3 * MARGIN) / 2;
//         int leftH = availableWidth * left.height() / left.width();   // 按原比例缩放
//         int rightH = availableWidth * right.height() / right.width();
//         int imgH = qMax (leftH, rightH); // 匹配最高的图，保证对齐

//         // 检查是否需要分页
//         checkNewPage (imgH + 30);

//         m_painter->drawImage (QRect (MARGIN, currentY, availableWidth, leftH), left);
//         m_painter->drawImage (QRect (MARGIN * 2 + availableWidth, currentY, availableWidth, rightH), right);

//         currentY += imgH + 30;
//         drawLine (currentY, 1);
//         currentY += 50;
//     }

//     void drawFourChartSection (const QString &sectionTitle, const QStringList &imagePaths)
//     {
//         if (imagePaths.size() != 4) return;

//         // 标题
//         m_painter->setFont (QFont ("Arial", 14, QFont::Bold));
//         m_painter->drawText (QRect (MARGIN, currentY, pageWidth - 2 * MARGIN, 50),  Qt::AlignCenter, sectionTitle);
//         currentY += 80;

//         // 计算单张图的宽度
//         int availableWidth = (pageWidth - (5 * MARGIN)) / 4;

//         QList<QImage> images;
//         QList<int> heights;
//         for (const auto &path : imagePaths)
//         {
//             QImage img (path);
//             images << img;
//             heights << (img.isNull() ? 0 : availableWidth * img.height() / img.width());
//         }

//         int imgH = *std::max_element (heights.begin(), heights.end()); // 行高 = 最大高度

//         // 检查是否需要分页
//         checkNewPage (imgH + 30);

//         // 绘制四张图
//         for (int i = 0; i < 4; ++i)
//         {
//             int x = MARGIN + i * (availableWidth + MARGIN);
//             m_painter->drawImage (QRect (x, currentY, availableWidth, heights[i]), images[i]);
//         }

//         currentY += imgH + 30;
//         drawLine (currentY, 1);
//         currentY += 50;
//     }


//     // 自动分页
//     void checkNewPage (int neededHeight)
//     {
//         if (currentY + neededHeight > pageHeight - MARGIN)
//         {
//             pdf.newPage();
//             currentY = MARGIN;
//         }
//     }

//     void end() { m_painter->end(); }

//   private:
//     void drawLine (int y, int thickness)
//     {
//         QPen pen (QColor ("#1F5F9E"));
//         pen.setWidth (thickness);
//         m_painter->setPen (pen);
//         m_painter->drawLine (MARGIN, y, pageWidth - MARGIN, y);
//         m_painter->setPen (Qt::black);
//     }

//   private:
//     QPdfWriter pdf;
//     QPainter *m_painter;
//     int pageWidth;
//     int pageHeight;
//     int currentY = 0;

//     static constexpr int MARGIN = 50;
//     static constexpr int HEADER_HEIGHT = 100;
//     static constexpr int LINE_HEIGHT = 80;
// };

// void generateReport()
// {
//     ReportGenerator report ("report.pdf");

//     report.drawHeader ("SJTU.口腔检测报告", "logo.sjtu.extend.png");
//     report.drawPatientInfo ("张三", "1995/12/27", "男", "咬合分析 和 运动报告");
//     report.drawChartSection ("矢状向 咬合运动轨迹", "chat1.png", "chat2.png");
//     report.drawChartSection ("矢状向 前伸运动轨迹", "chat3.png", "chat4.png");

//     report.drawFourChartSection ("侧方运动（头顶视角）的髁突运动分析",
//     {
//         "chat5.png", "chat6.png", "chat7.png", "chat8.png"
//     });

//     report.drawFourChartSection ("颌架的均值设置（髁导斜度）",
//     {
//         "chat9.png", "chat10.png", "chat11.png", "chat12.png"
//     });
//     report.end();
// }

// int main_ReportGenerator (int argc, char **argv)
// {
//     QApplication app (argc, argv);
//     generateReport();
//     return app.exec();
// }


