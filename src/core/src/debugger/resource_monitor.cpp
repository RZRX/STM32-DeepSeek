#include "resource_monitor.h"
#include <QChart>
#include <QLineSeries>

ResourceMonitor::ResourceMonitor(QWidget *parent)
    : QWidget(parent), m_cpuSeries(new QLineSeries()) {
    
    QChart *chart = new QChart();
    chart->addSeries(m_cpuSeries);
    chart->setTitle("CPU Usage");
    
    QChartView *chartView = new QChartView(chart);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

void ResourceMonitor::updateCpuUsage(float usage) {
    static int x = 0;
    m_cpuSeries->append(x++, usage);
    
    // Ограничение истории до 60 точек
    if (m_cpuSeries->count() > 60)
        m_cpuSeries->removePoints(0, m_cpuSeries->count() - 60);
}
