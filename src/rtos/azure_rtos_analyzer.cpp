#include "azure_rtos_analyzer.h"
#include <QTreeWidget>

AzureRTOSAnalyzer::AzureRTOSAnalyzer(DebugEngine *debugger, QObject *parent)
    : QObject(parent), m_debugger(debugger), m_threadView(new QTreeWidget()) {
    
    m_threadView->setHeaderLabels({"Thread", "State", "Priority", "Stack"});
    connect(m_debugger, &DebugEngine::debugOutput, 
            this, &AzureRTOSAnalyzer::parseDebugOutput);
}

void AzureRTOSAnalyzer::parseDebugOutput(const QString &output) {
    if (output.contains("_tx_thread_current_ptr")) {
        m_debugger->sendCommand("print _tx_thread_created_ptr");
        m_debugger->sendCommand("print _tx_thread_created_count");
    }
}

void AzureRTOSAnalyzer::fetchThreadDetails() {
    quint32 threadPtr = m_debugger->readSymbolAddress("_tx_thread_created_ptr");
    quint32 count = m_debugger->readSymbolAddress("_tx_thread_created_count");
    
    for (quint32 i = 0; i < count; ++i) {
        quint32 threadAddr = m_debugger->readMemory(threadPtr + i * 4);
        m_debugger->sendCommand(QString("print ((TX_THREAD*)0x%1)->tx_thread_name")
            .arg(threadAddr, 8, 16, QLatin1Char('0')));
    }
}

void AzureRTOSAnalyzer::updateThreadList(const QVector<ThreadInfo> &threads) {
    m_threadView->clear();
    for (const auto &thread : threads) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_threadView);
        item->setText(0, thread.name);
        item->setText(1, threadStateToString(thread.state));
        item->setText(2, QString::number(thread.priority));
        item->setText(3, QString::number(thread.stackRemaining));
    }
}

QString AzureRTOSAnalyzer::threadStateToString(ThreadState state) {
    switch (state) {
        case Ready: return "Ready";
        case Suspended: return "Suspended";
        case Terminated: return "Terminated";
        default: return "Unknown";
    }
}
