#include "theme_manager.h"
#include <QFile>
#include <QJsonObject>

ThemeManager::ThemeManager(QObject *parent) 
    : QObject(parent), m_currentTheme("Dark") {}

void ThemeManager::applyTheme(const QString &themeName) {
    QFile themeFile(":/themes/" + themeName + ".json");
    if (!themeFile.open(QIODevice::ReadOnly)) return;

    QJsonObject theme = QJsonDocument::fromJson(themeFile.readAll()).object();
    
    // Применение стилей
    qApp->setPalette(parsePalette(theme["palette"].toObject()));
    emit syntaxThemeChanged(theme["syntax"].toObject());
}

QPalette ThemeManager::parsePalette(const QJsonObject &palette) {
    QPalette p;
    p.setColor(QPalette::Window, QColor(palette["window"].toString()));
    p.setColor(QPalette::Base, QColor(palette["base"].toString()));
    return p;
}
