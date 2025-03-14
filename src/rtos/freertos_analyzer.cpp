#include "freertos_analyzer.h"
#include <QTreeWidget>

FreeRTOSAnalyzer::FreeRTOSAnalyzer(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger), m_taskView(new QTreeWidget()) {
    
    m_taskView->setHeaderLabels({"Task", "State", "Priority", "Stack"});
    connect(m_debugger, &DebugEngine::debugOutput, 
            this, &FreeRTOSAnalyzer::parseDebugOutput);
}

void FreeRTOSAnalyzer::parseDebugOutput(const QString &output) {
    if (output.contains("pxCurrentTCB")) {
        m_debugger->sendCommand("print pxReadyTasksList");
        m_debugger->sendCommand("print xDelayedTaskList");
    }
    if (output.contains("uxTaskNumber")) {
        // Пример вывода: "uxTaskNumber = 0x3"
        int taskCount = output.split("=")[1].trimmed().toInt(nullptr, 16);
        fetchTaskDetails(taskCount);
    }
}

void FreeRTOSAnalyzer::fetchTaskDetails(int count) {
    m_taskView->clear();
    for (int i = 0; i < count; ++i) {
        m_debugger->sendCommand(QString("print ((TCB_t*)pxReadyTasksList.xListEnd.pxNext->pvOwner)->pcTaskName"));
        m_debugger->sendCommand(QString("print ((TCB_t*)pxReadyTasksList.xListEnd.pxNext->pvOwner)->uxCurrentPriority"));
    }
}

void FreeRTOSAnalyzer::updateTaskList(const QVector<TaskInfo> &tasks) {
    for (const auto &task : tasks) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_taskView);
        item->setText(0, task.name);
        item->setText(1, task.state);
        item->setText(2, QString::number(task.priority));
        item->setText(3, QString::number(task.stackHighWaterMark));
    }
}
