#ifndef CONFIGMANAGER_H
#define CONFIGMANAGER_H

#include <QObject>
#include <QString>
#include <QSettings>
#include <QStandardPaths>

class ConfigManager : public QObject
{
    Q_OBJECT

public:
    // 单例模式，获取唯一实例
    static ConfigManager *instance();
    // 析构方法
    ~ConfigManager();
    // 调试输出当前配置
    void debugPrintSettings();
    // 初始化配置
    void initAppConfig();
    // 重新加载配置
    void reloadAppConfig();
public:
    // 获取配置项
    Q_INVOKABLE QString getConfig (const QString& key) const;

private:
    explicit ConfigManager(QObject *parent = nullptr);


private:
    // 当前选中的病例
    static ConfigManager *s_instance;
    // QSettings 对象，用于读取和写入配置
    QSettings *m_settings;
    // 文档路径
    QString m_documentsPath;
    // 应用配置路径
    QString m_localConfigPath;

};

#endif // CONFIGMANAGER_H
