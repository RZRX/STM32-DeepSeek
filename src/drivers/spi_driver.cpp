#include "spi_driver.h"
#include <stm32f4xx_hal.h>

SPIDriver::SPIDriver(SPI_TypeDef *instance, QObject *parent)
    : QObject(parent), m_spiHandle(new SPI_HandleTypeDef) {
    
    m_spiHandle->Instance = instance;
    m_spiHandle->Init.Mode = SPI_MODE_MASTER;
    m_spiHandle->Init.Direction = SPI_DIRECTION_2LINES;
    m_spiHandle->Init.DataSize = SPI_DATASIZE_8BIT;
    m_spiHandle->Init.CLKPolarity = SPI_POLARITY_LOW;
    m_spiHandle->Init.CLKPhase = SPI_PHASE_1EDGE;
    m_spiHandle->Init.NSS = SPI_NSS_SOFT;
    m_spiHandle->Init.BaudRatePrescaler = SPI_BAUDRATEPRESCALER_8;
    HAL_SPI_Init(m_spiHandle);
}

QByteArray SPIDriver::transfer(const QByteArray &data) {
    QByteArray rxBuf(data.size(), 0);
    HAL_SPI_TransmitReceive(m_spiHandle, 
        (uint8_t*)data.data(), 
        (uint8_t*)rxBuf.data(), 
        data.size(), 
        HAL_MAX_DELAY);
    return rxBuf;
}

void SPIDriver::setClockSpeed(uint32_t prescaler) {
    HAL_SPI_DeInit(m_spiHandle);
    m_spiHandle->Init.BaudRatePrescaler = prescaler;
    HAL_SPI_Init(m_spiHandle);
}
