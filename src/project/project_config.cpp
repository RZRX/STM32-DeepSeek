#include "project_config.h"
#include <QJsonDocument>

ProjectConfig::ProjectConfig(const QString &path)
    : m_projectPath(path) {
    
    load();
}

void ProjectConfig::save() {
    QJsonObject root;
    root["mcu"] = m_mcu;
    root["optimization"] = m_optimization;
    
    QFile configFile(m_projectPath + "/.stm32ide");
    configFile.open(QIODevice::WriteOnly);
    configFile.write(QJsonDocument(root).toJson());
}

void ProjectConfig::load() {
    QFile configFile(m_projectPath + "/.stm32ide");
    if (!configFile.open(QIODevice::ReadOnly)) return;
    
    QJsonObject root = QJsonDocument::fromJson(configFile.readAll()).object();
    m_mcu = root["mcu"].toString();
    m_optimization = root["optimization"].toString();
}
