#include "main_window.h"
#include <QMenuBar>
#include <QStatusBar>
#include <QFileDialog>
#include <QMessageBox>

MainWindow::MainWindow(QWidget *parent) 
    : QMainWindow(parent),
      projectManager(new ProjectManager(this)),
      debugEngine(new DebugEngine(this)) {
    
    resize(1280, 720);
    setWindowTitle("STM32 IDE");
    
    createMenus();
    initCodeEditor();
    setupDockWidgets();
}

void MainWindow::createMenus() {
    QMenu *fileMenu = menuBar()->addMenu("File");
    fileMenu->addAction("New Project", this, &MainWindow::newProject);
    fileMenu->addAction("Open Project", this, &MainWindow::openProject);
    
    QMenu *buildMenu = menuBar()->addMenu("Build");
    buildMenu->addAction("Build", this, &MainWindow::buildProject);
    
    QMenu *debugMenu = menuBar()->addMenu("Debug");
    debugMenu->addAction("Start", this, &MainWindow::startDebugging);
}

void MainWindow::initCodeEditor() {
    codeEditor = new QsciScintilla(this);
    codeEditor->setLexer(new QsciLexerCPP);
    codeEditor->setAutoIndent(true);
    codeEditor->setMarginLineNumbers(1, true);
    setCentralWidget(codeEditor);
}

void MainWindow::newProject() {
    QString path = QFileDialog::getExistingDirectory(this, "Select Project Folder");
    if (!path.isEmpty()) {
        projectManager->createNewProject(path);
    }
}

void MainWindow::buildProject() {
    if (projectManager->currentProject()) {
        connect(projectManager, &ProjectManager::buildFinished, 
                this, [this](bool success) {
                    statusBar()->showMessage(success ? "Build OK" : "Build Failed");
                });
        projectManager->buildProject();
    }
}
