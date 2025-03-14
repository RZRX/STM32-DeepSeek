#include "plugin_manager.h"
#include <QDirIterator>

void PluginManager::loadAllPlugins() {
    QDirIterator it(m_pluginDirs, {"*.so", "*.dll"}, QDir::Files);
    while (it.hasNext()) {
        QPluginLoader loader(it.next());
        if (auto *plugin = qobject_cast<IDEPlugin*>(loader.instance())) {
            m_plugins.insert(plugin->metadata().id, plugin);
            initPluginDependencies(plugin);
        }
    }
}

void PluginManager::initPluginDependencies(IDEPlugin *plugin) {
    const auto &deps = plugin->metadata().dependencies;
    for (const auto &dep : deps) {
        if (m_plugins.contains(dep.id)) {
            plugin->dependencyInjected(dep.interface, m_plugins[dep.id]);
        }
    }
    plugin->initialize(m_mainWindow);
}

QVector<IDEPlugin*> PluginManager::pluginsByCategory(const QString &category) {
    QVector<IDEPlugin*> result;
    for (auto *plugin : m_plugins) {
        if (plugin->metadata().category == category)
            result.append(plugin);
    }
    return result;
}
