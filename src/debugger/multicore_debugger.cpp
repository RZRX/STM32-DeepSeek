#include "multicore_debugger.h"
#include <QTabWidget>

MulticoreDebugger::MulticoreDebugger(QObject *parent)
    : QObject(parent), m_core1Active(false) {
    
    m_gdbProcesses.resize(2);
    m_registerViews.resize(2);
}

void MulticoreDebugger::attachToCores(const QString &elfPath) {
    // Подключение к ядру 0
    m_gdbProcesses[0].start("arm-none-eabi-gdb", {"-ex", "target remote :3333", elfPath});
    
    // Подключение к ядру 1 (если активно)
    if (m_core1Active) {
        m_gdbProcesses[1].start("arm-none-eabi-gdb", {"-ex", "target remote :3334", elfPath});
    }
}

void MulticoreDebugger::switchCore(int core) {
    if (core < 0 || core >= m_gdbProcesses.size()) return;
    m_currentCore = core;
    emit registersUpdated(m_registerViews[core]);
}

void MulticoreDebugger::readAllRegisters() {
    for (int i = 0; i < m_gdbProcesses.size(); ++i) {
        if (m_gdbProcesses[i].state() == QProcess::Running) {
            m_gdbProcesses[i].write("info registers\n");
        }
    }
}

void MulticoreDebugger::setBreakpointAllCores(uint32_t addr) {
    for (auto &gdb : m_gdbProcesses) {
        gdb.write(QString("break *0x%1\n").arg(addr, 8, 16, QLatin1Char('0')).toUtf8());
    }
}
