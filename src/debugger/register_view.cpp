#include "register_view.h"
#include <QHeaderView>

RegisterView::RegisterView(QWidget *parent)
    : QTableWidget(parent) {
    
    setColumnCount(3);
    setHorizontalHeaderLabels({"Register", "Hex Value", "Dec Value"});
    verticalHeader()->setVisible(false);
    setEditTriggers(QAbstractItemView::NoEditTriggers);
}

void RegisterView::updateRegister(const QString &name, quint32 value) {
    for (int i = 0; i < rowCount(); ++i) {
        if (item(i, 0)->text() == name) {
            item(i, 1)->setText(QString("0x%1").arg(value, 8, 16, QLatin1Char('0')));
            item(i, 2)->setText(QString::number((int)value));
            return;
        }
    }
    
    // Добавление новой строки
    int row = rowCount();
    insertRow(row);
    setItem(row, 0, new QTableWidgetItem(name));
    setItem(row, 1, new QTableWidgetItem(QString("0x%1").arg(value, 8, 16, QLatin1Char('0'))));
    setItem(row, 2, new QTableWidgetItem(QString::number((int)value)));
}
