#ifdef _MSC_VER

#include <windows.h>
#include <iostream>
#include <memory>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QWebEngineSettings>
#include "Window/MainWindow.h"

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine,int nShowCmd) {
    QApplication app(__argc, __argv);
    Q_INIT_RESOURCE(resources);
    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    std::unique_ptr<MainWindow> mainWindow(new MainWindow());
    mainWindow->show();
    return app.exec();
}

#endif // only on MSVC