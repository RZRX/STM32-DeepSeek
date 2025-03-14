#include "static_analyzer.h"
#include <clang-c/Index.h>

StaticAnalyzer::StaticAnalyzer(QObject *parent) 
    : IDEPlugin(parent) {}

void StaticAnalyzer::initialize(MainWindow *window) {
    m_editor = window->findChild<CodeEditor*>();
    connect(m_editor, &CodeEditor::textChanged, 
            this, &StaticAnalyzer::analyzeCurrentFile);
}

void StaticAnalyzer::analyzeCurrentFile() {
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit unit = clang_parseTranslationUnit(
        index,
        m_editor->currentFile().toUtf8(),
        nullptr, 0,
        nullptr, 0,
        CXTranslationUnit_None
    );

    if (!unit) return;

    // Анализ диагностик
    for (unsigned i = 0; i < clang_getNumDiagnostics(unit); ++i) {
        CXDiagnostic diag = clang_getDiagnostic(unit, i);
        emit issueFound(parseDiagnostic(diag));
    }

    clang_disposeTranslationUnit(unit);
    clang_disposeIndex(index);
}
