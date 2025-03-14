#include "event_logger.h"
#include <QDateTime>

EventLogger::EventLogger(QObject *parent)
    : QObject(parent), m_logLevel(LogLevel::Info) {
    
    m_logFile.setFileName("events.log");
    m_logFile.open(QIODevice::Append | QIODevice::Text);
}

void EventLogger::log(LogLevel level, const QString &message) {
    if (level < m_logLevel) return;
    
    QString entry = QString("[%1] [%2] %3\n")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg(logLevelToString(level))
        .arg(message);
    
    m_logFile.write(entry.toUtf8());
    emit newLogEntry(entry);
}

void EventLogger::setLogLevel(LogLevel level) {
    m_logLevel = level;
    log(LogLevel::Info, QString("Log level changed to %1").arg(logLevelToString(level)));
}

QString EventLogger::logLevelToString(LogLevel level) {
    switch (level) {
        case Debug:   return "DEBUG";
        case Info:    return "INFO";
        case Warning: return "WARN";
        case Error:   return "ERROR";
        default:      return "UNKNOWN";
    }
}
