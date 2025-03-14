#include "cubemx_integration.h"
#include <QProcess>
#include <QJsonDocument>

CubeMXIntegration::CubeMXIntegration(QObject *parent)
    : QObject(parent), m_cubemxProcess(new QProcess(this)) {}

void CubeMXIntegration::generateCode(const QString &iocPath) {
    if (!QFile::exists(iocPath)) {
        emit error("CubeMX .ioc file not found");
        return;
    }

    m_cubemxProcess->start("STM32CubeMX", {
        "-s", iocPath,
        "-t", "SW4STM32",
        "-r", "project"
    });

    connect(m_cubemxProcess, QOverload<int>::of(&QProcess::finished),
            [this](int exitCode) {
        if (exitCode != 0) {
            emit error("CubeMX codegen failed: " + 
                      m_cubemxProcess->readAllStandardError());
        } else {
            parseGeneratedCode();
        }
    });
}

void CubeMXIntegration::parseGeneratedCode() {
    QDir dir("Generated");
    for (const auto &file : dir.entryList({"*.c", "*.h"})) {
        emit fileGenerated(dir.filePath(file));
    }
}
