#include "main.hpp"
#include "thirdparty/nlohmann/json.hpp"

using json = nlohmann::json;

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
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>
#include <QComboBox>

//end Qt imports
//begin system imports

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

//end system imports

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define REVISION 2

namespace Counters{
	int BUZZERCount = 1;
	int LEDCount = 1;
	int LEDCTRLCount = 1;
	int BUZZERCTRLCount = 1;
	int SLEEPCount = 1;
	int BUTTONCount = 1;
	int FUNCTIONCount = 1;
	void reset(){
		BUZZERCount = 1;
		LEDCount = 1;
		LEDCTRLCount = 1;
		BUZZERCTRLCount = 1;
		SLEEPCount = 1;
		BUTTONCount = 1;
		FUNCTIONCount = 1;
	}
}
/*
  ____ _____ _   _ _____ ____  ___ ____
 / ___| ____| \ | | ____|  _ \|_ _/ ___|
| |  _|  _| |  \| |  _| | |_) || | |
| |_| | |___| |\  | |___|  _ < | | |___
 \____|_____|_| \_|_____|_| \_|___\____| */


QString getVersionInfo(){
	std::string VInfo = std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(REVISION);
	return QString(VInfo.c_str());
}

QString convertToQString(std::string in){
	return QString(in.c_str());
}

std::string convertToStdString(QString in){
	return in.toUtf8().constData();
}

/*
 __  __    _    ___ _   ___        _____ _   _ ____   _____        __
|  \/  |  / \  |_ _| \ | \ \      / |_ _| \ | |  _ \ / _ \ \      / /
| |\/| | / _ \  | ||  \|  \ \ /\ / / | ||  \| | | | | | | \ \ /\ / /
| |  | |/ ___ \ | || |\    \ V  V /  | || |\  | |_| | |_| |\ V  V /
|_|  |_/_/   \_|___|_| \_   \_/\_/  |___|_| \_|____/ \___/  \_/\_/ */

MainWindow::MainWindow(QApplication* parentApplication) : 
	MainWindowDrawArea (this),
	MainWindowConsole (this),
	MainWindowScrollArea(this),
	MainWindowGPIOScrollArea(this),
	MainWindowGPIOToolBar(&MainWindowGPIOScrollArea, this),
	MainWindowClearButton(this),
	MainWindowBuildButton(this),
	MainWindowBRComboButton(this),
	MainWindowQuitButton(this),
	MainWindowRefreshButton(this),
	MainWindowLayout(this)
{
	this->ParentApp = parentApplication;
	// Base layout for MainWindow
	MainWindowLayout.setSpacing(0);
	MainWindowLayout.setMargin(0);
	
	// MainWindowDrawArea, MainWindowScrollArea
	MainWindowDrawArea.setFixedSize(5120, 2880);
	MainWindowScrollArea.setWidget(&MainWindowDrawArea);
	MainWindowScrollArea.setFixedSize(1024, 612);
	MainWindowLayout.addWidget(&MainWindowScrollArea, 1, 2, 8, 8);
	
	// MainWindowConsole
	MainWindowConsole.setReadOnly(true);
	MainWindowConsole.setFixedSize(1280, 72);
	MainWindowConsole.setStyleSheet("background-color : #dddddd;");
	this->log("Welcome to " + convertToStdString("GPIO Studio v" + getVersionInfo()));
	MainWindowLayout.addWidget(&MainWindowConsole, 9, 0, 1, 10);

	// MainWindowGPIOScrollArea;
	MainWindowGPIOScrollArea.setFixedSize(256, 612);
	MainWindowGPIOScrollArea.setWidget(&MainWindowGPIOToolBar);
	MainWindowLayout.addWidget(&MainWindowGPIOScrollArea, 1, 0, 2, 8);

	// Finishing up
	this->setWindowTitle("GPIO Studio v" + getVersionInfo());
	this->setFixedSize(1280, 720);
	MainWindowClearButton.setFixedHeight(36);
	MainWindowClearButton.setText(" Clear All!");
	MainWindowClearButton.setIcon(QIcon("static/clear.svg"));
	MainWindowClearButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	MainWindowLayout.addWidget(&MainWindowClearButton, 0, 0, 1, 2);

	MainWindowBuildButton.setFixedHeight(36);
	MainWindowBuildButton.setText(" Build!");
	MainWindowBuildButton.setIcon(QIcon("static/hammer.svg"));
	MainWindowBuildButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
	MainWindowLayout.addWidget(&MainWindowBuildButton, 0, 2, 1, 2);

	MainWindowBRComboButton.setFixedHeight(36);
	MainWindowBRComboButton.setText(" Build and Run!");
	MainWindowBRComboButton.setIcon(QIcon("static/run-build.svg"));
	MainWindowBRComboButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
	MainWindowLayout.addWidget(&MainWindowBRComboButton, 0, 4, 1 ,2);

	MainWindowQuitButton.setFixedHeight(36);
	MainWindowQuitButton.setText(" Quit Application!");
	MainWindowQuitButton.setIcon(QIcon("static/logout.svg"));
	MainWindowQuitButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
	MainWindowLayout.addWidget(&MainWindowQuitButton, 0, 8, 1 ,2);

	MainWindowRefreshButton.setFixedHeight(36);
	MainWindowRefreshButton.setText(" Refresh!");
	MainWindowRefreshButton.setIcon(QIcon("static/refresh.svg"));
	MainWindowRefreshButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
	MainWindowLayout.addWidget(&MainWindowRefreshButton, 0, 6, 1 ,2);

	QObject::connect(&MainWindowClearButton, SIGNAL (clicked()), this, SLOT (resetDrawArea()));
	QObject::connect(&MainWindowBRComboButton, SIGNAL (clicked()), this, SLOT (buildAndRun()));
	QObject::connect(&MainWindowQuitButton, SIGNAL (clicked()), this, SLOT (QuitApp()));
	QObject::connect(&MainWindowRefreshButton, SIGNAL (clicked()), this, SLOT (RefreshDrawSelects()));
}

MainWindow::~MainWindow(){
	std::ofstream logFile ("GPIOStudio.log");
	this->log("Qutting Application!");
	logFile << convertToStdString(this->MainWindowConsole.toPlainText());
	logFile.close();
}

void MainWindow::log(std::string value){
	this->MainWindowConsole.setTextColor(QColor("black"));
	this->MainWindowConsole.append("LOG : " + convertToQString(value));
}

void MainWindow::warn(std::string value){
	this->MainWindowConsole.setTextColor(QColor("#E34234"));
	this->MainWindowConsole.append("WARNING : " + convertToQString(value));
}

void MainWindow::debug(std::string value){
	this->MainWindowConsole.setTextColor(QColor("green"));
	this->MainWindowConsole.append("DEBUG : " + convertToQString(value));
}

void MainWindow::err(std::string value){
	this->MainWindowConsole.setTextColor(QColor("red"));
	this->MainWindowConsole.append("ERROR : " + convertToQString(value));
}

void MainWindow::resetDrawArea(){
	this->MainWindowDrawArea.resetSelf();
}

void MainWindow::buildAndRun(){
	this->MainWindowBuildButton.click();
#ifndef __linux__
	this->warn("The device you are currently using is not a Raspberry Pi.");
	this->warn("The project has been built, but will not execute. Transfer the generated script.py file to your Raspberry Pi.");
#endif
#ifdef __linux__
	system("python3 script.py");
#endif
}

void MainWindow::QuitApp(){
	this->ParentApp->quit();
}

void MainWindow::RefreshDrawSelects(){
	this->MainWindowDrawArea.RefreshSelects();
}

/*  
____  ____     ___        _    _    ____  _____    _
|  _ \|  _ \   / \ \      /    / \  |  _ \| ____|  / \
| | | | |_) | / _ \ \ /\ / /  / _ \ | |_) |  _|   / _ \
| |_| |  _ < / ___ \ V  V /  / ___ \|  _ <| |___ / ___ \
|____/|_| \_/_/   \_\_/\_/  /_/   \_|_| \_|_____/_/   \_\ */

DrawArea::DrawArea(MainWindow *parent) : 
	QWidget(parent){
	this->ParentMainWindow = parent;
	this->setStyleSheet("background-color : white;");
	ButtonLabelMap.insert({1, "Simple LED"});
	ButtonLabelMap.insert({2, "Simple Buzzer"});
	ButtonLabelMap.insert({3, "LED Controls"});
	ButtonLabelMap.insert({4, "Buzzer Controls"});
	ButtonLabelMap.insert({5, "Sleep Timer"});
	ButtonLabelMap.insert({6, "Simple Button"});
	ButtonLabelMap.insert({7, "Custom Function"});
}

void DrawArea::mousePressEvent(QMouseEvent *event){
	if (this->NWMode){
		if (this->isNew){
			QRect StartBox = QRect(0, 0, 200, 100);
			ProgramStart* PStart = new ProgramStart(this, this->ParentMainWindow);
			this->ProgStart = PStart;
			PStart->setGeometry(StartBox);
			PStart->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(PStart->Color) + ";");
			this->isNew = false;
			PStart->show();
			GPIOCodeVector.push_back(PStart);
			this->LastPoint = QPoint(200, 50);
			this->CurrentPoint = QPoint(event->x(), event->y() + 50);
		} else {
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->CurrentPoint = QPoint(event->x(), event->y() + 50);
		}
		this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
		this->createGPIODevice(this->activeGPIO, event->x(), event->y());
		this->setStyleSheet("background-color : #ffffff;");
		this->NWMode = false;
	}
}

void DrawArea::createGPIODevice(int active, int X, int Y){
	// Constructs a GPIO Device at the given coordinates;
	this->activeGPIO = active;
	switch(this->activeGPIO){
		case 1:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			LED* GPIOD = new LED(this, ParentMainWindow, X, Y, ("LED " + std::to_string(Counters::LEDCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->GPIOCodeVector.push_back(GPIOD);
			this->LEDVec.push_back(GPIOD);
			break;
		}
		case 2:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Buzzer* GPIOD = new Buzzer(this, ParentMainWindow, X, Y, ("Buzzer " + std::to_string(Counters::BUZZERCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->GPIOCodeVector.push_back(GPIOD);
			this->BUZVec.push_back(GPIOD);
			break;
		}
		case 3:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			LEDCtrl* GPIOD = new LEDCtrl(this, ParentMainWindow, X, Y, ("LED Controls " + std::to_string(Counters::LEDCTRLCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->GPIOCodeVector.push_back(GPIOD);
			this->LEDCTRLVec.push_back(GPIOD);
			this->RefreshSelects();
			break;
		}
		case 4:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			BuzzerCtrl* GPIOD = new BuzzerCtrl(this, ParentMainWindow, X, Y, ("Buzzer Controls " + std::to_string(Counters::BUZZERCTRLCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->GPIOCodeVector.push_back(GPIOD);
			this->BUZCTRLVec.push_back(GPIOD);
			this->RefreshSelects();
			break;
		}
		case 5:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Sleep* GPIOD = new Sleep(this, ParentMainWindow, X, Y, ("Sleep Timer " + std::to_string(Counters::SLEEPCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
		case 6:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Button* GPIOD = new Button(this, ParentMainWindow, X, Y, ("Button " + std::to_string(Counters::BUTTONCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
		case 7:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Function* GPIOD = new Function(this, ParentMainWindow, X, Y, ("Function " + std::to_string(Counters::FUNCTIONCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->FUNCVec.push_back(GPIOD);
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
	}	
}

void DrawArea::paintEvent(QPaintEvent* event){
	QStyleOption* opt = new QStyleOption;
	QPainter* p = new QPainter(this);
	p->setCompositionMode(QPainter::CompositionMode_Source);
    opt->init(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, opt, p, this);
	p->setRenderHint(QPainter::Antialiasing);
	if(!this->isNew){
		for (std::pair<QPoint, QPoint> PointPair : this->Lines){
			p->setPen(QPen(Qt::black, 2));
			p->drawLine(std::get<0>(PointPair), std::get<1>(PointPair));
		};
	}
	p->end();
	event = event;
	delete p;
	delete opt;
}

void DrawArea::OnGPIODeviceSignal(int GPIOID){
	this->activeGPIO = GPIOID;
	this->NWMode = true;
}

void DrawArea::resetSelf(){
	this->Lines.clear();
	this->setStyleSheet("background-color : #ffffff;");
	this->isNew = true;	
	this->ParentMainWindow->log("Resetting Project!");	
	this->GPIOCodeVector.clear();
	this->BUZVec.clear();
	this->LEDVec.clear();
	this->LEDCTRLVec.clear();
	this->BUZCTRLVec.clear();
	this->FUNCVec.clear();
	Counters::reset();
}

void DrawArea::RefreshSelects(){
	QStringList LEDNames;
	QStringList BuzzerNames;
	QStringList FuncNames;
	// LEDCtrl Refresh

	for (LED* Led : this->LEDVec){
		LEDNames << Led->VarnameEdit.text();
	}
	for (LEDCtrl* LedControl : this->LEDCTRLVec){
		LedControl->LEDSelect.clear();
		LedControl->LEDSelect.insertItems(0, LEDNames);
		LedControl->LEDSelect.setMaxCount(LedControl->LEDSelect.count());
	}

	// BuzzerCtrl Refresh
	for (Buzzer* Buzzer : this->BUZVec){
		BuzzerNames << Buzzer->VarnameEdit.text();
	}
	for (BuzzerCtrl* BuzControl : this->BUZCTRLVec){
		BuzControl->BuzzerSelect.clear();
		BuzControl->BuzzerSelect.insertItems(0, BuzzerNames);
		BuzControl->BuzzerSelect.setMaxCount(BuzControl->BuzzerSelect.count());
	}

	// Function Refresh
	for (Function* F : this->FUNCVec){
		FuncNames << F->NameEdit.text();
	}
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
    QPainter* p = new QPainter(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, p, this);
	p->end();
	delete p;
}

GPIODevice::GPIODevice(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) : QWidget(parent){
	this->ParentMainWindow = parentMainWindow;
	this->XCoord = X;
	this->YCoord = Y;
	this->GPIOName = name;
	QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
}

void GPIODevice::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string GPIODevice::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return "# " + this->GPIOName + "\n";
}

LED::LED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("Name : ", this){
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		for (int i = 2; i < 28; i++){
			PinSelect.addItem(convertToQString(std::to_string(i)));
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : #cceecc;");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyLED" + convertToQString(std::to_string(Counters::LEDCount)));
		this->VarnameEdit.setStyleSheet("background-color : #cceecc;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::LEDCount++;
}

void LED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string LED::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (!(this->PinSelect.currentText() == "" || this->VarnameEdit.text() == "")){
		return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.LED(" + convertToStdString(this->PinSelect.currentText()) +
		")\n";
	} else {
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No suitable variable name or pin number provided\n";
	}
}

Buzzer::Buzzer(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) : 
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("State : ", this)
	{
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		for (int i = 2; i < 28; i++){
			PinSelect.addItem(convertToQString(std::to_string(i)));
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : #cceecc;");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyBuzzer" + convertToQString(std::to_string(Counters::BUZZERCount)));
		this->VarnameEdit.setStyleSheet("background-color : #cceecc;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		Counters::BUZZERCount++;
		QObject::connect(&this->ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT(deleteSelf()));
}

void Buzzer::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string Buzzer::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (!(this->PinSelect.currentText() == "" || this->VarnameEdit.text() == "")){
		return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.Buzzer(" + convertToStdString(this->PinSelect.currentText()) +
		")\n";
	} else {
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No suitable variable name or pin number provided\n";
	}
}

LEDCtrl::LEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	LEDSelect(this),
	StateSelect(this),
	DisplayLabel(convertToQString(name), this),
	LEDLabel("LED : ", this),
	StateLabel("Pin State : ", this)
	{
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		LEDLabel.setStyleSheet("border : 0px;");
		StateSelect.addItem("on");
		StateSelect.addItem("off");
		StateLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&LEDLabel, 1, 1);
		this->SelfLayout.addWidget(&this->LEDSelect, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 2, 1);
		this->SelfLayout.addWidget(&this->StateSelect, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::LEDCTRLCount++;
	}

void LEDCtrl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string LEDCtrl::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (this->LEDSelect.currentText() == ""){
		this->ParentMainWindow->err("No LED Selected for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No LED Selected\n";
	} else {
		return convertToStdString(this->LEDSelect.currentText()) + "." + convertToStdString(this->StateSelect.currentText()) + "()\n";
	}
}
BuzzerCtrl::BuzzerCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	BuzzerSelect(this),
	StateSelect(this),
	DisplayLabel(convertToQString(this->GPIOName), this),
	BuzzerLabel("Buzzer : ", this),
	StateLabel("Pin State : ", this)
	{
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		BuzzerLabel.setStyleSheet("border : 0px;");
		StateSelect.addItem("on");
		StateSelect.addItem("off");
		StateLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&BuzzerLabel, 1, 1);
		this->SelfLayout.addWidget(&this->BuzzerSelect, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 2, 1);
		this->SelfLayout.addWidget(&this->StateSelect, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::BUZZERCTRLCount++;
	}

void BuzzerCtrl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string BuzzerCtrl::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (this->BuzzerSelect.currentText() == ""){
		this->ParentMainWindow->err("No Buzzer Selected for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No Buzzer Selected\n";
	} else {
		return convertToStdString(this->BuzzerSelect.currentText()) + "." + convertToStdString(this->StateSelect.currentText()) + "()\n";
	}
}

Sleep::Sleep(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DurationEdit(this),
	DisplayLabel(convertToQString(name), this),
	DurationLabel("Duration : ", this){
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		DurationLabel.setStyleSheet("border : 0px;");
		// DisplayLabel->setText("MyLED" + convertToQString(std::to_string(LEDCount)));
		this->DurationEdit.setStyleSheet("background-color : #FFFFE0;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&DurationLabel, 1, 1);
		this->SelfLayout.addWidget(&this->DurationEdit, 1, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::SLEEPCount++;
}

void Sleep::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string Sleep::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (this->DurationEdit.text() == ""){
		this->ParentMainWindow->err("No Sleep Duration specified for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No sleep duration specified\n";
	} else {
		return "time.sleep(" + convertToStdString(this->DurationEdit.text()) + ")\n";
	}
}
Button::Button(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("Name : ", this){
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		for (int i = 2; i < 28; i++){
			PinSelect.addItem(convertToQString(std::to_string(i)));
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : #cceecc;");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyButton" + convertToQString(std::to_string(Counters::BUTTONCount)));
		this->VarnameEdit.setStyleSheet("background-color : #cceecc;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::BUTTONCount++;
}

void Button::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string Button::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (!(this->PinSelect.currentText() == "" || this->VarnameEdit.text() == "")){
		return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.Button(" + convertToStdString(this->PinSelect.currentText()) +
		")\n";
	} else {
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No suitable variable name or pin number provided\n";
	}
}

Function::Function(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	NameLabel("Name : ", this),
	NameEdit(this),
	BodyLabel("Body : ", this),
	BodyButton("Edit Function Body", this){
		QWidget* BodyWindow = new QWidget;
		QTextEdit* FunctionBodyEdit = new QTextEdit(BodyWindow);
		QPushButton* CBody = new QPushButton("Close Window", BodyWindow);
		BodyWindow->setWindowFlags(Qt::WindowMaximizeButtonHint);
		BodyWindow->setFixedSize(640, 480);
		FunctionBodyEdit->setGeometry(50, 20, 540, 380);
		FunctionBodyEdit->show();
		this->FunctionBodyWindow = BodyWindow;
		this->FunctionBody = FunctionBodyEdit;
		this->CloseBodyButton = CBody;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		NameLabel.setStyleSheet("border : 0px;");
		BodyLabel.setStyleSheet("border : 0px;");
		BodyLabel.setFixedHeight(20);
		BodyButton.setFixedHeight(20);
		NameEdit.setStyleSheet("background-color : #CCFFD3;");
		NameEdit.setText(convertToQString("MyFunction" + std::to_string(Counters::FUNCTIONCount)));
		BodyButton.setStyleSheet("background-color : #CCFFD3;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 1, 1);
		this->SelfLayout.addWidget(&NameEdit, 1, 2);
		this->SelfLayout.addWidget(&BodyLabel, 2, 1);
		this->SelfLayout.addWidget(&BodyButton, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT(deleteSelf()));
		QObject::connect(&BodyButton, SIGNAL (clicked()), this, SLOT (showBodyWindow()));
		QObject::connect(CBody, SIGNAL (clicked()), this, SLOT (hideBodyWindow()));
		Counters::FUNCTIONCount++;
}

void Function::showBodyWindow(){
	this->FunctionBodyWindow->show();
};

void Function::hideBodyWindow(){
	this->FunctionBodyWindow->hide();
};

void Function::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string Function::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string out, templine;
	this->FunctionText << convertToStdString(this->FunctionBody->toPlainText());
	out = "def " + convertToStdString(this->NameEdit.text()) + "():\n";
	while (std::getline(this->FunctionText, templine)){
		out += "\t" + templine + "\n";
	}
	return out;
}

/*  
  ____ ____ ___ ___ _____ ___   ___  _     ____    _    ____
 / ___|  _ |_ _/ _ |_   _/ _ \ / _ \| |   | __ )  / \  |  _ \
| |  _| |_) | | | | || || | | | | | | |   |  _ \ / _ \ | |_) |
| |_| |  __/| | |_| || || |_| | |_| | |___| |_) / ___ \|  _ <
 \____|_|  |___\___/ |_| \___/ \___/|_____|____/_/   \_|_| \_\ */

 GPIOToolBar::GPIOToolBar(QWidget* parent, MainWindow* parentMainWindow) : 
 	QWidget(parent),
	GPIOToolBarLayout(this){
	this->ParentMainWindow = parentMainWindow;
	this->MainWindowDrawArea = &this->ParentMainWindow->MainWindowDrawArea;
	GPIOToolBarLayout.setSpacing(0);
	GPIOToolBarLayout.setMargin(0);
	QMap<int, QString> GPIOSignalMap;
	for (int i = 1; i < (int)this->ParentMainWindow->MainWindowDrawArea.ButtonLabelMap.size() + 1; i++){
		GPIOButton* GPIOSelectButton = new GPIOButton(convertToQString(this->ParentMainWindow->MainWindowDrawArea.ButtonLabelMap.at(i)), i,  this, this->ParentMainWindow);
		GPIOSelectButton->setFixedSize(233, 36);
		GPIOToolBarLayout.addWidget(GPIOSelectButton);
		QObject::connect(GPIOSelectButton, SIGNAL (GPIOButtonPressed(int)), &this->ParentMainWindow->MainWindowDrawArea, SLOT(OnGPIODeviceSignal(int)));
	}
	GPIOToolBarLayout.setAlignment((Qt::AlignTop | Qt::AlignHCenter));
	this->setFixedSize(232, 720);
 };
/* 
  ____ ____ ___ ___  ____  _   _ _____ _____ ___  _   _
 / ___|  _ |_ _/ _ \| __ )| | | |_   _|_   _/ _ \| \ | |
| |  _| |_) | | | | |  _ \| | | | | |   | || | | |  \| |
| |_| |  __/| | |_| | |_) | |_| | | |   | || |_| | |\  |
 \____|_|  |___\___/|____/ \___/  |_|   |_| \___/|_| \_|
 */

GPIOButton::GPIOButton(QString label, int GPIOID, GPIOToolBar* mainWindowGPIOToolBar, MainWindow* parentMainWindow) : QPushButton(label, mainWindowGPIOToolBar){
	this->ParentGPIOToolBar = mainWindowGPIOToolBar;
	this->ParentMainWindow = parentMainWindow;
	this->GPIOID = GPIOID;
	QObject::connect(this, SIGNAL (clicked()), this, SLOT(SelfPressed()));
}

void GPIOButton::SelfPressed(){
	emit GPIOButtonPressed(this->GPIOID);
}
/* 
 ____  ____   ___   ____ ____     _    __  __ ____ _____  _    ____ _____
|  _ \|  _ \ / _ \ / ___|  _ \   / \  |  \/  / ___|_   _|/ \  |  _ |_   _|
| |_) | |_) | | | | |  _| |_) | / _ \ | |\/| \___ \ | | / _ \ | |_) || |
|  __/|  _ <| |_| | |_| |  _ < / ___ \| |  | |___) || |/ ___ \|  _ < | |
|_|   |_| \_\\___/ \____|_| \_/_/   \_|_|  |_|____/ |_/_/   \_|_| \_\|_|
 */

void ProgramStart::TriggerBuild(){
	this->ParentMainWindow->log("Building Project!");
	std::ofstream outfile ("script.py");
	for (GPIODevice* GPD : this->ParentDrawArea->GPIOCodeVector)
	{
		outfile << GPD->build();
	} 
	outfile.close();
	this->ParentMainWindow->log("Finished Building!");
}

ProgramStart::ProgramStart(DrawArea* parent, MainWindow* parentMainWindow) :
	GPIODevice(parent, parentMainWindow, 0, 0, ""),
	DisplayText(this),
	SelfLayout(this){
		DisplayText.setText("Program Start!");
		DisplayText.setFixedWidth(200);
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		QObject::connect(&(this->ParentMainWindow->MainWindowBuildButton), SIGNAL (clicked()), this, SLOT ( TriggerBuild() ));
}


std::string ProgramStart::build(){
	return  "# script.py generated by " + convertToStdString("GPIO Studio v" + getVersionInfo()) + "\n"
			"import gpiozero\n"
			"import time\n";
}
/*
 __  __    _    ___ _   _
|  \/  |  / \  |_ _| \ | |
| |\/| | / _ \  | ||  \| |
| |  | |/ ___ \ | || |\  |
|_|  |_/_/   \_|___|_| \_| */


int main(int argc, char** argv){
	QApplication app (argc, argv);
	MainWindow w (&app);
	w.show();
	return app.exec();
}