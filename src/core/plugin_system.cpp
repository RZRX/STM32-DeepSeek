#include "plugin_system.h"
#include <QDir>
#include <QPluginLoader>

PluginSystem::PluginSystem(QObject *parent) 
    : QObject(parent), m_pluginsDir(QDir::currentPath() + "/plugins") {
    
    m_pluginsDir.mkpath(".");
    m_fileWatcher = new QFileSystemWatcher(this);
    connect(m_fileWatcher, &QFileSystemWatcher::fileChanged, 
            this, &PluginSystem::reloadPlugin);
}

void PluginSystem::loadPlugins() {
    for (const QString &fileName : m_pluginsDir.entryList(QDir::Files)) {
        if (QLibrary::isLibrary(fileName)) {
            QPluginLoader loader(m_pluginsDir.filePath(fileName));
            if (auto *plugin = qobject_cast<IDEPlugin*>(loader.instance())) {
                m_plugins[fileName] = plugin;
                m_fileWatcher->addPath(m_pluginsDir.filePath(fileName));
                plugin->initialize();
            }
        }
    }
}

void PluginSystem::reloadPlugin(const QString &path) {
    QPluginLoader loader(path);
    if (loader.unload()) {
        if (auto *plugin = qobject_cast<IDEPlugin*>(loader.instance())) {
            m_plugins[QFileInfo(path).fileName()] = plugin;
            plugin->initialize();
        }
    }
}
