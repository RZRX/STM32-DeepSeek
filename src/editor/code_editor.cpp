#include "code_editor.h"
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexermakefile.h>
#include <QShortcut>

CodeEditor::CodeEditor(QWidget *parent) 
    : QsciScintilla(parent) {
    
    // Настройка лексера C/C++
    QsciLexerCPP *lexer = new QsciLexerCPP(this);
    lexer->setFont(QFont("Consolas", 12));
    setLexer(lexer);

    // Автодополнение
    setAutoCompletionSource(QsciScintilla::AcsAll);
    setAutoCompletionThreshold(2);
    setAutoCompletionCaseSensitivity(true);

    // Параметры отображения
    setMarginType(0, QsciScintilla::NumberMargin);
    setMarginWidth(0, "0000");
    setFolding(QsciScintilla::BoxedTreeFoldStyle);
    setBraceMatching(QsciScintilla::SloppyBraceMatch);

    // Горячие клавиши
    new QShortcut(QKeySequence("Ctrl+Space"), this, SLOT(autoComplete()));
}

void CodeEditor::loadFile(const QString &path) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        setText(file.readAll());
        m_currentFile = path;
        emit fileOpened(path);
    }
}

void CodeEditor::saveFile() {
    if (m_currentFile.isEmpty()) return;
    QFile file(m_currentFile);
    if (file.open(QIODevice::WriteOnly)) {
        file.write(text().toUtf8());
    }
}
