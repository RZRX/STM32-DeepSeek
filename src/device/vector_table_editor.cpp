#include "vector_table_editor.h"
#include <QTableWidget>

VectorTableEditor::VectorTableEditor(QWidget *parent)
    : QTableWidget(parent), m_vectorTable(256) {
    
    setColumnCount(3);
    setHorizontalHeaderLabels({"IRQ", "Handler", "Priority"});
    verticalHeader()->setVisible(false);
    
    for (int i = 0; i < m_vectorTable.size(); ++i) {
        insertRow(i);
        setItem(i, 0, new QTableWidgetItem(QString::number(i)));
        setItem(i, 1, new QTableWidgetItem("Undefined"));
        setItem(i, 2, new QTableWidgetItem("0"));
    }
}

void VectorTableEditor::loadFromMCU(DebugEngine *debugger) {
    for (int i = 0; i < m_vectorTable.size(); ++i) {
        quint32 handlerAddr = debugger->readMemory(VECTOR_TABLE_BASE + i * 4);
        setItem(i, 1, new QTableWidgetItem(
            debugger->symbolForAddress(handlerAddr)));
        
        quint32 priority = debugger->readMemory(NVIC_IPR0 + i);
        setItem(i, 2, new QTableWidgetItem(QString::number(priority)));
    }
}

void VectorTableEditor::applyChanges(DebugEngine *debugger) {
    for (int i = 0; i < rowCount(); ++i) {
        QString handler = item(i, 1)->text();
        quint32 addr = debugger->addressForSymbol(handler);
        debugger->writeMemory(VECTOR_TABLE_BASE + i * 4, addr);
        
        quint8 priority = item(i, 2)->text().toUInt();
        debugger->writeMemory(NVIC_IPR0 + i, priority << 4);
    }
}
