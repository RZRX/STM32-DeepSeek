#include "protocol_analyzer.h"
#include <QSerialPort>

ProtocolAnalyzer::ProtocolAnalyzer(QObject *parent)
    : QObject(parent), m_serial(new QSerialPort(this)) {
    
    connect(m_serial, &QSerialPort::readyRead,
            this, &ProtocolAnalyzer::processData);
}

void ProtocolAnalyzer::startCapture(ProtocolType type, const QString &port) {
    m_serial->setPortName(port);
    m_serial->setBaudRate(type == SPI ? 1000000 : 115200);
    m_serial->open(QIODevice::ReadOnly);
}

void ProtocolAnalyzer::processData() {
    QByteArray data = m_serial->readAll();
    switch(m_protocol) {
        case I2C: parseI2C(data); break;
        case SPI: parseSPI(data); break;
        case UART: parseUART(data); break;
    }
}

void ProtocolAnalyzer::parseI2C(const QByteArray &data) {
    // Декодирование I2C пакетов
    QVector<I2CFrame> frames;
    for (int i = 0; i < data.size(); i += 3) {
        frames.append({
            .address = static_cast<quint8>(data[i]),
            .data = static_cast<quint8>(data[i+1]),
            .ack = static_cast<bool>(data[i+2])
        });
    }
    emit i2cDataDecoded(frames);
}
