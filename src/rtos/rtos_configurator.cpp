#include "rtos_configurator.h"
#include <QFormLayout>

RTOSConfigurator::RTOSConfigurator(QWidget *parent)
    : QWidget(parent), m_debugger(nullptr) {
    
    QFormLayout *layout = new QFormLayout(this);
    m_threadPriority = new QSpinBox(this);
    m_stackSize = new QSpinBox(this);
    m_timeSlice = new QSpinBox(this);
    
    layout->addRow("Default Thread Priority:", m_threadPriority);
    layout->addRow("Default Stack Size (bytes):", m_stackSize);
    layout->addRow("Time Slice (ticks):", m_timeSlice);
    
    connect(m_threadPriority, QOverload<int>::of(&QSpinBox::valueChanged),
            this, &RTOSConfigurator::configChanged);
}

void RTOSConfigurator::applyConfiguration() {
    if (!m_debugger) return;
    
    // Запись конфигурации в отладочные регистры
    m_debugger->writeSymbol("_tx_thread_priority_default", m_threadPriority->value());
    m_debugger->writeSymbol("_tx_thread_stack_default", m_stackSize->value());
    m_debugger->writeSymbol("_tx_thread_time_slice", m_timeSlice->value());
}

void RTOSConfigurator::loadCurrentConfig() {
    m_threadPriority->setValue(m_debugger->readSymbol("_tx_thread_priority_default"));
    m_stackSize->setValue(m_debugger->readSymbol("_tx_thread_stack_default"));
    m_timeSlice->setValue(m_debugger->readSymbol("_tx_thread_time_slice"));
}
