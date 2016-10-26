#pragma once
#include <QMainWindow>
#include <memory>
#include <thread>
#include "ui_MainWindow.h"
#include <QPushButton>

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    std::unique_ptr<Ui::MainWindow> ui;
    std::unique_ptr<std::thread> wRThread;
    void windowResizeCheck();
    bool tokenLoadOK = false;
    bool resizeLock = false;
    bool stopAllWindowThread = false;
    //ui
    std::unique_ptr<QPushButton> SShotbutton;
private slots:
    void onBrowserLoadFinish(bool stat);
    void onBrowserWaitComplete();
    void onWindowResized();
    void saveScreenShot();
};