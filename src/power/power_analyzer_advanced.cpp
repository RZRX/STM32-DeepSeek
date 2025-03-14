#include "power_analyzer_advanced.h"
#include <QtDataVisualization>

PowerAnalyzerAdvanced::PowerAnalyzerAdvanced(QWidget *parent)
    : Q3DScatter(parent), m_currentMode(Idle) {
    
    addSeries(&m_powerSeries);
    setAxisX(new QValue3DAxis());
    setAxisY(new QValue3DAxis());
    setAxisZ(new QValue3DAxis());
}

void PowerAnalyzerAdvanced::updatePowerProfile(const PowerSample &sample) {
    QScatter3DSeries *series = new QScatter3DSeries();
    QScatterDataArray data;
    
    data << QVector3D(sample.voltage, sample.current, sample.temperature);
    series->dataProxy()->addItems(data);
    
    if (m_powerHistory.size() > 100) {
        removeSeries(m_powerHistory.dequeue());
    }
    m_powerHistory.enqueue(series);
}

void PowerAnalyzerAdvanced::analyzePowerStates() {
    QVector<PowerState> states;
    for (const auto &sample : m_samples) {
        if (sample.current > 100) states.append(Active);
        else if (sample.current > 50) states.append(Intermediate);
        else states.append(Idle);
    }
    emit powerStatesAnalyzed(states);
}
