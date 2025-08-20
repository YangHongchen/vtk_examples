// #include <QVector>
// #include <QVector3D>
// #include <algorithm>
// #include <cmath>
// #include <iostream>
// #include <vtkPoints.h>

// // 计算中位数（辅助函数）
// double median (QVector<double> values)
// {
//     if (values.isEmpty()) return 0.0;
//     std::sort (values.begin(), values.end());
//     int n = values.size();
//     if (n % 2 == 0)
//     {
//         return (values[n / 2 - 1] + values[n / 2]) / 2.0;
//     }
//     else
//     {
//         return values[n / 2];
//     }
// }

// // 找出最稳定的点
// QVector3D findMostStablePoint (const QVector<QVector3D> &points)
// {
//     if (points.isEmpty()) return QVector3D();

//     // 1. 分别取出 X、Y、Z 的所有值
//     QVector<double> xs, ys, zs;
//     xs.reserve (points.size());
//     ys.reserve (points.size());
//     zs.reserve (points.size());

//     for (const auto& p : points)
//     {
//         xs.append (p.x());
//         ys.append (p.y());
//         zs.append (p.z());
//     }

//     // 2. 计算中位数点
//     QVector3D medianPoint (median (xs), median (ys), median (zs));

//     // 3. 找到距离中位数点最近的实际测量点
//     double minDist = std::numeric_limits<double>::max();
//     QVector3D bestPoint;
//     for (const auto& p : points)
//     {
//         double dist = std::sqrt (
//                           std::pow (p.x() - medianPoint.x(), 2) +
//                           std::pow (p.y() - medianPoint.y(), 2) +
//                           std::pow (p.z() - medianPoint.z(), 2)
//                       );
//         if (dist < minDist)
//         {
//             minDist = dist;
//             bestPoint = p;
//         }
//     }

//     return bestPoint;
// }

// // 示例使用
// int main()
// {
//     QVector<QVector3D> measurements =
//     {
//         {0.00138819926092, 0.08749532934669, 0.00021803043413},
//         {0.00203139454673, 0.08841870536497, 0.00009885114685},
//         {0.00145809864539, 0.08780470149545, 0.00016202961842},
//         {0.0028403138243,  0.08948824942988, 0.00000261444907},
//         {0.00210799513794, 0.08831828872676, 0.00016994568686},
//         {0.00251940764615, 0.08857021206525, -0.00016591919565},
//         {0.00257517830254, 0.08852266727824, -0.00023192824228},
//         {0.00268777595785, 0.08878001173959, 0.00019228803319},
//         {0.00623646783417, 0.09100040691479, 0.00012693248101},
//         {0.00635973620882, 0.09090746845115, -0.00135825903767},
//         {0.00444625878914, 0.09036391095363, -0.00003773842408},
//         {-0.00118573461814, 0.08577647238713, 0.0001079757574},
//         {-0.00132598427264, 0.08527593267688, 0.00005780769423},
//         {-0.00145118668159, 0.0851979772253, 0.00007120212911}
//     };

//     QVector3D best = findMostStablePoint (measurements);
//     std::cout << "最稳定点: "
//               << best.x() << ", "
//               << best.y() << ", "
//               << best.z() << std::endl;

//     return 0;
// }
