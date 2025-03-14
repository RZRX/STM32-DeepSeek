#include "memory_leak_detector.h"
#include <QStackedBarSeries>

MemoryLeakDetector::MemoryLeakDetector(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger) {
    
    m_allocationMap.fill(0, 1024); // Карта памяти 1KB
    connect(m_debugger, &DebugEngine::memoryWritten,
            this, &MemoryLeakDetector::trackAllocation);
}

void MemoryLeakDetector::trackAllocation(uint32_t addr, uint32_t size) {
    if (addr >= 0x20000000 && addr < 0x20001000) { // RAM область
        int index = (addr - 0x20000000) / 4;
        for (int i = 0; i < size/4; ++i) {
            if (m_allocationMap[index + i] != 0) {
                emit leakDetected(addr + i*4);
            }
            m_allocationMap[index + i]++;
        }
    }
}

void MemoryLeakDetector::visualizeMemoryMap() {
    QStackedBarSeries *series = new QStackedBarSeries();
    for (int i = 0; i < m_allocationMap.size(); ++i) {
        QBarSet *set = new QBarSet(QString::number(0x20000000 + i*4));
        *set << m_allocationMap[i];
        series->append(set);
    }
    emit memoryMapUpdated(series);
}
