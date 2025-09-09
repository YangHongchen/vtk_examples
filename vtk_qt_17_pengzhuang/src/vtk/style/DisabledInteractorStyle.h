#ifndef DISABLEDINTERACTORSTYLE_H
#define DISABLEDINTERACTORSTYLE_H

#include <vtkBuffer.h>
#include <vtkInteractorStyle.h>


class DisabledInteractorStyle: public vtkInteractorStyle {
  public:
    static DisabledInteractorStyle *New();
    vtkTypeMacro (DisabledInteractorStyle, vtkInteractorStyle);
};
vtkStandardNewMacro (DisabledInteractorStyle);

#endif // DISABLEDINTERACTORSTYLE_H
