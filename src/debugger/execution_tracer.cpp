#include "execution_tracer.h"
#include <QGraphicsPathItem>

ExecutionTracer::ExecutionTracer(QObject *parent)
    : QObject(parent), m_traceMap(new QGraphicsView()) {
    
    m_scene = new QGraphicsScene();
    m_traceMap->setScene(m_scene);
    m_traceMap->setRenderHint(QPainter::Antialiasing);
}

void ExecutionTracer::recordBranch(quint32 from, quint32 to) {
    QPointF start = addressToPoint(from);
    QPointF end = addressToPoint(to);
    
    QGraphicsPathItem *path = new QGraphicsPathItem();
    QPainterPath p(start);
    p.lineTo(end);
    path->setPath(p);
    path->setPen(QPen(Qt::green, 1));
    m_scene->addItem(path);
}

QPointF ExecutionTracer::addressToPoint(quint32 addr) {
    return QPointF(
        (addr >> 16) & 0xFFFF, 
        addr & 0xFFFF
    );
}
