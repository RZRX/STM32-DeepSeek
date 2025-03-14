#include "memory_profiler.h"
#include <QStackedBarSeries>

MemoryProfiler::MemoryProfiler(QObject *parent)
    : QObject(parent), m_heapUsage(0), m_leakDetectionEnabled(false) {
    
    connect(&m_timer, &QTimer::timeout, this, &MemoryProfiler::checkHeap);
    m_timer.start(1000); // Проверка каждую секунду
}

void MemoryProfiler::checkHeap() {
    quint32 heapStart = m_debugger->readMemory(0x20000000);
    quint32 heapEnd = m_debugger->readMemory(0x20000000 + m_heapSize);
    m_heapUsage = heapEnd - heapStart;
    
    if (m_leakDetectionEnabled) {
        detectLeaks();
    }
    emit heapUsageUpdated(m_heapUsage);
}

void MemoryProfiler::detectLeaks() {
    // Сравнение текущих выделений с предыдущим снимком
    QVector<AllocationRecord> currentAllocs = parseAllocations();
    for (const auto &alloc : currentAllocs) {
        if (!m_previousAllocs.contains(alloc)) {
            m_leaks.append(alloc);
            emit leakDetected(alloc);
        }
    }
    m_previousAllocs = currentAllocs;
}

QVector<AllocationRecord> MemoryProfiler::parseAllocations() {
    QVector<AllocationRecord> allocs;
    quint32 allocPtr = m_debugger->readMemory(0x20000100); // Адрес таблицы аллокаций
    while (allocPtr != 0) {
        AllocationRecord rec = {
            .address = m_debugger->readMemory(allocPtr),
            .size = m_debugger->readMemory(allocPtr + 4)
        };
        allocs.append(rec);
        allocPtr = m_debugger->readMemory(allocPtr + 8);
    }
    return allocs;
}
