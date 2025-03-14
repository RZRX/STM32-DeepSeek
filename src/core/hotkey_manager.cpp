#include "hotkey_manager.h"
#include <QAction>
#include <QJsonArray>

HotkeyManager::HotkeyManager(QObject *parent)
    : QObject(parent) {
    loadDefaultBindings();
}

void HotkeyManager::bind(const QString &actionId, const QKeySequence &seq) {
    if (m_actions.contains(actionId)) {
        m_actions[actionId]->setShortcut(seq);
    }
}

void HotkeyManager::loadProfile(const QString &path) {
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly)) return;

    QJsonArray bindings = QJsonDocument::fromJson(file.readAll()).array();
    for (const auto &entry : bindings) {
        QJsonObject obj = entry.toObject();
        bind(obj["action"].toString(), 
             QKeySequence(obj["key"].toString()));
    }
}

void HotkeyManager::loadDefaultBindings() {
    bind("build", QKeySequence("Ctrl+B"));
    bind("debug.start", QKeySequence("F5"));
    bind("debug.step", QKeySequence("F10"));
}
