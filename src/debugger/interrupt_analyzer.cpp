#include "interrupt_analyzer.h"
#include <QBarSeries>
#include <QChartView>

InterruptAnalyzer::InterruptAnalyzer(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger), m_maxIrq(82) {
    
    m_timer.setInterval(100); // Обновление данных каждые 100 мс
    connect(&m_timer, &QTimer::timeout, this, &InterruptAnalyzer::updateStats);
    connect(m_debugger, &DebugEngine::interruptTriggered,
            this, &InterruptAnalyzer::handleInterrupt);
}

void InterruptAnalyzer::handleInterrupt(int irq, quint32 timestamp) {
    IrqStats &stats = m_stats[irq];
    stats.count++;
    
    if (stats.lastTimestamp != 0) {
        stats.period = timestamp - stats.lastTimestamp;
    }
    stats.lastTimestamp = timestamp;
    stats.maxDuration = qMax(stats.maxDuration, 
        m_debugger->readRegister(IRQ_DURATION_REG));
}

void InterruptAnalyzer::updateStats() {
    QVector<QBarSet*> sets;
    QBarSet *countSet = new QBarSet("Trigger Count");
    QBarSet *durationSet = new QBarSet("Max Duration (µs)");
    
    for (int irq = 0; irq < m_maxIrq; ++irq) {
        *countSet << m_stats[irq].count;
        *durationSet << m_stats[irq].maxDuration;
    }
    
    QBarSeries *series = new QBarSeries();
    series->append(countSet);
    series->append(durationSet);
    emit statsUpdated(series);
}

void InterruptAnalyzer::exportToCSV(const QString &path) {
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "IRQ,Count,Max Duration (µs),Avg Period (ms)\n";
        for (int irq = 0; irq < m_maxIrq; ++irq) {
            stream << irq << ","
                   << m_stats[irq].count << ","
                   << m_stats[irq].maxDuration << ","
                   << (m_stats[irq].count > 0 ? 
                      (m_stats[irq].lastTimestamp / m_stats[irq].count) : 0)
                   << "\n";
        }
    }
}
