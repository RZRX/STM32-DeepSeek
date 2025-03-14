#include "unit_test_runner.h"
#include <QTest>

UnitTestRunner::UnitTestRunner(QObject *parent)
    : QObject(parent), m_testOutput(new QPlainTextEdit()) {}

void UnitTestRunner::runTests(const QString &testDir) {
    QProcess testProc;
    testProc.setProgram("arm-none-eabi-run");
    testProc.setArguments({"-t", testDir + "/test_suite.elf"});
    
    connect(&testProc, &QProcess::readyReadStandardOutput, [this, &testProc]() {
        m_testOutput->appendPlainText(testProc.readAllStandardOutput());
    });
    
    testProc.start();
    testProc.waitForFinished();
    emit testsFinished(testProc.exitCode() == 0);
}

void UnitTestRunner::parseTestResults() {
    // Анализ вывода в формате TAP (Test Anything Protocol)
}
