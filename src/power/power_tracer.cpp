#include "power_tracer.h"
#include <QtDataVisualization>

PowerTracer::PowerTracer(QObject *parent)
    : QObject(parent), m_currentSample(0) {
    
    m_series = new QLineSeries();
    m_chart = new QChart();
    m_chart->addSeries(m_series);
    m_chart->createDefaultAxes();
}

void PowerTracer::addSample(float voltage, float current) {
    float power = voltage * current;
    m_series->append(m_currentSample++, power);
    
    if (m_series->count() > 1000) {
        m_series->removePoints(0, 100);
        m_chart->axisX()->setRange(m_currentSample - 1000, m_currentSample);
    }
    emit powerUpdated(power);
}

void PowerTracer::saveToCSV(const QString &path) {
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        QTextStream stream(&file);
        stream << "Sample,Power (mW)\n";
        for (int i = 0; i < m_series->count(); ++i) {
            stream << i << "," << m_series->at(i).y() << "\n";
        }
    }
}
