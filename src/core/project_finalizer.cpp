#include "project_finalizer.h"
#include <QDir>

void ProjectFinalizer::optimizeBinary(const QString &elfPath) {
    QProcess objcopy;
    objcopy.start("arm-none-eabi-objcopy", 
        {"-O", "binary", elfPath, elfPath + ".bin"});
    objcopy.waitForFinished();
    
    QProcess size;
    size.start("arm-none-eabi-size", {elfPath});
    size.waitForFinished();
    emit buildStats(size.readAllStandardOutput());
}

void ProjectFinalizer::generateMapFile(const QString &elfPath) {
    QProcess nm;
    nm.start("arm-none-eabi-nm", {"--size-sort", "--radix=d", elfPath});
    nm.waitForFinished();
    QFile mapFile(elfPath + ".map");
    mapFile.open(QIODevice::WriteOnly);
    mapFile.write(nm.readAllStandardOutput());
}

void ProjectFinalizer::cleanBuildArtifacts() {
    QDir buildDir("build");
    buildDir.removeRecursively();
}
