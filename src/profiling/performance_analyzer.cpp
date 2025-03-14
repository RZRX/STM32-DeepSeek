#include "performance_analyzer.h"
#include <QtConcurrent>

PerformanceAnalyzer::PerformanceAnalyzer(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger), m_samples(10000) {

    connect(&m_sampleTimer, &QTimer::timeout, this, &PerformanceAnalyzer::sampleMetrics);
}

void PerformanceAnalyzer::startProfiling() {
    m_samples.clear();
    m_sampleTimer.start(1); // 1 ms интервал сэмплирования
}

void PerformanceAnalyzer::sampleMetrics() {
    PerformanceSample sample;
    sample.timestamp = QDateTime::currentDateTime();
    sample.pc = m_debugger->readRegister(15); // PC
    sample.memoryUsage = m_debugger->readMemory(0x20000000); // Начало heap
    
    m_samples.append(sample);
    if (m_samples.size() > 10000) {
        analyzeSamples();
    }
}

void PerformanceAnalyzer::analyzeSamples() {
    QtConcurrent::run([this]() {
        QMap<uint32_t, uint32_t> functionHits;
        for (const auto &sample : m_samples) {
            functionHits[sample.pc]++;
        }
        emit hotspotsDetected(functionHits);
    });
}
