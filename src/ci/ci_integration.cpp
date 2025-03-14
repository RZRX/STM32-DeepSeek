#include "ci_integration.h"
#include <QProcess>

CIIntegration::CIIntegration(QObject *parent)
    : QObject(parent), m_process(new QProcess(this)) {
    
    connect(m_process, &QProcess::readyReadStandardOutput, [this]() {
        emit buildOutput(m_process->readAllStandardOutput());
    });
}

void CIIntegration::runPipeline(const QString &configFile) {
    m_process->start("python3", {"ci_runner.py", configFile});
}

void CIIntegration::parseTestResults(const QString &xmlPath) {
    QFile file(xmlPath);
    if (!file.open(QIODevice::ReadOnly)) return;
    
    QXmlStreamReader xml(&file);
    while (!xml.atEnd()) {
        if (xml.isStartElement() && xml.name() == "testcase") {
            TestResult result;
            result.name = xml.attributes().value("name").toString();
            result.status = xml.attributes().value("status").toString() == "passed";
            m_results.append(result);
        }
        xml.readNext();
    }
    emit testResultsReady(m_results);
}

void CIIntegration::generateJUnitReport(const QString &path) {
    QFile report(path);
    report.open(QIODevice::WriteOnly);
    QXmlStreamWriter xml(&report);
    
    xml.writeStartDocument();
    xml.writeStartElement("testsuite");
    for (const auto &result : m_results) {
        xml.writeStartElement("testcase");
        xml.writeAttribute("name", result.name);
        xml.writeAttribute("status", result.status ? "passed" : "failed");
        xml.writeEndElement();
    }
    xml.writeEndElement();
    xml.writeEndDocument();
}
