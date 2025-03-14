#include "plugin_loader.h"
#include <QDir>
#include <QPluginLoader>

PluginLoader::PluginLoader(QObject *parent) : QObject(parent) {}

void PluginLoader::loadAllPlugins(const QString &path) {
    QDir pluginsDir(path);
    for(const auto &entry : pluginsDir.entryList(QDir::Files)) {
        QPluginLoader loader(pluginsDir.absoluteFilePath(entry));
        if(auto *plugin = qobject_cast<IDEPlugin*>(loader.instance())) {
            m_plugins.append(plugin);
            qInfo() << "Loaded plugin:" << plugin->name();
        }
    }
}
