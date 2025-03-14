#include "settings_profile.h"
#include <QJsonDocument>

void SettingsProfile::saveToFile(const QString &path) {
    QJsonObject root;
    root["theme"] = m_theme;
    root["keybindings"] = QJsonObject::fromVariantMap(m_keybindings);
    
    QFile file(path);
    file.open(QIODevice::WriteOnly);
    file.write(QJsonDocument(root).toJson());
}

void SettingsProfile::loadFromFile(const QString &path) {
    QFile file(path);
    file.open(QIODevice::ReadOnly);
    QJsonObject root = QJsonDocument::fromJson(file.readAll()).object();
    
    m_theme = root["theme"].toString();
    m_keybindings = root["keybindings"].toVariant().toMap();
}

QStringList SettingsProfile::availableProfiles() {
    return QDir(configPath()).entryList({"*.json"});
}
