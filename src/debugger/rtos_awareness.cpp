#include "rtos_awareness.h"
#include <QTreeWidget>

RTOSAwareness::RTOSAwareness(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger) {

    m_taskView = new QTreeWidget();
    m_taskView->setHeaderLabels({"Task", "State", "Priority"});
    
    connect(m_debugger, &DebugEngine::debugOutput,
            this, &RTOSAwareness::parseDebugOutput);
}

void RTOSAwareness::parseDebugOutput(const QString &output) {
    if (output.contains("FreeRTOS")) {
        m_debugger->sendCommand("info threads");
        m_debugger->sendCommand("print pxReadyTasksList");
    }
}

void RTOSAwareness::updateTaskList(const QVector<TaskInfo> &tasks) {
    m_taskView->clear();
    for (const auto &task : tasks) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_taskView);
        item->setText(0, task.name);
        item->setText(1, task.state);
        item->setText(2, QString::number(task.priority));
    }
}
