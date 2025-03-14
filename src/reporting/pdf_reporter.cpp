#include "pdf_reporter.h"
#include <QPdfWriter>
#include <QPainter>

PDFReporter::PDFReporter(QObject *parent) 
    : QObject(parent), m_margin(20) {}

void PDFReporter::generateTestReport(const TestResults &results, const QString &path) {
    QPdfWriter writer(path);
    writer.setPageSize(QPageSize(QPageSize::A4));
    QPainter painter(&writer);
    
    painter.setFont(QFont("Arial", 12));
    painter.drawText(m_margin, m_margin + 20, "STM32 IDE Test Report");
    
    int yPos = m_margin + 50;
    for (const auto &[testName, status] : results) {
        painter.drawText(m_margin, yPos, 
            QString("%1: %2").arg(testName).arg(status ? "PASS" : "FAIL"));
        yPos += 30;
    }
    
    painter.end();
    emit reportGenerated(path);
}

void PDFReporter::generateMemoryReport(const QMap<QString, uint32_t> &usage) {
    QPdfWriter writer("memory_report.pdf");
    QPainter painter(&writer);
    
    painter.setPen(Qt::darkBlue);
    int yPos = m_margin;
    for (auto it = usage.begin(); it != usage.end(); ++it) {
        painter.drawText(m_margin, yPos, 
            QString("%1: %2 KB").arg(it.key()).arg(it.value() / 1024));
        yPos += 20;
    }
    
    painter.end();
}
