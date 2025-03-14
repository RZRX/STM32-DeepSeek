#include "macro_system.h"
#include <QFileInfo>

MacroSystem::MacroSystem(CodeEditor *editor, QObject *parent)
    : QObject(parent), m_editor(editor), m_recording(false) {
    
    m_macroDir.setPath(QStandardPaths::writableLocation(QStandardPaths::AppDataLocation) + "/macros");
    m_macroDir.mkpath(".");
}

void MacroSystem::startRecording() {
    m_recordedActions.clear();
    m_recording = true;
    connect(m_editor, &CodeEditor::keyPressed, this, &MacroSystem::recordAction);
}

void MacroSystem::stopRecording() {
    m_recording = false;
    disconnect(m_editor, &CodeEditor::keyPressed, this, &MacroSystem::recordAction);
}

void MacroSystem::saveMacro(const QString &name) {
    QFile file(m_macroDir.filePath(name + ".macro"));
    file.open(QIODevice::WriteOnly);
    QDataStream stream(&file);
    stream << m_recordedActions;
}

void MacroSystem::playMacro(const QString &name) {
    QFile file(m_macroDir.filePath(name + ".macro"));
    if (file.open(QIODevice::ReadOnly)) {
        QDataStream stream(&file);
        QVector<EditorAction> actions;
        stream >> actions;
        for (const auto &action : actions) {
            processAction(action);
        }
    }
}
