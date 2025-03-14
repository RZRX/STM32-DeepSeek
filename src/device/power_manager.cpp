#include "power_manager.h"
#include <cmath>

void PowerManager::enterLowPowerMode(PowerMode mode) {
    switch (mode) {
        case SleepMode:
            // Настройка регистров PWR
            m_debugger->writeMemory(0x40007000, 0x00000200); // SLEEPDEEP = 0
            asm("wfi");
            break;
        case StopMode:
            m_debugger->writeMemory(0x40007000, 0x00000400); // SLEEPDEEP = 1
            m_debugger->writeMemory(0x40023820, 0x00000008); // Clock config
            asm("wfi");
            break;
    }
}

void PowerManager::monitorPowerConsumption() {
    const quint32 adcAddr = 0x40012000;
    quint32 raw = m_debugger->readMemory(adcAddr + 0x4C); // Чтение ADC_DR
    double current = (3.3 * raw / 4096) / 0.1; // Расчет тока через шунт 0.1 Ом
    m_powerHistory.append(current * 3.3); // P = V*I
}
