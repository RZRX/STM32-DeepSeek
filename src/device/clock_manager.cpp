#include "clock_manager.h"
#include <stm32h7xx_hal.h>

ClockManager::ClockManager(QObject *parent)
    : QObject(parent), m_currentClock(400) {} // 400 MHz по умолчанию

void ClockManager::setClockSpeed(uint32_t mhz) {
    RCC_OscInitTypeDef osc = {0};
    RCC_ClkInitTypeDef clk = {0};
    
    // Настройка PLL для H7
    osc.OscillatorType = RCC_OSCILLATORTYPE_HSE;
    osc.HSEState = RCC_HSE_ON;
    osc.PLL.PLLState = RCC_PLL_ON;
    osc.PLL.PLLSource = RCC_PLLSOURCE_HSE;
    osc.PLL.PLLM = 5;
    osc.PLL.PLLN = mhz;
    osc.PLL.PLLP = 2;
    osc.PLL.PLLQ = 4;
    
    HAL_RCC_OscConfig(&osc);
    
    clk.ClockType = RCC_CLOCKTYPE_SYSCLK | RCC_CLOCKTYPE_HCLK | RCC_CLOCKTYPE_PCLK1 | RCC_CLOCKTYPE_PCLK2;
    clk.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
    clk.SYSCLKDivider = RCC_SYSCLK_DIV1;
    clk.AHBCLKDivider = RCC_HCLK_DIV2;
    
    HAL_RCC_ClockConfig(&clk, FLASH_LATENCY_4);
    m_currentClock = mhz;
    emit clockChanged(mhz);
}

void ClockManager::enableLowPowerMode() {
    setClockSpeed(16); // Переход на MSI 16 MHz
    HAL_PWREx_EnableLowPowerRunMode();
}
