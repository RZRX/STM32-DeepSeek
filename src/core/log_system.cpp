#include "log_system.h"
#include <QFile>
#include <QDateTime>

LogSystem::LogSystem(QObject *parent)
    : QObject(parent), m_logFile(new QFile("stm32ide.log", this)) {
    
    m_logFile->open(QIODevice::Append | QIODevice::Text);
}

void LogSystem::log(LogLevel level, const QString &message) {
    QString prefix;
    switch (level) {
        case Debug: prefix = "DEBUG"; break;
        case Warning: prefix = "WARN"; break;
        case Critical: prefix = "ERROR"; break;
    }
    
    QString entry = QString("[%1] %2: %3\n")
        .arg(QDateTime::currentDateTime().toString("yyyy-MM-dd hh:mm:ss"))
        .arg(prefix)
        .arg(message);
    
    m_logFile->write(entry.toUtf8());
    emit newLogEntry(entry);
}
