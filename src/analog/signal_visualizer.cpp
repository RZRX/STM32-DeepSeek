#include "signal_visualizer.h"
#include <QtCharts/QLineSeries>

SignalVisualizer::SignalVisualizer(QObject *parent)
    : QWidget(parent), m_sampleRate(1000) {
    
    m_chart = new QChart();
    m_series = new QLineSeries();
    m_chart->addSeries(m_series);
    
    QChartView *chartView = new QChartView(m_chart);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

void SignalVisualizer::addSample(float value) {
    static int x = 0;
    m_series->append(x++, value);
    
    if (m_series->count() > 1000) {
        m_series->removePoints(0, m_series->count() - 1000);
        m_chart->axisX()->setRange(x - 1000, x);
    }
}

void SignalVisualizer::setSampleRate(int rate) {
    m_sampleRate = rate;
    m_timer.setInterval(1000 / rate);
    connect(&m_timer, &QTimer::timeout, [this]() {
        if (m_dataSource) addSample(m_dataSource->read());
    });
}
