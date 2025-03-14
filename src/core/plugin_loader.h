#pragma once
#include <QObject>
#include <QtPlugin>

class IDEPlugin {
public:
    virtual ~IDEPlugin() = default;
    virtual void initialize(class MainWindow *window) = 0;
    virtual QString name() const = 0;
};

Q_DECLARE_INTERFACE(IDEPlugin, "org.stm32ide.Plugin/1.0")

class PluginLoader : public QObject {
    Q_OBJECT
public:
    explicit PluginLoader(QObject *parent = nullptr);
    void loadAllPlugins(const QString &path);
    
private:
    QVector<IDEPlugin*> m_plugins;
};
