#include "script_engine.h"
#include <QJSEngine>

ScriptEngine::ScriptEngine(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger) {
    
    m_engine = new QJSEngine(this);
    m_engine->globalObject().setProperty("debugger", 
        m_engine->newQObject(m_debugger));
}

QVariant ScriptEngine::executeScript(const QString &script) {
    QJSValue result = m_engine->evaluate(script);
    if (result.isError()) {
        emit scriptError(result.toString());
        return {};
    }
    return result.toVariant();
}

void ScriptEngine::registerAPI(const QString &name, QObject *api) {
    m_engine->globalObject().setProperty(name, 
        m_engine->newQObject(api));
}

void ScriptEngine::loadPluginScripts(const QString &dir) {
    QDir scriptsDir(dir);
    for (const QString &file : scriptsDir.entryList({"*.js"})) {
        QFile scriptFile(scriptsDir.filePath(file));
        scriptFile.open(QIODevice::ReadOnly);
        executeScript(scriptFile.readAll());
    }
}
