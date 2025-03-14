#include "main_window.h"
#include "plugin_loader.h"
#include "settings_manager.h"
#include <QApplication>

int main(int argc, char *argv[]) {
    QApplication app(argc, argv);
    
    // Инициализация ядра
    SettingsManager::instance()->loadSettings();
    DebugEngine::initHardware();
    
    // Создание главного окна
    MainWindow window;
    window.show();
    
    // Загрузка плагинов
    PluginLoader plugins;
    plugins.loadAll();
    
    // Восстановление сессии
    if (SettingsManager::instance()->restoreSession()) {
        window.openLastProject();
    }

    return app.exec();
}
