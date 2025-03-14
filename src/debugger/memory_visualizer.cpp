#include "memory_visualizer.h"
#include <QPainter>
#include <QPaintEvent>

MemoryVisualizer::MemoryVisualizer(QWidget *parent)
    : QWidget(parent), m_baseAddr(0x08000000), m_bytesPerRow(16) {
    
    setFixedWidth(800);
    setFont(QFont("Monospace", 10));
}

void MemoryVisualizer::updateMemory(const QByteArray &data, quint32 base) {
    m_data = data;
    m_baseAddr = base;
    update();
}

void MemoryVisualizer::paintEvent(QPaintEvent *event) {
    QPainter painter(this);
    painter.fillRect(event->rect(), QColor(30, 30, 30));

    const int lineHeight = painter.fontMetrics().height();
    const int addrWidth = 100;
    const int byteWidth = 25;

    painter.setPen(Qt::white);
    for (int i = 0; i < m_data.size(); i += m_bytesPerRow) {
        quint32 addr = m_baseAddr + i;
        painter.drawText(10, lineHeight * (i/m_bytesPerRow + 1), 
            QString("%1").arg(addr, 8, 16, QLatin1Char('0')));

        for (int j = 0; j < m_bytesPerRow; j++) {
            if (i + j >= m_data.size()) break;
            quint8 val = static_cast<quint8>(m_data[i + j]);
            painter.setPen(val == 0 ? Qt::gray : Qt::cyan);
            painter.drawText(addrWidth + j * byteWidth, 
                            lineHeight * (i/m_bytesPerRow + 1),
                            QString("%1").arg(val, 2, 16, QLatin1Char('0')));
        }
    }
}

void MemoryVisualizer::mouseDoubleClickEvent(QMouseEvent *event) {
    int col = (event->pos().x() - 100) / 25;
    int row = event->pos().y() / fontMetrics().height();
    quint32 addr = m_baseAddr + row * m_bytesPerRow + col;
    emit memoryCellSelected(addr);
}
