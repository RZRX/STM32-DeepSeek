#pragma once
#include <QMainWindow>
#include <Qsci/qsciscintilla.h>
#include "project_manager.h"
#include "debug_engine.h"

class MainWindow : public QMainWindow {
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = nullptr);
    
private slots:
    void newProject();
    void openProject();
    void buildProject();
    void startDebugging();
    void updateMemoryView();

private:
    void createMenus();
    void initCodeEditor();
    void setupDockWidgets();
    
    ProjectManager *projectManager;
    DebugEngine *debugEngine;
    QsciScintilla *codeEditor;
    QTreeWidget *projectView;
    QDockWidget *memoryDock;
};
