#include "power_analyzer.h"
#include <QtCharts/QLineSeries>

PowerAnalyzer::PowerAnalyzer(QWidget *parent)
    : QWidget(parent), m_currentSeries(new QLineSeries()) {

    QChart *chart = new QChart();
    chart->addSeries(m_currentSeries);
    chart->createDefaultAxes();
    
    QChartView *chartView = new QChartView(chart);
    QVBoxLayout *layout = new QVBoxLayout(this);
    layout->addWidget(chartView);
}

void PowerAnalyzer::updateFromADC(const QVector<quint16> &samples) {
    static int x = 0;
    for (auto sample : samples) {
        m_currentSeries->append(x++, 3.3 * sample / 4096); // Convert to voltage
    }
    
    if (m_currentSeries->count() > 1000) {
        m_currentSeries->removePoints(0, m_currentSeries->count() - 1000);
    }
}

void PowerAnalyzer::exportReport(const QString &path) {
    QFile file(path);
    if (file.open(QIODevice::WriteOnly)) {
        for (int i = 0; i < m_currentSeries->count(); ++i) {
            file.write(QString("%1,%2\n")
                .arg(m_currentSeries->at(i).x())
                .arg(m_currentSeries->at(i).y()).toUtf8());
        }
    }
}
