// // main.cpp
// #include <QApplication>
// #include "MainWindow.h"
// #include "chart/TrackPlotter.h"


// int main2 (int argc, char **argv)
// {
//     // qputenv ("QSG_RHI_BACKEND", "opengl");
//     QApplication app (argc, argv);
//     // // 在 main.cpp  中最早调用的位置添加：

//     // MainWindow w;
//     // w.showMaximized();


//     TrackPlotter plotter;

//     // 模拟右髁突轨迹
//     std::vector<std::pair<float, float>> rightTrack =
//     {
//         {0, 0}, {1, -0.5}, {2, -2}, {4, -3.5}, {6, -5}, {7, -4.5}
//     };

//     // 模拟左髁突轨迹
//     std::vector<std::pair<float, float>> leftTrack =
//     {
//         {0, 0}, {-1, -0.5}, {-2, -2}, {-4, -3.5}, {-6, -5}, {-7, -4.5}
//     };

//     plotter.setRightTrack (rightTrack);
//     plotter.setLeftTrack (leftTrack);
//     plotter.show();

//     return app.exec();
// }
