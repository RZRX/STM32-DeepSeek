#include "build_system.h"
#include <QProcessEnvironment>

BuildSystem::BuildSystem(QObject *parent) 
    : QObject(parent), 
      m_buildType(Release),
      m_toolchain("ARM GCC") {}

void BuildSystem::configureProject(const QString &buildDir) {
    QProcess cmake;
    cmake.setWorkingDirectory(buildDir);
    
    QStringList args = {
        "-DCMAKE_TOOLCHAIN_FILE=" + toolchainPath(),
        "-DCMAKE_BUILD_TYPE=" + buildTypeToString(),
        "-GNinja",
        "../"
    };

    cmake.start("cmake", args);
    if (!cmake.waitForFinished(10000)) {
        emit buildError("CMake configuration timeout");
    }
    emit buildOutput(cmake.readAllStandardOutput());
}

QString BuildSystem::toolchainPath() const {
    if (m_toolchain == "ARM GCC") 
        return "cmake/arm-gcc.cmake";
    if (m_toolchain == "IAR") 
        return "cmake/iar.cmake";
    return "";
}

void BuildSystem::handleCompilerOutput() {
    static QRegularExpression errorRegex(R"((error|warning): (.*)\((\d+)\))");
    QProcess *proc = qobject_cast<QProcess*>(sender());
    QString output = proc->readAllStandardError();
    
    auto match = errorRegex.match(output);
    if (match.hasMatch()) {
        BuildIssue issue;
        issue.type = match.captured(1) == "error" ? Error : Warning;
        issue.message = match.captured(2);
        issue.line = match.captured(3).toInt();
        emit issueDetected(issue);
    }
}
