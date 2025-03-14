#include "ble_manager.h"
#include <QLowEnergyController>

BLEManager::BLEManager(QObject *parent)
    : QObject(parent), m_controller(nullptr) {}

void BLEManager::startScan() {
    m_deviceScanner = new QBluetoothDeviceDiscoveryAgent(this);
    connect(m_deviceScanner, &QBluetoothDeviceDiscoveryAgent::deviceDiscovered,
            this, &BLEManager::addDevice);
    m_deviceScanner->start();
}

void BLEManager::connectToDevice(const QString &address) {
    QBluetoothAddress deviceAddr(address);
    m_controller = QLowEnergyController::createCentral(deviceAddr, this);
    
    connect(m_controller, &QLowEnergyController::connected,
            this, [this]() { emit deviceConnected(); });
    
    connect(m_controller, &QLowEnergyController::serviceDiscovered,
            this, &BLEManager::serviceDiscovered);
    
    m_controller->connectToDevice();
}

void BLEManager::serviceDiscovered(const QBluetoothUuid &uuid) {
    if (uuid == QBluetoothUuid::DeviceInformation) {
        m_service = m_controller->createServiceObject(uuid, this);
        m_service->discoverDetails();
    }
}

void BLEManager::readCharacteristic(const QBluetoothUuid &charUuid) {
    QLowEnergyCharacteristic characteristic = m_service->characteristic(charUuid);
    if (characteristic.isValid()) {
        m_service->readCharacteristic(characteristic);
    }
}
