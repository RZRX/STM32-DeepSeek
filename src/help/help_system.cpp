#include "help_system.h"
#include <QWebEngineView>

HelpSystem::HelpSystem(QObject *parent)
    : QObject(parent), 
      m_view(new QWebEngineView()),
      m_docsPath(":/docs") {
    
    m_view->setUrl(QUrl("qrc:/docs/index.html"));
    m_view->setWindowTitle("STM32 IDE Documentation");
}

void HelpSystem::showHelpForKeyword(const QString &keyword) {
    QString path = QString("qrc:/docs/%1.html").arg(keyword);
    if (QFile::exists(m_docsPath.filePath(keyword + ".html"))) {
        m_view->setUrl(QUrl(path));
        m_view->show();
    }
}

void HelpSystem::generateAPIDocs() {
    QProcess doxygen;
    doxygen.start("doxygen", {"Doxyfile"});
    connect(&doxygen, &QProcess::finished, 
            [this]() { m_view->reload(); });
}
