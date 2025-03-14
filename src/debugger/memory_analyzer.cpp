#include "memory_analyzer.h"
#include <QtCharts/QPieSeries>

MemoryAnalyzer::MemoryAnalyzer(const QVector<MemoryRegion> &regions, QWidget *parent)
    : QWidget(parent) {
    
    QPieSeries *series = new QPieSeries();
    for (const auto &region : regions) {
        series->append(region.name + " (" + QString::number(region.size) + " bytes)", region.size);
    }

    QChart *chart = new QChart();
    chart->addSeries(series);
    chart->setTitle("Memory Map");

    QChartView *chartView = new QChartView(chart);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}
