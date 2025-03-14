#include "dynamic_plugin_loader.h"
#include <QLibrary>

DynamicPluginLoader::DynamicPluginLoader(QObject *parent)
    : QObject(parent), m_pluginInterface(nullptr) {}

bool DynamicPluginLoader::loadPlugin(const QString &path) {
    QLibrary lib(path);
    if (!lib.load()) return false;

    typedef IDEPlugin* (*CreatePluginFunc)();
    CreatePluginFunc create = (CreatePluginFunc)lib.resolve("createPlugin");
    if (!create) return false;

    m_pluginInterface = create();
    m_pluginInterface->initialize(m_mainWindow);
    return true;
}

void DynamicPluginLoader::unloadPlugin() {
    if (m_pluginInterface) {
        delete m_pluginInterface;
        m_pluginInterface = nullptr;
    }
}

QVector<QAction*> DynamicPluginLoader::getPluginActions() {
    return m_pluginInterface ? m_pluginInterface->toolbarActions() : QVector<QAction*>();
}
