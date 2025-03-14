#include "usb_device_stack.h"
#include <usbd_core.h>
#include <usbd_dfu.h>

USBDeviceStack::USBDeviceStack(QObject *parent)
    : QObject(parent), m_dfuModeActive(false) {
    
    USBD_Init(&m_usbHandle, &DFU_Desc, 0);
    USBD_RegisterClass(&m_usbHandle, &USBD_DFU);
    USBD_Start(&m_usbHandle);
}

void USBDeviceStack::enterDFUMode() {
    m_dfuModeActive = true;
    USBD_Stop(&m_usbHandle);
    // Переконфигурация USB в режим DFU
    USBD_Init(&m_usbHandle, &DFU_Desc, 0);
    USBD_RegisterClass(&m_usbHandle, &USBD_DFU);
    USBD_Start(&m_usbHandle);
    emit dfuModeChanged(true);
}

void USBDeviceStack::flashThroughUSB(const QString &firmwarePath) {
    QFile file(firmwarePath);
    if (file.open(QIODevice::ReadOnly)) {
        m_dfuData = file.readAll();
        USBD_DFU_HandleTypeDef *dfu = (USBD_DFU_HandleTypeDef*)m_usbHandle.pClassData;
        dfu->wblock_num = m_dfuData.size() / USB_DFU_XFER_SIZE;
        emit flashProgress(0);
    }
}

void USBDeviceStack::handleUSBEvent(USBD_EventTypeDef event) {
    if (event == USBD_EVT_RESET && m_dfuModeActive) {
        processDFURequest();
    }
}
