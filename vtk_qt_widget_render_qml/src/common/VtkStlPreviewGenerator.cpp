#include "src/common/VtkStlPreviewGenerator.h"

#include <QObject>
#include <QString>
#include <QFileInfo>
#include <QDir>
#include <QThread>
#include <QDebug>

#include <vtkSmartPointer.h>
#include <vtkSTLReader.h>
#include <vtkPolyDataMapper.h>
#include <vtkPolyDataNormals.h>
#include <vtkActor.h>
#include <vtkRenderer.h>
#include <vtkRenderWindow.h>
#include <vtkWindowToImageFilter.h>
#include <vtkPNGWriter.h>
#include <vtkLight.h>
#include <vtkProperty.h>
#include <vtkPlaneSource.h>
#include <vtkTransform.h>
#include <vtkTransformPolyDataFilter.h>

#include "src/config/ConfigManager.h"

VtkStlPreviewGenerator::VtkStlPreviewGenerator (QObject *parent): QObject {parent}
{
    m_patientModel = PatientModel::instance();
}

VtkStlPreviewGenerator::~VtkStlPreviewGenerator()
{
    m_patientModel = nullptr;
}

void VtkStlPreviewGenerator::startGenerate (const QString &stlFilePath,  int imageSize, int stlType)
{
    // 在实际项目建议放到工作线程执行，这里为简化示例直接同步执行
    if (!QFileInfo::exists (stlFilePath))
    {
        emit generationFailed (QStringLiteral ("STL file does not exist: %1").arg (stlFilePath));
        return;
    }
    const QString& outputFileName = getStlThumbnailSavePath (stlType);
    bool ok = generatePreview (stlFilePath.toStdString(), outputFileName.toStdString(), imageSize);
    if (ok)
    {
        emit thumbnailReady (outputFileName, stlType);
    }
    else
    {
        emit generationFailed (QStringLiteral ("Failed to generate preview for %1").arg (stlFilePath));
    }
}

bool VtkStlPreviewGenerator::generatePreview (const std::string &stlFilePath, const std::string &outputImagePath,
        int imageSize)
{
    // 1. 读取 STL + 法线
    auto reader = vtkSmartPointer<vtkSTLReader>::New();
    reader->SetFileName (stlFilePath.c_str());
    reader->Update();
    if (!reader->GetOutput() || reader->GetOutput()->GetNumberOfPoints() == 0)
    {
        qWarning() << "Empty or invalid STL model.";
        return false;
    }

    auto normals = vtkSmartPointer<vtkPolyDataNormals>::New();
    normals->SetInputConnection (reader->GetOutputPort());
    normals->AutoOrientNormalsOn();
    normals->SplittingOff();
    normals->Update();

    // 2. 计算模型边界和缩放比例
    double bounds[6];
    normals->GetOutput()->GetBounds (bounds);
    double modelWidth = bounds[1] - bounds[0];
    double modelHeight = bounds[3] - bounds[2];
    double modelDepth = bounds[5] - bounds[4];
    double maxDimension = std::max ({modelWidth, modelHeight, modelDepth});
    if (maxDimension <= 0) return false;
    const double targetImageFraction = 0.7;
    double scaleFactor = (imageSize * targetImageFraction) / maxDimension;

    // 3. 创建Mapper和Actor
    auto mapper = vtkSmartPointer<vtkPolyDataMapper>::New();

    // 应用中心平移和缩放变换
    double center[3] =
    {
        (bounds[0] + bounds[1]) / 2.0,
        (bounds[2] + bounds[3]) / 2.0,
        (bounds[4] + bounds[5]) / 2.0
    };

    auto transform = vtkSmartPointer<vtkTransform>::New();
    transform->Translate (-center[0], -center[1], -center[2]);
    transform->Scale (scaleFactor, scaleFactor, scaleFactor);

    auto transformFilter = vtkSmartPointer<vtkTransformPolyDataFilter>::New();
    transformFilter->SetTransform (transform);
    transformFilter->SetInputConnection (normals->GetOutputPort());
    transformFilter->Update();

    mapper->SetInputConnection (transformFilter->GetOutputPort());

    auto actor = vtkSmartPointer<vtkActor>::New();
    actor->SetMapper (mapper);
    actor->GetProperty()->SetColor (0.75, 0.75, 0.73);
    actor->GetProperty()->SetAmbient (0.3);
    actor->GetProperty()->SetDiffuse (0.7);
    actor->GetProperty()->SetSpecular (0.2);
    actor->GetProperty()->SetSpecularPower (15.0);

    // 4. 渲染器配置
    auto renderer = vtkSmartPointer<vtkRenderer>::New();
    renderer->SetBackground (0.07, 0.09, 0.15);
    renderer->SetBackground2 (0.01, 0.01, 0.03);
    renderer->GradientBackgroundOn();

    // 光源
    auto light = vtkSmartPointer<vtkLight>::New();
    light->SetLightTypeToSceneLight();
    light->SetPosition (1.0, 1.0, 1.0);
    light->SetFocalPoint (0.0, 0.0, 0.0);
    light->SetColor (1.0, 1.0, 1.0);
    light->SetIntensity (0.8);
    renderer->AddLight (light);

    // // 背景网格 - 固定大小固定位置
    // auto plane = vtkSmartPointer<vtkPlaneSource>::New();
    // double gridHalfSize = 200.0;
    // double gridZ = -20.0;

    // plane->SetOrigin (-gridHalfSize, -gridHalfSize, gridZ);
    // plane->SetPoint1 (gridHalfSize, -gridHalfSize, gridZ);
    // plane->SetPoint2 (-gridHalfSize, gridHalfSize, gridZ);
    // plane->SetResolution (80, 80);

    // auto gridMapper = vtkSmartPointer<vtkPolyDataMapper>::New();
    // gridMapper->SetInputConnection (plane->GetOutputPort());

    // auto gridActor = vtkSmartPointer<vtkActor>::New();
    // gridActor->SetMapper (gridMapper);
    // gridActor->GetProperty()->SetRepresentationToWireframe();
    // gridActor->GetProperty()->SetColor (0.3, 0.3, 0.35);
    // gridActor->GetProperty()->SetLineWidth (0.5);
    // gridActor->GetProperty()->LightingOff();

    // 5. 渲染窗口
    auto renderWindow = vtkSmartPointer<vtkRenderWindow>::New();
    renderWindow->AddRenderer (renderer);
    renderWindow->SetSize (imageSize, imageSize);
    renderWindow->SetOffScreenRendering (1);

    renderer->AddActor (actor);
    // renderer->AddActor (gridActor);
    renderer->ResetCamera();
    renderWindow->Render();

    // 6. 保存PNG截图
    auto w2if = vtkSmartPointer<vtkWindowToImageFilter>::New();
    w2if->SetInput (renderWindow);
    w2if->SetScale (1);
    w2if->SetInputBufferTypeToRGBA();
    w2if->ReadFrontBufferOff();
    w2if->Update();

    auto writer = vtkSmartPointer<vtkPNGWriter>::New();
    writer->SetFileName (outputImagePath.c_str());
    writer->SetInputConnection (w2if->GetOutputPort());
    writer->Write();
    return true;
}

QString VtkStlPreviewGenerator::getStlThumbnailSavePath (int stlType)
{
    auto currentPatient = m_patientModel->currentPatient();
    if (!currentPatient)
    {
        qWarning() << "未选中病例，无法生成缩略图路径";
        emit generationFailed ("未选中病例，无法生成缩略图路径");
        return "";
    }

    const QString phone = currentPatient->phone().trimmed();
    if (phone.isEmpty())
    {
        qWarning() << "当前病例电话号码为空";
        emit generationFailed ("当前病例电话号码为空，无法上传文件");
        return "";
    }

    QString appConfigPath = ConfigManager::instance()->getConfig ("Facebow/AppConfigLocation");
    if (appConfigPath.isEmpty())
    {
        ConfigManager::instance()->reloadAppConfig();
        appConfigPath = ConfigManager::instance()->getConfig ("Facebow/AppConfigLocation");
    }

    if (appConfigPath.isEmpty())
    {
        qWarning() << "AppConfig 路径未设置";
        emit generationFailed ("AppConfig 路径未设置，无法上传文件");
        return "";
    }

    // 构建目标目录路径
    QDir patientDir (appConfigPath);
    QString relativeDirPath = phone + "/stl_thumbnail";
    QString fullDirPath = patientDir.filePath (relativeDirPath);

    QDir targetDir (fullDirPath);
    if (!targetDir.exists() && !targetDir.mkpath ("."))
    {
        qWarning() << "创建目标目录失败:" << fullDirPath;
        emit generationFailed ("创建目标目录失败: " + fullDirPath);
        return "";
    }

    // 根据 STL 类型生成对应缩略图文件名
    QString filename;
    switch (stlType)
    {
    case 1: filename = "maxilla_preview.png"; break;
    case 2: filename = "mandible_preview.png"; break;
    case 3: filename = "upper_dentition_preview.png"; break;
    case 4: filename = "lower_dentition_preview.png"; break;
    default:
        qWarning() << "不支持的 STL 类型:" << stlType;
        emit generationFailed ("不支持的 STL 类型: " + QString::number (stlType));
        return "";
    }

    // 返回最终文件完整路径
    qDebug() << "返回最终文件完整路径:" << targetDir.filePath (filename);

    return targetDir.filePath (filename);

}

