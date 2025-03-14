#include "touch_driver.h"
#include <stm32h7xx_hal.h>

TouchDriver::TouchDriver(I2C_HandleTypeDef *hi2c, QObject *parent)
    : QObject(parent), m_hi2c(hi2c), m_lastTouchPoint({0, 0}) {}

void TouchDriver::calibrate() {
    uint8_t calData[4];
    HAL_I2C_Mem_Read(m_hi2c, TOUCH_ADDR << 1, 
        CALIBRATION_REG, I2C_MEMADD_SIZE_8BIT,
        calData, sizeof(calData), HAL_MAX_DELAY);
    
    m_calibMatrix = {
        static_cast<float>(calData[0] << 8 | calData[1]),
        static_cast<float>(calData[2] << 8 | calData[3])
    };
}

TouchPoint TouchDriver::getTouch() {
    uint8_t rawData[4];
    HAL_I2C_Mem_Read(m_hi2c, TOUCH_ADDR << 1,
        TOUCH_DATA_REG, I2C_MEMADD_SIZE_8BIT,
        rawData, sizeof(rawData), HAL_MAX_DELAY);
    
    return {
        .x = static_cast<int>((rawData[0] << 8 | rawData[1]) * m_calibMatrix[0]),
        .y = static_cast<int>((rawData[2] << 8 | rawData[3]) * m_calibMatrix[1])
    };
}

void TouchDriver::handleInterrupt() {
    TouchPoint tp = getTouch();
    if (tp.x != m_lastTouchPoint.x || tp.y != m_lastTouchPoint.y) {
        emit touchEvent(tp);
        m_lastTouchPoint = tp;
    }
}
