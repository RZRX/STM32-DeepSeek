#include "breakpoint_manager.h"
#include <QFileInfo>

BreakpointManager::BreakpointManager(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger) {}

void BreakpointManager::setBreakpoint(const QString &file, int line) {
    quint64 addr = resolveAddress(file, line);
    if (addr != 0) {
        m_debugger->sendCommand(QString("break *0x%1").arg(addr, 8, 16, QLatin1Char('0')));
        m_breakpoints.insert({file, line}, addr);
    }
}

quint64 BreakpointManager::resolveAddress(const QString &file, int line) {
    QProcess addr2line;
    addr2line.start("arm-none-eabi-addr2line", 
        {"-e", m_debugger->elfPath(), "-a", QString::number(line), "-i", file});
    
    if (addr2line.waitForFinished()) {
        QString output = addr2line.readAllStandardOutput();
        return output.split(" ").first().toULongLong(nullptr, 16);
    }
    return 0;
}

void BreakpointManager::onBreakpointHit(quint64 addr) {
    auto it = std::find_if(m_breakpoints.begin(), m_breakpoints.end(),
        [addr](const auto &bp) { return bp.addr == addr; });
    
    if (it != m_breakpoints.end()) {
        emit breakpointTriggered(it->file, it->line);
    }
}
