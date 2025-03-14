#include "hardware_test_suite.h"
#include <QTest>

HardwareTestSuite::HardwareTestSuite(QObject *parent)
    : QObject(parent), m_gpioTester(new GPIOTester()),
      m_ramTester(new MemTester()) {}

void HardwareTestSuite::runAllTests() {
    QTest::qExec(m_gpioTester.data());
    QTest::qExec(m_ramTester.data());
    testPeripheralCommunication();
    emit testsCompleted(m_results);
}

void HardwareTestSuite::testPeripheralCommunication() {
    // Тест SPI
    QByteArray txData = "Hello SPI";
    QByteArray rxData = m_spiDriver->transfer(txData);
    m_results["SPI"] = (rxData == txData) ? "PASS" : "FAIL";

    // Тест I2C
    m_i2cDriver->write(0x50, QByteArray::fromHex("A0"));
    QByteArray i2cResponse = m_i2cDriver->read(0x50, 2);
    m_results["I2C"] = (i2cResponse.size() == 2) ? "PASS" : "FAIL";
}

void HardwareTestSuite::generateReport(const QString &path) {
    QFile report(path);
    report.open(QIODevice::WriteOnly);
    QTextStream stream(&report);
    for (auto it = m_results.begin(); it != m_results.end(); ++it) {
        stream << it.key() << ": " << it.value() << "\n";
    }
}
