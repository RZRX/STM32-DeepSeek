#include "peripheral_emulator.h"
#include <QJsonDocument>

PeripheralEmulator::PeripheralEmulator(QObject *parent)
    : QObject(parent), m_registers(0x2000) { // Эмуляция 8KB памяти

    loadPeripheralDefinitions(":/emulation/svd.json");
}

void PeripheralEmulator::loadPeripheralDefinitions(const QString &path) {
    QFile file(path);
    if (file.open(QIODevice::ReadOnly)) {
        QJsonObject defs = QJsonDocument::fromJson(file.readAll()).object();
        for (const QString &periph : defs.keys()) {
            m_peripheralMap[periph] = PeripheralDef::fromJson(defs[periph].toObject());
        }
    }
}

void PeripheralEmulator::writeRegister(uint32_t addr, uint32_t value) {
    if (addr >= 0x40000000 && addr < 0x60000000) { // Периферийные регистры
        const QString periph = findPeripheral(addr);
        if (!periph.isEmpty()) {
            // Эмуляция поведения периферии
            if (m_peripheralMap[periph].callbacks.contains("write")) {
                m_peripheralMap[periph].callbacks["write"](addr, value);
            }
        }
    }
    m_registers[addr] = value;
}

uint32_t PeripheralEmulator::readRegister(uint32_t addr) {
    if (m_peripheralMap.contains(periph)) {
        if (m_peripheralMap[periph].callbacks.contains("read")) {
            return m_peripheralMap[periph].callbacks["read"](addr);
        }
    }
    return m_registers[addr];
}
