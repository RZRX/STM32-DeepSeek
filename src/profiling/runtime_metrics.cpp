#include "runtime_metrics.h"
#include <QtConcurrent>

RuntimeMetrics::RuntimeMetrics(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger) {
    
    m_metricsTimer.setInterval(100);
    connect(&m_metricsTimer, &QTimer::timeout, this, &RuntimeMetrics::collectMetrics);
}

void RuntimeMetrics::startCollection() {
    m_metrics.clear();
    m_metricsTimer.start();
}

void RuntimeMetrics::collectMetrics() {
    RuntimeMetric metric;
    metric.timestamp = QDateTime::currentDateTime();
    metric.cpuLoad = m_debugger->readSymbol("osCPUUsage");
    metric.heapUsage = m_debugger->readSymbol("xPortGetFreeHeapSize");
    
    m_metrics.append(metric);
    emit metricsUpdated(metric);
}

void RuntimeMetrics::generateReport() {
    QtConcurrent::run([this]() {
        QFile report("metrics_report.csv");
        if (report.open(QIODevice::WriteOnly)) {
            report.write("Timestamp,CPU Load,Heap Usage\n");
            for (const auto &m : m_metrics) {
                report.write(QString("%1,%2,%3\n")
                    .arg(m.timestamp.toString("yyyy-MM-dd hh:mm:ss"))
                    .arg(m.cpuLoad)
                    .arg(m.heapUsage).toUtf8());
            }
        }
    });
}
