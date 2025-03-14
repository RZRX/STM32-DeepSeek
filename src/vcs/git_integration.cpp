#include "git_integration.h"
#include <QProcess>
#include <QStandardPaths>

GitManager::GitManager(QObject *parent) 
    : VCSProvider(parent), 
      m_gitPath(QStandardPaths::findExecutable("git")) {}

void GitManager::initRepository(const QString &path) {
    QProcess git;
    git.setWorkingDirectory(path);
    git.start(m_gitPath, {"init"});
    git.waitForFinished();
    emit repositoryChanged();
}

void GitManager::commitChanges(const QString &message) {
    QProcess git;
    git.setWorkingDirectory(m_currentPath);
    git.start(m_gitPath, {"commit", "-am", message});
    connect(&git, &QProcess::finished, 
            [this, &git](int code) { emit commitResult(code == 0); });
}

void GitManager::fetchHistory() {
    QProcess git;
    git.setWorkingDirectory(m_currentPath);
    git.start(m_gitPath, {"log", "--pretty=format:%h|%an|%ad|%s"});
    git.waitForFinished();
    
    QVector<GitCommit> history;
    for (const auto &line : git.readAllStandardOutput().split('\n')) {
        auto parts = line.split('|');
        if (parts.size() == 4) {
            history.append({parts[0], parts[1], QDateTime::fromString(parts[2]), parts[3]});
        }
    }
    emit historyFetched(history);
}
