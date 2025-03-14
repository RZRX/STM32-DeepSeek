#include "stack_widget.h"
#include <QTreeWidget>
#include <QHeaderView>

StackWidget::StackWidget(QWidget *parent)
    : QTreeWidget(parent) {
    
    setColumnCount(4);
    setHeaderLabels({"Frame", "Function", "File", "Line"});
    setSortingEnabled(true);
    header()->setSectionResizeMode(QHeaderView::Interactive);
}

void StackWidget::updateStack(const QVector<StackFrame> &frames) {
    clear();
    for (const auto &frame : frames) {
        QTreeWidgetItem *item = new QTreeWidgetItem(this);
        item->setText(0, QString::number(frame.level));
        item->setText(1, frame.function);
        item->setText(2, frame.file);
        item->setText(3, QString::number(frame.line));
    }
    expandAll();
}

void StackWidget::onDoubleClick(QTreeWidgetItem *item) {
    emit frameSelected({
        .file = item->text(2),
        .line = item->text(3).toInt()
    });
}
