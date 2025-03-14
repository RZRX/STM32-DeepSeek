#include "interrupt_dependency.h"
#include <QGraphicsScene>

InterruptDependencyView::InterruptDependencyView(QWidget *parent)
    : QGraphicsView(parent), m_scene(new QGraphicsScene(this)) {
    
    setScene(m_scene);
    setRenderHint(QPainter::Antialiasing);
}

void InterruptDependencyView::updateDependencies(const QMap<int, QVector<int>> &deps) {
    m_scene->clear();
    QMap<int, QGraphicsEllipseItem*> nodes;
    
    // Создание узлов
    int x = 0;
    for (auto it = deps.begin(); it != deps.end(); ++it) {
        QGraphicsEllipseItem *node = m_scene->addEllipse(x, 0, 50, 50);
        node->setToolTip(QString("IRQ%1").arg(it.key()));
        nodes[it.key()] = node;
        x += 100;
    }
    
    // Создание связей
    for (auto it = deps.begin(); it != deps.end(); ++it) {
        for (int dep : it.value()) {
            QLineF line(nodes[it.key()]->rect().center(),
                        nodes[dep]->rect().center());
            m_scene->addLine(line);
        }
    }
}
