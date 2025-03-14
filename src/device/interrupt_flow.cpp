#include "interrupt_flow.h"
#include <QVector>

InterruptFlow::InterruptFlow(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger) {
    
    connect(m_debugger, &DebugEngine::debugOutput,
            this, &InterruptFlow::parseDebugOutput);
}

void InterruptFlow::parseDebugOutput(const QString &output) {
    if (output.contains("Exception")) {
        QRegularExpressionMatch match = m_exceptionRegex.match(output);
        if (match.hasMatch()) {
            ExceptionInfo info = {
                .type = match.captured(1),
                .address = match.captured(2).toUInt(nullptr, 16),
                .timestamp = QDateTime::currentDateTime()
            };
            m_exceptionLog.append(info);
            emit exceptionOccurred(info);
        }
    }
}

void InterruptFlow::generateSVGReport(const QString &filename) {
    QSvgGenerator generator;
    generator.setFileName(filename);
    QPainter painter(&generator);
    
    // Визуализация временной линии исключений
    painter.drawText(50, 50, "Interrupt Flow Report");
    for (int i = 0; i < m_exceptionLog.size(); ++i) {
        painter.drawText(50, 80 + i*30, 
            QString("%1: %2 @ 0x%3")
                .arg(m_exceptionLog[i].timestamp.toString("hh:mm:ss"))
                .arg(m_exceptionLog[i].type)
                .arg(m_exceptionLog[i].address, 8, 16));
    }
}
