#include "interrupt_visualizer.h"
#include <QBarSeries>

InterruptVisualizer::InterruptVisualizer(QWidget *parent)
    : QChartView(parent), m_maxIrq(82) { // STM32H7 имеет 82 прерывания
    QBarSet *irqSet = new QBarSet("IRQ Activity");
    m_series = new QBarSeries();
    m_series->append(irqSet);
    
    QChart *chart = new QChart();
    chart->addSeries(m_series);
    chart->setTitle("Interrupt Activity");
    setChart(chart);
}

void InterruptVisualizer::updateIRQCount(int irqNumber) {
    if (irqNumber < 0 || irqNumber > m_maxIrq) return;
    QBarSet *set = m_series->barSets().first();
    set->replace(irqNumber, set->at(irqNumber) + 1);
    chart()->update();
}

void InterruptVisualizer::resetCounters() {
    QBarSet *set = m_series->barSets().first();
    for (int i = 0; i < m_maxIrq; ++i) {
        set->replace(i, 0);
    }
}
