#include "task_scheduler.h"
#include <QThreadPool>

TaskScheduler::TaskScheduler(QObject *parent)
    : QObject(parent), m_isRunning(false) {

    m_threadPool.setMaxThreadCount(4);
}

void TaskScheduler::addTask(RTOS::Task task) {
    QMutexLocker locker(&m_mutex);
    m_taskQueue.enqueue(task);
    if (!m_isRunning) {
        QThreadPool::globalInstance()->start(this);
    }
}

void TaskScheduler::run() {
    m_isRunning = true;
    while (!m_taskQueue.isEmpty()) {
        QMutexLocker locker(&m_mutex);
        RTOS::Task task = m_taskQueue.dequeue();
        locker.unlock();

        if (task.priority > currentPriority()) {
            preemptCurrentTask(task);
        }
        executeTask(task);
    }
    m_isRunning = false;
}

void TaskScheduler::executeTask(const RTOS::Task &task) {
    QElapsedTimer timer;
    timer.start();
    task.function();
    emit taskCompleted(task.id, timer.elapsed());
}
