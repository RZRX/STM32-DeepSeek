#include "code_audit.h"
#include <clang-c/Index.h>

CodeAuditor::CodeAuditor(QObject *parent) 
    : QObject(parent), m_lexer(new QsciLexerCPP()) {}

void CodeAuditor::analyzeCodeSecurity(const QString &code) {
    CXIndex index = clang_createIndex(0, 0);
    CXTranslationUnit tu = clang_parseTranslationUnit(
        index,
        "input.c",
        nullptr, 0,
        code.toUtf8().constData(),
        code.size(),
        CXTranslationUnit_Incomplete
    );

    // Поиск уязвимостей
    CXCursor cursor = clang_getTranslationUnitCursor(tu);
    clang_visitChildren(cursor, [](CXCursor c, CXCursor parent, CXClientData data) {
        if (clang_getCursorKind(c) == CXCursor_CallExpr) {
            CXString name = clang_getCursorSpelling(c);
            const char *funcName = clang_getCString(name);
            if (strstr(funcName, "strcpy")) {
                emit static_cast<CodeAuditor*>(data)->securityIssueFound(
                    "Use of unsafe function: " + QString(funcName),
                    clang_getCursorLocation(c)
                );
            }
            clang_disposeString(name);
        }
        return CXChildVisit_Recurse;
    }, this);

    clang_disposeTranslationUnit(tu);
    clang_disposeIndex(index);
}
