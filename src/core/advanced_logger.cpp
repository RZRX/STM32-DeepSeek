#include "advanced_logger.h"
#include <QDateTime>

AdvancedLogger::AdvancedLogger(QObject *parent)
    : QObject(parent), m_logLevel(LogLevel::Info), m_rotationSize(1024 * 1024) {
    
    m_logFile.setFileName("debug.log");
    rotateLogIfNeeded();
}

void AdvancedLogger::log(LogLevel level, const QString &message, const QString &category) {
    if (level < m_logLevel || m_filteredCategories.contains(category)) return;
    
    QString entry = QString("[%1][%2][%3] %4\n")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss.zzz"))
        .arg(logLevelToString(level))
        .arg(category)
        .arg(message);
    
    m_logFile.write(entry.toUtf8());
    emit newLogEntry(entry);
    rotateLogIfNeeded();
}

void AdvancedLogger::rotateLogIfNeeded() {
    if (m_logFile.size() >= m_rotationSize) {
        m_logFile.close();
        QFile::rename("debug.log", 
            QString("debug_%1.log").arg(QDateTime::currentDateTime().toString("yyyyMMdd_hhmmss")));
        m_logFile.open(QIODevice::WriteOnly | QIODevice::Append);
    }
}

void AdvancedLogger::addCategoryFilter(const QString &category) {
    m_filteredCategories.insert(category);
}
