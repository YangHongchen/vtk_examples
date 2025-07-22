#ifndef VTKSTLPREVIEWGENERATOR_H
#define VTKSTLPREVIEWGENERATOR_H

#include <QObject>

/**
 * @brief The Vtk Stl 文件缩略图生成
 */
class VtkStlPreviewGenerator : public QObject {
    Q_OBJECT
  public:
    explicit VtkStlPreviewGenerator (QObject* parent = nullptr);

    /**
     * @brief 启动生成，内部实现调用VTK流程
     * @param stlFilePath stl文件路径
     * @param outputDir   输出缩略图路径
     * @param imageSize   生成普片分辨率（例如1024，长宽均为1024）
     * @param stlTye      生成的stl模型文件类型（1.上颌，2下颌，3上牙列，4下牙列）
     */
    Q_INVOKABLE void startGenerate (const QString& stlFilePath, const QString& outputDir, int imageSize = 1024,
                                    int stlTye = 0);

  signals:
    // 成功信号，返回完整图片路径
    void thumbnailReady (const QString& thumbnailFullPath, int stlTye);
    // 失败信号，返回错误信息
    void generationFailed (const QString& errorString);

  private:
    bool generatePreview (const std::string& stlFilePath, const std::string& outputImagePath, int imageSize);

};

#endif // VTKSTLPREVIEWGENERATOR_H
