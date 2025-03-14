#include "memory_view.h"
#include <QScrollBar>
#include <QPainter>

MemoryView::MemoryView(QWidget *parent)
    : QAbstractScrollArea(parent), m_bytesPerLine(16) {
    
    setFont(QFont("Monospace", 10));
    m_addressColor = QColor(Qt::blue);
}

void MemoryView::setData(const QByteArray &data, quint32 baseAddress) {
    m_data = data;
    m_baseAddress = baseAddress;
    updateScrollBars();
    viewport()->update();
}

void MemoryView::paintEvent(QPaintEvent *event) {
    QPainter painter(viewport());
    painter.fillRect(event->rect(), Qt::white);

    const int lineHeight = fontMetrics().height();
    int y = 0;
    
    for (int i = 0; i < m_data.size(); i += m_bytesPerLine) {
        // Адрес
        painter.setPen(m_addressColor);
        painter.drawText(5, y + lineHeight, 
            QString("%1").arg(m_baseAddress + i, 8, 16, QLatin1Char('0')));
        
        // Данные
        painter.setPen(Qt::black);
        for (int j = 0; j < m_bytesPerLine; ++j) {
            if (i + j >= m_data.size()) break;
            QString byte = QString("%1")
                .arg((quint8)m_data[i + j], 2, 16, QLatin1Char('0'));
            painter.drawText(100 + j * 25, y + lineHeight, byte);
        }
        y += lineHeight;
    }
}
