#include "clang_integration.h"
#include <clang-c/Index.h>

ClangAnalyzer::ClangAnalyzer(QObject *parent)
    : QObject(parent), m_index(clang_createIndex(0, 0)) {}

QVector<CodeIssue> ClangAnalyzer::analyzeProject(const QString &projectDir) {
    QVector<CodeIssue> issues;
    QDirIterator it(projectDir, {"*.c", "*.h"}, QDir::Files);
    
    while (it.hasNext()) {
        CXTranslationUnit tu = clang_parseTranslationUnit(
            m_index, it.next().toUtf8(), 
            nullptr, 0, nullptr, 0, CXTranslationUnit_None);
        
        unsigned diagCount = clang_getNumDiagnostics(tu);
        for (unsigned i = 0; i < diagCount; ++i) {
            CXDiagnostic diag = clang_getDiagnostic(tu, i);
            issues.append(parseDiagnostic(diag));
        }
        
        clang_disposeTranslationUnit(tu);
    }
    return issues;
}

CodeIssue ClangAnalyzer::parseDiagnostic(CXDiagnostic diag) {
    CXString msg = clang_getDiagnosticSpelling(diag);
    CXSourceLocation loc = clang_getDiagnosticLocation(diag);
    
    CXFile file;
    unsigned line, column;
    clang_getExpansionLocation(loc, &file, &line, &column, nullptr);
    
    return {
        .message = QString(clang_getCString(msg)),
        .file = QString(clang_getCString(clang_getFileName(file))),
        .line = line,
        .severity = static_cast<Severity>(clang_getDiagnosticSeverity(diag))
    };
}
