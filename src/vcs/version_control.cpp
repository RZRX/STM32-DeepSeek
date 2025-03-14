#include "version_control.h"
#include <QProcess>

void VersionControl::initializeRepo(const QString &path) {
    QProcess git;
    git.setWorkingDirectory(path);
    git.start("git", {"init"});
    git.waitForFinished();
    
    // .gitignore для STM32 проектов
    QFile gitignore(path + "/.gitignore");
    gitignore.open(QIODevice::WriteOnly);
    gitignore.write("build/\n*.elf\n*.hex\n*.bin\n");
}

void VersionControl::commitChanges(const QString &message) {
    QProcess git;
    git.setWorkingDirectory(m_projectPath);
    git.start("git", {"commit", "-am", message});
    connect(&git, &QProcess::finished, [this](int code) {
        emit commitResult(code == 0 ? "Success" : "Failed");
    });
}

QStringList VersionControl::getUncommittedFiles() {
    QProcess git;
    git.setWorkingDirectory(m_projectPath);
    git.start("git", {"status", "--porcelain"});
    git.waitForFinished();
    
    QStringList files;
    for (const auto &line : git.readAllStandardOutput().split('\n')) {
        if (!line.isEmpty()) files << line.mid(3);
    }
    return files;
}
