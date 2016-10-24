#pragma once
#include <QMainWindow>
#include <memory>
#include "ui_MainWindow.h"

class MainWindow : public QMainWindow{
    Q_OBJECT
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
private:
    std::unique_ptr<Ui::MainWindow> ui;
    bool tokenLoadOK = false;
private slots:
    void onBrowserLoadFinish(bool stat);
    void onBrowserWaitComplete();
};