#include "interrupt_controller.h"
#include <QVector>

void InterruptController::configureNVIC(const NvicConfig &config) {
    // Программирование регистров NVIC
    const quint32 base = 0xE000E100;
    for (const auto &irq : config.irqs) {
        quint32 regOffset = irq.number / 4;
        quint32 bitPos = (irq.number % 4) * 8;
        quint32 addr = base + 0x100 + regOffset * 4;
        
        // Установка приоритета
        m_debugger->writeMemory(addr, (m_debugger->readMemory(addr) & ~(0xFF << bitPos)) 
            | (irq.priority << bitPos));
        
        // Активация прерывания
        m_debugger->writeMemory(base + 0x100 + (irq.number / 32) * 4, 1 << (irq.number % 32));
    }
}

void InterruptController::handlePendingIRQ() {
    quint32 pending = m_debugger->readMemory(0xE000E200);
    for (int i = 0; i < 32; ++i) {
        if (pending & (1 << i)) {
            emit irqTriggered(i);
        }
    }
}
