#include "usb_debug_interface.h"
#include <QElapsedTimer>

USBDebugInterface::USBDebugInterface(QObject *parent)
    : DebugInterface(parent), m_vcp(new VCPDriver(this)) {
    
    connect(m_vcp, &VCPDriver::dataAvailable,
            this, &USBDebugInterface::processIncomingPacket);
}

void USBDebugInterface::sendCommand(const QByteArray &cmd) {
    m_vcp->sendData(packGDBPacket(cmd));
}

void USBDebugInterface::processIncomingPacket(const QByteArray &data) {
    if (data.startsWith("$")) {
        QByteArray payload = extractPayload(data);
        emit packetReceived(payload);
    }
}

QByteArray USBDebugInterface::packGDBPacket(const QByteArray &data) {
    QByteArray packet = "$" + data + "#" 
        + QByteArray::number(qChecksum(data), 2, 16);
    return packet;
}
