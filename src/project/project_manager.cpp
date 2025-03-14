#include "project_manager.h"
#include <QDir>
#include <QFile>
#include <QProcess>

ProjectManager::ProjectManager(QObject *parent) 
    : QObject(parent), currentProj(nullptr) {}

bool ProjectManager::createNewProject(const QString &path) {
    QDir dir(path);
    if (!dir.mkpath("src") || !dir.mkpath("inc")) return false;
    
    currentProj = new ProjectConfig;
    currentProj->name = dir.dirName();
    currentProj->mcuFamily = "STM32F4";
    
    // Создание основных файлов
    QFile mainFile(path + "/src/main.c");
    if (mainFile.open(QIODevice::WriteOnly)) {
        mainFile.write("#include \"stm32f4xx.h\"\n\nint main() { while(1); }");
    }
    
    generateCMakeLists();
    emit projectLoaded();
    return true;
}

void ProjectManager::generateCMakeLists() {
    QString content = QString(
        "cmake_minimum_required(VERSION 3.20)\n"
        "project(%1)\n"
        "set(CMAKE_C_COMPILER arm-none-eabi-gcc)\n"
        "add_executable(${PROJECT_NAME}\n"
        "    src/main.c\n"
        ")\n"
        "target_include_directories(${PROJECT_NAME} PRIVATE inc)\n"
    ).arg(currentProj->name);
    
    QFile cmakeFile(currentProj->name + "/CMakeLists.txt");
    cmakeFile.open(QIODevice::WriteOnly);
    cmakeFile.write(content.toUtf8());
}

void ProjectManager::buildProject() {
    QFuture<void> future = QtConcurrent::run([this]() {
        QProcess cmake;
        cmake.setWorkingDirectory(currentProj->name);
        cmake.start("cmake", {"-Bbuild", "-GNinja"});
        cmake.waitForFinished();
        
        QProcess ninja;
        ninja.setWorkingDirectory(currentProj->name);
        ninja.start("ninja", {"-Cbuild"});
        emit buildFinished(ninja.waitForFinished());
    });
    buildWatcher.setFuture(future);
}
