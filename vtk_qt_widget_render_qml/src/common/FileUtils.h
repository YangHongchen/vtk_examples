#ifndef FILEUTILS_H
#define FILEUTILS_H

#include <QString>
#include <QFileInfo>
#include <QUuid>
#include <QRandomGenerator>
#include <QDir>

class FileUtils
{
public:
    /**
     * @brief generateRandomFileName
     * @param originalFileName
     * @return
     */
    static QString generateRandomFileName (const QString& originalFileName)
    {
        QFileInfo fileInfo (originalFileName);
        // 获取原始目录路径
        QString dirPath = fileInfo.path();
        QString suffix = fileInfo.suffix().isEmpty()  ? "" : "." + fileInfo.suffix();
        // 使用时间戳 + UUID + 随机数确保唯一性
        QString baseName = QDateTime::currentDateTime().toString ("yyyyMMddhhmmsszzz") + "_" +
                           QUuid::createUuid().toString (QUuid::Id128).left (8) + "_" +
                           QString::number (QRandomGenerator::global()->generate());
        QString randomFileName = baseName + suffix;
        // 构造完整路径（使用QDir确保路径分隔符正确）
        return QDir(dirPath).filePath (randomFileName);
    }
};

#endif // FILEUTILS_H
