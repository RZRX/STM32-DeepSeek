#pragma once
#include <QObject>
#include <QProcess>
#include <QTreeWidgetItem>
#include "svd_parser.h"

class DebugEngine : public QObject {
    Q_OBJECT
public:
    enum DebugState {
        Disconnected,
        Running,
        Paused,
        Crashed
    };

    explicit DebugEngine(QObject *parent = nullptr);
    ~DebugEngine();

    void startSession(const QString &elfPath);
    void stopSession();
    void readRegister(uint32_t addr);
    void setBreakpoint(uint32_t addr);
    void requestMemoryDump(uint32_t addr, uint32_t size);

signals:
    void registerValueUpdated(uint32_t addr, uint32_t value);
    void memoryContentsReceived(QByteArray data);
    void debugOutput(const QString &msg);
    void stateChanged(DebugState state);

private slots:
    void handleGdbOutput();
    void handleOpenOcdError(QProcess::ProcessError error);

private:
    void parseGdbResponse(const QString &response);
    void initGdbCommands();
    
    QProcess *m_gdbProcess;
    QProcess *m_openocdProcess;
    SVDParser *m_svdParser;
    DebugState m_state;
    QMap<uint32_t, QTreeWidgetItem*> m_regItems;
};
