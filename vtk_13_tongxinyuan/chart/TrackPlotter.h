#ifndef TRACKPLOTTER_H
#define TRACKPLOTTER_H

#include <vector>
#include <utility>
#include <string>

#include <vtkSmartPointer.h>
#include <vtkChartXY.h>
#include <vtkContextView.h>
#include <vtkRenderWindow.h>
#include <vtkRenderer.h>

class TrackPlotter {
  public:
    TrackPlotter();
    ~TrackPlotter() = default;

    void setRightTrack (const std::vector<std::pair<float, float>>& track);
    void setLeftTrack (const std::vector<std::pair<float, float>> &track);

    void show();

  private:
    vtkSmartPointer<vtkChartXY> createTrackChart (
        const std::vector<std::pair<float, float>>& track,
        const std::string& title,
        double r, double g, double b);

    std::vector<std::pair<float, float>> smoothCurve (
        const std::vector<std::pair<float, float>>& points, int resolution);

  private:
    std::vector<std::pair<float, float>> m_rightTrack;
    std::vector<std::pair<float, float>> m_leftTrack;

    vtkSmartPointer<vtkContextView> m_viewRight;
    vtkSmartPointer<vtkContextView> m_viewLeft;
    vtkSmartPointer<vtkRenderWindow> m_renderWindow;
};

#endif // TRACKPLOTTER_H
