#include "gpio_viewer.h"
#include <QGroupBox>
#include <QCheckBox>

GPIOViewer::GPIOViewer(QObject *parent)
    : IDEPlugin(parent), m_gpioWidget(nullptr) {}

void GPIOViewer::initialize(MainWindow *window) {
    m_window = window;
    m_gpioWidget = new QGroupBox("GPIO State");
    QGridLayout *layout = new QGridLayout(m_gpioWidget);
    
    for (int i = 0; i < 16; ++i) {
        QCheckBox *pin = new QCheckBox(QString("P%1").arg(i));
        layout->addWidget(pin, i/8, i%8);
        m_pins.append(pin);
    }
    
    window->addDebugWidget(m_gpioWidget);
}

void GPIOViewer::updateGPIOState(quint32 gpio) {
    for (int i = 0; i < 16; ++i) {
        m_pins[i]->setChecked(gpio & (1 << i));
    }
}
