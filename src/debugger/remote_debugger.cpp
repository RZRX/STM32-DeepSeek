#include "remote_debugger.h"
#include <QTcpSocket>
#include <QMessageBox>

RemoteDebugger::RemoteDebugger(QObject *parent)
    : QObject(parent), m_socket(new QTcpSocket(this)) {

    connect(m_socket, &QTcpSocket::readyRead, 
            this, &RemoteDebugger::handleIncomingData);
    connect(m_socket, &QTcpSocket::errorOccurred,
            this, &RemoteDebugger::handleSocketError);
}

void RemoteDebugger::connectToHost(const QString &host, quint16 port) {
    m_socket->connectToHost(host, port);
    if (!m_socket->waitForConnected(3000)) {
        emit error("Connection timeout");
    }
}

void RemoteDebugger::sendGdbCommand(const QString &cmd) {
    QByteArray data = QString("$%1#%2")
        .arg(cmd)
        .arg(qChecksum(cmd.toUtf8(), cmd.size()), 2, 16)
        .toUtf8();
    m_socket->write(data);
}

void RemoteDebugger::handleIncomingData() {
    QByteArray data = m_socket->readAll();
    if (data.startsWith('$')) {
        int end = data.indexOf('#');
        QString payload = data.mid(1, end-1);
        quint8 checksum = data.mid(end+1, 2).toInt(nullptr, 16);
        
        if (qChecksum(payload.toUtf8(), payload.size()) == checksum) {
            processGdbPacket(payload);
        }
    }
}
