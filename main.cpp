#include <iostream>
#include <memory>
#include <QApplication>
#include <QQmlApplicationEngine>
#include <QWebEngineSettings>
#include "Window/MainWindow.h"

int main(int argc, char *argv[]){
    QApplication app(argc, argv);
    QWebEngineSettings::globalSettings()->setAttribute(QWebEngineSettings::PluginsEnabled, true);
    std::unique_ptr<MainWindow> mainWindow(new MainWindow());
    mainWindow->show();
    return app.exec();
}