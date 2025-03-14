#include "code_profiler.h"
#include <QTreeWidget>
#include <QVector2D>

CodeProfiler::CodeProfiler(QObject *parent)
    : QObject(parent), 
      m_hotspotView(new QGraphicsView()),
      m_scene(new QGraphicsScene(this)) {

    m_hotspotView->setScene(m_scene);
    m_hotspotView->setRenderHint(QPainter::Antialiasing);
}

void CodeProfiler::analyzeExecutionTrace(const ExecutionTrace &trace) {
    QMap<QString, FunctionStats> stats;
    for (const auto &event : trace.events) {
        FunctionStats &s = stats[event.function];
        s.totalTime += event.duration;
        s.callCount++;
    }

    // Визуализация тепловой карты
    QLinearGradient gradient(0, 0, 1, 1);
    gradient.setColorAt(0, Qt::blue);
    gradient.setColorAt(1, Qt::red);

    const int cellSize = 30;
    int x = 0, y = 0;
    for (auto it = stats.begin(); it != stats.end(); ++it) {
        float intensity = qMin(it->totalTime / 1000.0f, 1.0f);
        QGraphicsRectItem *rect = m_scene->addRect(x, y, cellSize, cellSize);
        rect->setBrush(QBrush(gradient));
        rect->setOpacity(intensity);
        
        x += cellSize;
        if (x > 500) { x = 0; y += cellSize; }
    }
}
