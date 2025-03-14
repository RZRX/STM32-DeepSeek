#include "hardware_debug.h"
#include <libusb-1.0/libusb.h>

HardwareDebug::HardwareDebug(QObject *parent)
    : QObject(parent), m_ctx(nullptr) {
    
    libusb_init(&m_ctx);
}

void HardwareDebug::connectToProbe() {
    libusb_device **devs;
    ssize_t cnt = libusb_get_device_list(m_ctx, &devs);
    
    for (ssize_t i = 0; i < cnt; i++) {
        libusb_device_descriptor desc;
        libusb_get_device_descriptor(devs[i], &desc);
        
        if (desc.idVendor == 0x0483 && desc.idProduct == 0x3748) {
            libusb_open(devs[i], &m_handle);
            emit probeConnected();
            break;
        }
    }
    libusb_free_device_list(devs, 1);
}

void HardwareDebug::swdRead(uint32_t addr, uint32_t *data) {
    if (!m_handle) return;
    
    uint8_t cmd[5] = {
        0x81, // READ AP command
        static_cast<uint8_t>(addr >> 24),
        static_cast<uint8_t>(addr >> 16),
        static_cast<uint8_t>(addr >> 8),
        static_cast<uint8_t>(addr)
    };
    
    int transferred;
    libusb_bulk_transfer(m_handle, 0x01, cmd, 5, &transferred, 1000);
    libusb_bulk_transfer(m_handle, 0x81, (uint8_t*)data, 4, &transferred, 1000);
}
