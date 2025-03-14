#include "hardware_tester.h"
#include <QTest>

void HardwareTester::runGPIOTest() {
    // Конфигурация пинов
    m_debugger->writeMemory(0x40020000, 0x55555555); // GPIOA MODER
    
    // Тест на обрыв
    quint32 idr = m_debugger->readMemory(0x40020010);
    if ((idr & 0xFF) != 0xAA) {
        emit testFailed("GPIO Short Circuit Detected");
    }
}

void HardwareTester::runADCTest() {
    m_debugger->writeMemory(0x40012008, 0x00000001); // ADC Enable
    QTest::qWait(100);
    
    quint32 dr = m_debugger->readMemory(0x4001204C);
    if (dr == 0 || dr > 4095) {
        emit testFailed("ADC Invalid Reading");
    }
}

void HardwareTester::generateTestReport() {
    QPdfWriter writer("tests_report.pdf");
    QPainter painter(&writer);
    painter.drawText(100, 100, "STM32 Hardware Test Report");
    // ... добавление результатов тестов
}
