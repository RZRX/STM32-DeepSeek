#include "sensor_monitor.h"
#include <QDial>

SensorMonitor::SensorMonitor(QObject *parent)
    : IDEPlugin(parent), m_sensorWidget(new QWidget()) {
    
    QGridLayout *layout = new QGridLayout(m_sensorWidget);
    for (int i = 0; i < 6; ++i) {
        QDial *dial = new QDial();
        dial->setRange(0, 100);
        layout->addWidget(dial, i/3, i%3);
    }
}

void SensorMonitor::initialize(MainWindow *window) {
    window->addDebugWidget(m_sensorWidget);
    connect(window->debugEngine(), &DebugEngine::registerValueUpdated,
            this, &SensorMonitor::updateSensor);
}

void SensorMonitor::updateSensor(uint32_t addr, uint32_t value) {
    if (addr >= 0x40012000 && addr <= 0x400123FF) { // ADC регистры
        int channel = (addr - 0x40012000) / 0x04;
        m_dials[channel]->setValue(value);
    }
}
