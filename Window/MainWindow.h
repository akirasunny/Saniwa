#pragma once
#include <QMainWindow>
#include <memory>
#include <thread>
#include "ui_MainWindow.h"
#include <QToolButton>
#include <QCloseEvent>

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
    std::unique_ptr<QToolButton> sShotButton;
    std::unique_ptr<QToolButton> logoutButton;
    std::unique_ptr<QToolButton> reloadButton;
    std::unique_ptr<QToolButton> onTopButton;
	std::unique_ptr<QToolButton> muteButton;
    void closeEvent(QCloseEvent *event) override;
private slots:
    void onBrowserLoadFinish(bool stat);
    void onBrowserWaitComplete();
    void onWindowResized();
    void saveScreenShot();
    void logoutDMM();
    void reloadPage();
    void enableOnTop(bool toggle);
	void enableMute(bool toggle);
};