#include "rtos_objects_view.h"
#include <QTreeWidget>

RTOSObjectsView::RTOSObjectsView(QWidget *parent)
    : QTabWidget(parent), 
      m_queuesTab(new QTreeWidget()),
      m_semaphoresTab(new QTreeWidget()) {
    
    addTab(m_queuesTab, "Queues");
    addTab(m_semaphoresTab, "Semaphores");
    
    m_queuesTab->setHeaderLabels({"Name", "Messages", "Capacity"});
    m_semaphoresTab->setHeaderLabels({"Name", "Count", "Max Count"});
}

void RTOSObjectsView::updateQueues(const QVector<QueueInfo> &queues) {
    m_queuesTab->clear();
    for (const auto &queue : queues) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_queuesTab);
        item->setText(0, queue.name);
        item->setText(1, QString::number(queue.messages));
        item->setText(2, QString::number(queue.capacity));
    }
}

void RTOSObjectsView::updateSemaphores(const QVector<SemaphoreInfo> &semaphores) {
    m_semaphoresTab->clear();
    for (const auto &sem : semaphores) {
        QTreeWidgetItem *item = new QTreeWidgetItem(m_semaphoresTab);
        item->setText(0, sem.name);
        item->setText(1, QString::number(sem.count));
        item->setText(2, QString::number(sem.maxCount));
    }
}
