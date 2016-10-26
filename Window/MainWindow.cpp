#include <iostream>
#include <thread>
#include "MainWindow.h"
#include "ui_MainWindow.h"

using namespace std;
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle("Saniwa");
    connect(ui->webEngineView,SIGNAL(loadFinished(bool)),this,SLOT(onBrowserLoadFinish(bool)));
    ui->webEngineView->setUrl(QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=825012/"));
    wRThread = std::unique_ptr<std::thread>(new std::thread(&MainWindow::windowResizeCheck,this));
}
MainWindow::~MainWindow() {
    stopAllWindowThread = true;
    wRThread->detach();
}
void MainWindow::onBrowserLoadFinish(bool stat) {
    std::string url = ui->webEngineView->url().toString().toStdString();
    cout << "Load finished. " << url << endl;
    if(url == u8"http://www.dmm.com/netgame/social/-/gadgets/=/app_id=825012/"){
        tokenLoadOK = true;
        cout << "Game page load OK." << endl;
        ui->webEngineView->page()->runJavaScript("$(\"#game_frame\").attr(\"src\")",[this](const QVariant &v) {
            cout << "iframe url " << v.toString().toStdString() << endl;
            ui->webEngineView->setUrl(QUrl(v.toString()));
        });
    }else if(tokenLoadOK && ui->webEngineView->url().authority().toStdString() == u8"osapi.dmm.com"){
        std::thread th([this](){
            cout << "sleep start" << endl;
            std::this_thread::sleep_for(std::chrono::seconds(3));
            //call slot on main window thread
            QMetaObject::invokeMethod(this,"onBrowserWaitComplete");
        });
        th.detach();
    }
}

void MainWindow::onBrowserWaitComplete() {
    if(ui == nullptr) return;
    ui->webEngineView->page()->runJavaScript("document.body.style.overflow = \"hidden\"");
    ui->webEngineView->page()->runJavaScript("$(function(){$('#flash_object').css({'position': 'fixed','left': '0','width': '100%','height': '100%','z-index': '1000000'})});",[this](const QVariant &v){
        cout << v.toString().toStdString() << endl;
    });
    this->resize(960,580 + this->ui->statusbar->height());
}

void MainWindow::onWindowResized() {
    double resizedHeight = (this->size().width() / 960.0) * (580.0 + (double)this->ui->statusbar->height());
    this->resize(this->size().width(),(int)resizedHeight);
}

void MainWindow::windowResizeCheck() {
    QSize lastSize;
    while (!stopAllWindowThread){
        lastSize = this->size();
        std::this_thread::sleep_for(std::chrono::seconds(1));
        if(tokenLoadOK && (lastSize.width() != this->size().width() || lastSize.height() != this->size().height())){
            //call slot on main window thread
            QMetaObject::invokeMethod(this,"onWindowResized");
        }
    }
}