#include "src/config/ConfigManager.h"
#include <QDir>

ConfigManager *ConfigManager::s_instance = nullptr;

ConfigManager::ConfigManager (QObject *parent)
{
    m_localConfigPath = QStandardPaths::writableLocation (QStandardPaths::AppConfigLocation);
    QDir().mkpath (m_localConfigPath);
    m_settings = new QSettings (m_localConfigPath + "/config.ini",  QSettings::IniFormat);
    // 每次重启重新先加载
    initAppConfig();
}

ConfigManager::~ConfigManager()
{
    if (m_settings)
    {
        delete m_settings;
        m_settings = nullptr; // 避免悬挂指针
    }
    qDebug() << "~回收ConfigManager";
}

ConfigManager *ConfigManager::instance()
{
    if (!s_instance)
        s_instance = new ConfigManager();
    return s_instance;
}

QString ConfigManager::getConfig (const QString &key) const
{
    return m_settings->value (key).toString();
}

void ConfigManager::initAppConfig()
{
    m_settings->beginGroup ("Facebow");
    m_settings->setValue ("initialized", true);     // 加载配置在这里处理
    m_settings->setValue ("DocumentsLocation", QStandardPaths::writableLocation (QStandardPaths::DocumentsLocation));  // 设置应用目录
    m_settings->setValue ("AppConfigLocation", QStandardPaths::writableLocation (QStandardPaths::AppConfigLocation));  // 设置应用目录
    m_settings->endGroup();
    m_settings->sync();   //【重要】立即写入磁盘
}

void ConfigManager::reloadAppConfig()
{
    // 删除旧的 QSettings 对象（避免内存泄漏）
    if (m_settings)
    {
        delete m_settings;
        m_settings = nullptr;
    }
    // 重新创建 QSettings（会从磁盘加载现有配置）
    QString configPath = QStandardPaths::writableLocation (QStandardPaths::AppConfigLocation);
    QDir().mkpath (configPath);
    m_settings = new QSettings (configPath, QSettings::IniFormat, this);
    // 强制初始化默认配置（覆盖现有配置）
    initAppConfig();
}

void ConfigManager::debugPrintSettings()
{
    QStringList keys = m_settings->allKeys();
    for (const QString &key : std::as_const (keys))
    {
        qDebug() << "配置项 " << key << ":" << m_settings->value (key).toString();
    }
}
