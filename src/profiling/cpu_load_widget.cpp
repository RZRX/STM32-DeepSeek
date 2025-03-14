#include "cpu_load_widget.h"
#include <QPainter>

CPULoadWidget::CPULoadWidget(QWidget *parent)
    : QWidget(parent), m_load(0), m_maxLoad(100) {
    
    setFixedSize(200, 100);
}

void CPULoadWidget::updateLoad(float load) {
    m_load = qMin(load, m_maxLoad);
    update();
}

void CPULoadWidget::paintEvent(QPaintEvent *) {
    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);
    
    // Фон
    painter.fillRect(rect(), QColor(30, 30, 30));
    
    // Градиентная заливка
    QLinearGradient grad(0, 0, width(), 0);
    grad.setColorAt(0, Qt::green);
    grad.setColorAt(0.5, Qt::yellow);
    grad.setColorAt(1, Qt::red);
    
    // Индикатор нагрузки
    painter.fillRect(10, 10, (width()-20)*m_load/m_maxLoad, 30, grad);
    
    // Текст
    painter.setPen(Qt::white);
    painter.drawText(rect(), Qt::AlignCenter, QString("%1%").arg(m_load, 0, 'f', 1));
}
