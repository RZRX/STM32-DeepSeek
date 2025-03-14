#pragma once
#include <QObject>
#include <QFutureWatcher>
#include "svd_parser.h"

struct ProjectConfig {
    QString name;
    QString mcuFamily;
    QString toolchainPath;
    QMap<QString, QString> sourceFiles;
};

class ProjectManager : public QObject {
    Q_OBJECT
public:
    explicit ProjectManager(QObject *parent = nullptr);
    bool createNewProject(const QString &path);
    void buildProject();
    ProjectConfig* currentProject() const;

signals:
    void buildFinished(bool success);
    void projectLoaded();

private slots:
    void onBuildProcessFinished();

private:
    void generateCMakeLists();
    void parseProjectFiles();
    
    ProjectConfig *currentProj;
    QFutureWatcher<void> buildWatcher;
    SVDParser svdParser;
};
