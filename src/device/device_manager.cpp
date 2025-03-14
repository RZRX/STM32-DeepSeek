#include "device_manager.h"
#include <QSerialPortInfo>

DeviceManager::DeviceManager(QObject *parent)
    : QObject(parent), m_currentDevice(nullptr) {}

QStringList DeviceManager::availablePorts() const {
    QStringList ports;
    for(const auto &port : QSerialPortInfo::availablePorts())
        ports << port.portName();
    return ports;
}

bool DeviceManager::connectToDevice(const QString &portName) {
    if(m_currentDevice) disconnectDevice();
    
    m_currentDevice = new QSerialPort(portName, this);
    m_currentDevice->setBaudRate(115200);
    
    if(!m_currentDevice->open(QIODevice::ReadWrite)) {
        delete m_currentDevice;
        return false;
    }
    
    connect(m_currentDevice, &QSerialPort::readyRead,
            this, &DeviceManager::handleSerialData);
    return true;
}

void DeviceManager::sendCommand(const QByteArray &cmd) {
    if(m_currentDevice && m_currentDevice->isOpen())
        m_currentDevice->write(cmd + '\n');
}
