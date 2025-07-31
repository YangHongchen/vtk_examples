#ifndef CUSTOMINTERACTORSTYLE_H
#define CUSTOMINTERACTORSTYLE_H

#include <vtkBuffer.h>
#include <vtkInteractorStyleTrackballCamera.h>


class CustomInteractorStyle: public vtkInteractorStyleTrackballCamera  {
  public:
    static CustomInteractorStyle *New();
    vtkTypeMacro (CustomInteractorStyle, vtkInteractorStyleTrackballCamera);

    // 禁用缩放
    void OnMouseWheelForward() override {}
    void OnMouseWheelBackward() override {}

    // 禁用旋转和平移
    void OnMouseMove() override {}
    void OnLeftButtonDown() override {}
    void OnLeftButtonUp() override {}
    void OnMiddleButtonDown() override {}
    void OnMiddleButtonUp() override {}
    void OnRightButtonDown() override {}
    void OnRightButtonUp() override {}

};

vtkStandardNewMacro (CustomInteractorStyle);


#endif // CUSTOMINTERACTORSTYLE_H
