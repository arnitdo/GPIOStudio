#include "main.hpp"

//begin Qt imports

#include <QWidget>
#include <QString>
#include <QImage>
#include <QMenuBar>
#include <QMenu>
#include <QIcon>
#include <QMouseEvent>
#include <QApplication>
#include <QMap>
#include <QEvent>
#include <QAction>
#include <QObject>
#include <QGridLayout>
#include <QTextEdit>
#include <QGridLayout>
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QScrollArea>
#include <QPushButton>

//end Qt imports
//begin system imports

#include <string>
#include <iostream>

//end system imports

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define REVISION 1

/*
  ____ _____ _   _ _____ ____  ___ ____
 / ___| ____| \ | | ____|  _ \|_ _/ ___|
| |  _|  _| |  \| |  _| | |_) || | |
| |_| | |___| |\  | |___|  _ < | | |___
 \____|_____|_| \_|_____|_| \_|___\____| */


QString getVersionInfo(){
	std::string VInfo = "GPIO Studio v" + std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(REVISION);
	return QString(VInfo.c_str());
}

QString convertToQString(std::string in){
	return QString(in.c_str());
}

/*
 __  __    _    ___ _   ___        _____ _   _ ____   _____        __
|  \/  |  / \  |_ _| \ | \ \      / |_ _| \ | |  _ \ / _ \ \      / /
| |\/| | / _ \  | ||  \|  \ \ /\ / / | ||  \| | | | | | | \ \ /\ / /
| |  | |/ ___ \ | || |\    \ V  V /  | || |\  | |_| | |_| |\ V  V /
|_|  |_/_/   \_|___|_| \_   \_/\_/  |___|_| \_|____/ \___/  \_/\_/ */

MainWindow::MainWindow() : 
	MainWindowDrawArea (this),
	MainWindowConsole (this),
	MainWindowScrollArea(this),
	MainWindowClearButton("Clear", this)	
{;
	QGridLayout* MainWindowLayout = new QGridLayout(this);
	MainWindowDrawArea.setFixedSize(2560, 1440);
	MainWindowScrollArea.setWidget(&MainWindowDrawArea);
	MainWindowScrollArea.setFixedSize(768, 592);
	MainWindowConsole.setReadOnly(true);
	MainWindowConsole.setFixedSize(1260, 72);
	MainWindowConsole.setStyleSheet("background-color : #dddddd;");
	MainWindowConsole.append("Welcome to " + getVersionInfo());
	MainWindowLayout->addWidget(&MainWindowScrollArea, 1, 2);
	MainWindowLayout->addWidget(&MainWindowConsole, 9, 0);
	this->setWindowTitle(getVersionInfo());
	this->setFixedSize(1280, 720);
}

void MainWindow::log(std::string value){
	this->MainWindowConsole.append(convertToQString(value));
}

void MainWindow::warn(std::string value){
	this->MainWindowConsole.setTextColor(QColor("black"));
	this->MainWindowConsole.append(convertToQString(value));
}

/*  
____  ____     ___        _    _    ____  _____    _
|  _ \|  _ \   / \ \      /    / \  |  _ \| ____|  / \
| | | | |_) | / _ \ \ /\ / /  / _ \ | |_) |  _|   / _ \
| |_| |  _ < / ___ \ V  V /  / ___ \|  _ <| |___ / ___ \
|____/|_| \_/_/   \_\_/\_/  /_/   \_|_| \_|_____/_/   \_\ */

DrawArea::DrawArea(MainWindow *parent) : QWidget(parent){
	this->MainWindowParent = parent;
	this->setStyleSheet("background-color : white;");
}

void DrawArea::mousePressEvent(QMouseEvent *event){
	QRect Qr = QRect(QPoint(event->x(), event->y()), QPoint(event->x() + 100, event->y() + 100));
	QWidget* w = new GPIODevice(this, MainWindowParent);
	w->setGeometry(Qr);
	w->setStyleSheet("border : 2px solid red; background-color : ");
	w->show();
}

void DrawArea::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

/* 
  ____ ____ ___ ___
 / ___|  _ |_ _/ _ \
| |  _| |_) | | | | |
| |_| |  __/| | |_| |
 \____|_|  |___\___/ */


void GPIODevice::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter p(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, &p, this);
}

GPIODevice::GPIODevice(DrawArea* parent, MainWindow* parentMainWindow) : QWidget(parent){
	this->ParentMainWindow = parentMainWindow;
	QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
	this->ParentMainWindow->warn("Inserting text");
}

void GPIODevice::deleteSelf(){
	this->ParentMainWindow->MainWindowConsole.setTextColor(QColor("red"));
	this->ParentMainWindow->log("Deleting self");
	delete this;
}
/*
 __  __    _    ___ _   _
|  \/  |  / \  |_ _| \ | |
| |\/| | / _ \  | ||  \| |
| |  | |/ ___ \ | || |\  |
|_|  |_/_/   \_|___|_| \_| */


int main(int argc, char** argv){
	QApplication app (argc, argv);
	MainWindow w;
	w.show();
	return app.exec();
}