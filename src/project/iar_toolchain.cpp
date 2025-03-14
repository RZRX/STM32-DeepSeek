#include "iar_toolchain.h"
#include <QProcess>

IAREmbeddedWorkbench::IAREmbeddedWorkbench(QObject *parent)
    : Toolchain("IAR EWARM", parent), 
      m_installPath("C:/Program Files/IAR Systems/Embedded Workbench 9.0") {}

bool IAREmbeddedWorkbench::buildProject(const QString &projectFile) {
    QProcess iarbuild;
    iarbuild.setProgram(m_installPath + "/common/bin/iarbuild.exe");
    iarbuild.setArguments({projectFile, "-build", "Debug"});
    
    connect(&iarbuild, &QProcess::readyReadStandardOutput, [this, &iarbuild]() {
        emit buildOutput(iarbuild.readAllStandardOutput());
    });
    
    return iarbuild.execute() == 0;
}

void IAREmbeddedWorkbench::parseErrorOutput(const QString &output) {
    static QRegularExpression iarError(R"(Error\[(.*)\]: (.*) in (.*) line (\d+))");
    auto match = iarError.match(output);
    if (match.hasMatch()) {
        emit errorDetected({
            .file = match.captured(3),
            .line = match.captured(4).toInt(),
            .message = match.captured(2)
        });
    }
}
