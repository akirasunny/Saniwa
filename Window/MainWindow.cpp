#include <iostream>
#include <thread>
#include <QFileDialog>
#include <QBrush>
#include <QPainter>
#include <QMessageBox>
#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "Browser/UserScriptManager.h"

using namespace std;

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),ui(new Ui::MainWindow){
    ui->setupUi(this);
    this->setWindowTitle(QString("Saniwa(Build:") + QString(__DATE__) + QString(u8" ") + QString(__TIME__) + QString(u8")"));
    connect(ui->webEngineView,SIGNAL(loadFinished(bool)),this,SLOT(onBrowserLoadFinish(bool)));
    //init js plugin
    UserScriptManager::addUserScript(&ui->webEngineView->page()->scripts());
    ui->webEngineView->page()->setUrl(QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=825012/"));
    wRThread = std::unique_ptr<std::thread>(new std::thread(&MainWindow::windowResizeCheck,this));
    //init statusbar button
    sShotButton = std::unique_ptr<QToolButton>(new QToolButton(ui->statusbar));
    sShotMenu = std::unique_ptr<QMenu>(new QMenu(this));
    sShotMenu->addAction(u8"名前を隠してスクショ",this,SLOT(saveScennShotWithoutName()));
    sShotButton->setMenu(sShotMenu.get());
    sShotButton->setPopupMode(QToolButton::MenuButtonPopup);
    sShotButton->setText(u8"スクショ");
    ui->statusbar->addWidget(sShotButton.get());
    connect(sShotButton.get(),SIGNAL(clicked()),this,SLOT(saveScreenShotWithName()));
    logoutButton = std::unique_ptr<QToolButton>(new QToolButton(ui->statusbar));
    logoutButton->setText(u8"ログアウト");
    ui->statusbar->addWidget(logoutButton.get());
    connect(logoutButton.get(),SIGNAL(clicked()),this,SLOT(logoutDMM()));
    reloadButton = std::unique_ptr<QToolButton>(new QToolButton(ui->statusbar));
    reloadButton->setText(u8"リロード");
    ui->statusbar->addWidget(reloadButton.get());
    connect(reloadButton.get(),SIGNAL(clicked()),this,SLOT(reloadPage()));
#ifndef __unix
    //not working on X11 env
    onTopButton = std::unique_ptr<QToolButton>(new QToolButton(ui->statusbar));
    onTopButton->setText(u8"最上面に表示");
    onTopButton->setCheckable(true);
    ui->statusbar->addWidget(onTopButton.get());
    connect(onTopButton.get(),SIGNAL(toggled(bool)),this,SLOT(enableOnTop(bool)));
#endif
	muteButton = std::unique_ptr<QToolButton>(new QToolButton(ui->statusbar));
	muteButton->setText(u8"消音");
	muteButton->setCheckable(true);
	ui->statusbar->addWidget(muteButton.get());
	connect(muteButton.get(), SIGNAL(toggled(bool)), this, SLOT(enableMute(bool)));
    this->resize(960, 580 + this->ui->statusbar->height() + 3);
    cout << "window initialized" << endl;
}

MainWindow::~MainWindow() {
    stopAllWindowThread = true;
    wRThread->detach();
}

void MainWindow::onBrowserLoadFinish(bool stat) {
    std::string url = ui->webEngineView->url().toString().toStdString();
    cout << "Load finished. " << url << endl;
    if(url == "http://www.dmm.com/netgame/social/-/gadgets/=/app_id=825012/"){
        tokenLoadOK = true;
        cout << "Game page load OK." << endl;
    }else if(url == "http://www.dmm.com/"){
        ui->webEngineView->page()->setUrl(QUrl("http://www.dmm.com/netgame/social/-/gadgets/=/app_id=825012/"));
    }
}

void MainWindow::onWindowResized() {
    double resizedHeight = (this->size().width() / 960.0) * 580.0;
    this->resize(this->size().width(),(int)resizedHeight + this->ui->statusbar->height() + 3);
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

void MainWindow::saveScreenShot(bool hideName) {
    QPixmap p = QPixmap::grabWidget(ui->webEngineView);
    QString savepath = QFileDialog::getSaveFileName(this,u8"スクショを保存","","PNG (*.png)");
    if(savepath.isEmpty()) return;
    if(!savepath.endsWith(".png")) savepath += ".png";
    std::thread([&]{
        //resize image
        p = p.scaledToWidth(960);
        QPixmap drawpixmap(p.size());
        QPainter painter(&drawpixmap);
        painter.drawPixmap(0,0,p);
        painter.setBrush(QBrush(QColor::fromRgb(171,53,62)));
        if(hideName) painter.drawRect(116,7,167,15);
        drawpixmap.save(savepath);
        cout << "took screenshot" << endl;
    }).join();
}

void MainWindow::logoutDMM() {
    tokenLoadOK = false;
    if(QMessageBox::question(this,"Saniwa",u8"本当に本丸からログアウトしますか?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No) == QMessageBox::Yes){
        ui->webEngineView->setUrl(QUrl(u8"https://www.dmm.com/my/-/login/logout/=/path=Sg__/"));
    }
}

void MainWindow::reloadPage() {
    if(QMessageBox::question(this,"Saniwa",u8"リロードしますか?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No) == QMessageBox::Yes){
        ui->webEngineView->stop();
        ui->webEngineView->reload();
    }
}

void MainWindow::enableOnTop(bool toggle) {
    if(!toggle){
        this->setWindowFlags(this->windowFlags() & ~Qt::WindowStaysOnTopHint);
        this->show();
    }else{
        this->setWindowFlags(this->windowFlags() | Qt::WindowStaysOnTopHint);
        this->show();
    }
}

void MainWindow::enableMute(bool toggle) {
	toggle ? ui->webEngineView->page()->setAudioMuted(true) : ui->webEngineView->page()->setAudioMuted(false);
}

void MainWindow::closeEvent(QCloseEvent *event) {
    if(QMessageBox::question(this,"Saniwa",u8"本当に終了しますか?",QMessageBox::Yes | QMessageBox::No,QMessageBox::No) == QMessageBox::Yes){
        event->accept();
    }else{
        event->ignore();
    }
}

void MainWindow::saveScreenShotWithName() {
    saveScreenShot();
}

void MainWindow::saveScennShotWithoutName() {
    saveScreenShot(true);
}
