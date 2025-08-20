// #include "TrackPlotter.h"

// #include <vtkTable.h>
// #include <vtkFloatArray.h>
// #include <vtkPlot.h>
// #include <vtkAxis.h>
// #include <vtkPen.h>
// #include <vtkTextProperty.h>
// #include <vtkCardinalSpline.h>
// #include <vtkRenderWindowInteractor.h>

// // 构造函数
// TrackPlotter::TrackPlotter()
// {
//     m_viewRight = vtkSmartPointer<vtkContextView>::New();
//     m_viewLeft  = vtkSmartPointer<vtkContextView>::New();
//     m_renderWindow = vtkSmartPointer<vtkRenderWindow>::New();

//     m_viewRight->SetRenderWindow (m_renderWindow);
//     m_viewLeft->SetRenderWindow (m_renderWindow);

//     // 分屏显示
//     m_viewRight->GetRenderer()->SetViewport (0.0, 0.0, 0.5, 1.0);
//     m_viewLeft->GetRenderer()->SetViewport (0.5, 0.0, 1.0, 1.0);
// }

// void TrackPlotter::setRightTrack (const std::vector<std::pair<float, float>>& track)
// {
//     m_rightTrack = track;
// }

// void TrackPlotter::setLeftTrack (const std::vector<std::pair<float, float>> &track)
// {
//     m_leftTrack = track;
// }

// std::vector<std::pair<float, float>> TrackPlotter::smoothCurve (
//     const std::vector<std::pair<float, float>> &points, int resolution)
// {
//     if (points.size() < 3) return points;

//     auto splineX = vtkSmartPointer<vtkCardinalSpline>::New();
//     auto splineY = vtkSmartPointer<vtkCardinalSpline>::New();

//     for (int i = 0; i < static_cast<int> (points.size()); ++i)
//     {
//         splineX->AddPoint (i, points[i].first);
//         splineY->AddPoint (i, points[i].second);
//     }

//     std::vector<std::pair<float, float>> smoothPts;
//     int n = static_cast<int> (points.size());
//     for (double t = 0; t < n - 1; t += 1.0 / resolution)
//     {
//         double x = splineX->Evaluate (t);
//         double y = splineY->Evaluate (t);
//         smoothPts.emplace_back (static_cast<float> (x), static_cast<float> (y));
//     }
//     return smoothPts;
// }

// vtkSmartPointer<vtkChartXY> TrackPlotter::createTrackChart (
//     const std::vector<std::pair<float, float>>& track,
//     const std::string& title,
//     double r, double g, double b)
// {
//     auto smoothTrack = smoothCurve (track, 10);

//     auto table = vtkSmartPointer<vtkTable>::New();

//     auto arrX = vtkSmartPointer<vtkFloatArray>::New();
//     arrX->SetName ("X (mm)");
//     auto arrY = vtkSmartPointer<vtkFloatArray>::New();
//     arrY->SetName ("Y (mm)");

//     for (auto& p : smoothTrack)
//     {
//         arrX->InsertNextValue (p.first);
//         arrY->InsertNextValue (p.second);
//     }

//     table->AddColumn (arrX);
//     table->AddColumn (arrY);

//     auto chart = vtkSmartPointer<vtkChartXY>::New();
//     chart->GetAxis (vtkAxis::BOTTOM)->SetTitle ("mm");
//     chart->GetAxis (vtkAxis::LEFT)->SetTitle ("mm");
//     chart->GetAxis (vtkAxis::BOTTOM)->GetTitleProperties()->SetFontSize (12);
//     chart->GetAxis (vtkAxis::LEFT)->GetTitleProperties()->SetFontSize (12);

//     chart->SetTitle (title);
//     chart->GetTitleProperties()->SetFontSize (14);

//     // 绘制轨迹
//     vtkPlot* line = chart->AddPlot (vtkChart::LINE);
//     line->SetInputData (table, 0, 1);
//     line->SetColor (r, g, b);
//     line->SetWidth (2.0);

//     // 背景网格
//     chart->GetAxis (vtkAxis::BOTTOM)->SetGridVisible (true);
//     chart->GetAxis (vtkAxis::LEFT)->SetGridVisible (true);

//     // 固定范围
//     chart->GetAxis (vtkAxis::BOTTOM)->SetRange (-8, 16);
//     chart->GetAxis (vtkAxis::LEFT)->SetRange (-10, 2);

//     return chart;
// }

// void TrackPlotter::show()
// {
//     auto chartRight = createTrackChart (m_rightTrack, "Sag. Condyle Opening, right", 0.1, 0.7, 0.1);
//     auto chartLeft  = createTrackChart (m_leftTrack,  "Sag. Condyle Opening, left",  0.8, 0.1, 0.1);

//     m_viewRight->GetScene()->AddItem (chartRight);
//     m_viewLeft->GetScene()->AddItem (chartLeft);

//     m_renderWindow->SetSize (800, 400);
//     m_renderWindow->SetWindowName ("Condyle Tracks");
//     m_renderWindow->Render();
//     m_renderWindow->GetInteractor()->Start();
// }
