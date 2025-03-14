#include "peripheral_manager.h"
#include <QJsonDocument>

PeripheralManager::PeripheralManager(QObject *parent)
    : QObject(parent), m_debugger(nullptr) {}

void PeripheralManager::loadConfig(const QString &configPath) {
    QFile configFile(configPath);
    if (!configFile.open(QIODevice::ReadOnly)) return;
    
    QJsonObject config = QJsonDocument::fromJson(configFile.readAll()).object();
    for (const QString &key : config.keys()) {
        PeripheralConfig pConfig;
        pConfig.baseAddress = config[key].toObject()["base_address"].toString().toUInt(nullptr, 16);
        pConfig.irqNumber = config[key].toObject()["irq"].toInt();
        m_peripherals[key] = pConfig;
    }
}

void PeripheralManager::configureClock(PeripheralType type, uint32_t freq) {
    uint32_t regAddr = m_peripherals[peripheralTypeToString(type)].baseAddress + 0x08;
    uint32_t clockDiv = SystemCoreClock / freq - 1;
    m_debugger->writeMemory(regAddr, clockDiv);
}

void PeripheralManager::enableInterrupt(PeripheralType type) {
    uint32_t irq = m_peripherals[peripheralTypeToString(type)].irqNumber;
    m_debugger->writeMemory(NVIC_ISER + (irq / 32) * 4, 1 << (irq % 32));
}
