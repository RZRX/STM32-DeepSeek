#include "vcp_driver.h"
#include <usbd_cdc.h>

VCPDriver::VCPDriver(QObject *parent)
    : QObject(parent), m_rxBufferPos(0) {
    
    USBD_Init(&m_usbHandle, &CDC_Desc, 0);
    USBD_RegisterClass(&m_usbHandle, &USBD_CDC);
    USBD_CDC_RegisterInterface(&m_usbHandle, &USBD_CDC_fops);
    USBD_Start(&m_usbHandle);
}

void VCPDriver::sendData(const QByteArray &data) {
    CDC_Transmit_FS((uint8_t*)data.constData(), data.size());
}

void VCPDriver::onDataReceived(uint8_t* buf, uint32_t len) {
    m_rxBuffer.append((char*)buf, len);
    emit dataAvailable(m_rxBuffer);
    m_rxBuffer.clear();
}

void VCPDriver::handleLineCodingChanged(USBD_CDC_LineCodingTypeDef *lineCoding) {
    m_currentBaudrate = lineCoding->bitrate;
    emit baudrateChanged(m_currentBaudrate);
}
