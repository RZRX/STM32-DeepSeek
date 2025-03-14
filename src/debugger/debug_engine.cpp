#include "debug_engine.h"
#include <QTcpSocket>
#include <QFileInfo>

DebugEngine::DebugEngine(QObject *parent) 
    : QObject(parent),
      m_state(Disconnected),
      m_svdParser(new SVDParser(this)) {
    
    m_gdbProcess = new QProcess(this);
    m_openocdProcess = new QProcess(this);
    
    connect(m_gdbProcess, &QProcess::readyReadStandardOutput,
            this, &DebugEngine::handleGdbOutput);
    connect(m_openocdProcess, &QProcess::errorOccurred,
            this, &DebugEngine::handleOpenOcdError);
}

void DebugEngine::startSession(const QString &elfPath) {
    if(m_state != Disconnected) return;

    // Запуск OpenOCD
    m_openocdProcess->start("openocd", {
        "-f", "interface/stlink-v2.cfg",
        "-f", "target/stm32f4x.cfg",
        "-c", "gdb_port 3333"
    });
    
    // Запуск GDB
    m_gdbProcess->start("arm-none-eabi-gdb", {
        "--nx",
        "-ex", "target extended-remote :3333",
        "-ex", "file " + elfPath,
        "-ex", "load"
    });
    
    m_state = Running;
    emit stateChanged(m_state);
}

void DebugEngine::handleGdbOutput() {
    const QString output = m_gdbProcess->readAllStandardOutput();
    debugOutput(output); // Пересылка в консоль
    
    // Пример обработки ответа
    if(output.contains("$")) {
        QString payload = output.mid(output.indexOf('$')+1);
        parseGdbResponse(payload.split('#')[0]);
    }
}

void DebugEngine::parseGdbResponse(const QString &response) {
    if(response.startsWith("T05")) { // Остановка на брейкпоинте
        m_state = Paused;
        emit stateChanged(m_state);
        readRegister(0xE000ED00); // Чтение регистра CONTROL
    }
    else if(response.startsWith("X")) { // Чтение памяти
        QByteArray data = QByteArray::fromHex(response.mid(1).toLatin1());
        emit memoryContentsReceived(data);
    }
}
