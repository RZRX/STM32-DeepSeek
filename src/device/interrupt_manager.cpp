#include "interrupt_manager.h"
#include <QVector>

InterruptManager::InterruptManager(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger) {

    connect(m_debugger, &DebugEngine::debugOutput,
            this, &InterruptManager::handleDebugOutput);
}

void InterruptManager::handleDebugOutput(const QString &output) {
    if (output.contains("Exception")) {
        QRegularExpression regex("(\\w+) \\[IRQ:(\\d+)\\]");
        auto match = regex.match(output);
        if (match.hasMatch()) {
            emit interruptTriggered({
                .name = match.captured(1),
                .irqNumber = match.captured(2).toInt(),
                .timestamp = QDateTime::currentDateTime()
            });
        }
    }
}

void InterruptManager::setBreakpointOnIRQ(int irqNumber) {
    m_debugger->sendCommand(QString("b HAL_IRQHandler if $irq == %1").arg(irqNumber));
}
