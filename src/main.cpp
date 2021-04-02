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
#include <QPainter>
#include <QStyle>
#include <QStyleOption>
#include <QScrollArea>
#include <QPushButton>
#include <QLabel>

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
#define REVISION 1

int BUZZERCount = 1;
int LEDCount = 1;

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
	MainWindowQuitButton(this)
{
	this->ParentApp = parentApplication;
	// Base layout for MainWindow
	QGridLayout* MainWindowLayout = new QGridLayout(this);
	MainWindowLayout->setSpacing(0);
	MainWindowLayout->setMargin(0);
	
	// MainWindowDrawArea, MainWindowScrollArea
	MainWindowDrawArea.setFixedSize(2560, 1440);
	MainWindowScrollArea.setWidget(&MainWindowDrawArea);
	MainWindowScrollArea.setFixedSize(1024, 612);
	MainWindowLayout->addWidget(&MainWindowScrollArea, 1, 2, 8, 8);
	
	// MainWindowConsole
	MainWindowConsole.setReadOnly(true);
	MainWindowConsole.setFixedSize(1280, 72);
	MainWindowConsole.setStyleSheet("background-color : #dddddd;");
	this->log("Welcome to " + convertToStdString(getVersionInfo()));
	MainWindowLayout->addWidget(&MainWindowConsole, 9, 0, 1, 10);

	// MainWindowGPIOScrollArea;
	MainWindowGPIOScrollArea.setFixedSize(256, 612);
	MainWindowGPIOScrollArea.setWidget(&MainWindowGPIOToolBar);
	MainWindowLayout->addWidget(&MainWindowGPIOScrollArea, 1, 0, 2, 8);

	// Finishing up
	this->setWindowTitle(getVersionInfo());
	this->setFixedSize(1280, 720);
	MainWindowClearButton.setFixedHeight(36);
	MainWindowClearButton.setText("Clear All!");
	MainWindowClearButton.setIcon(QIcon("static/clear.svg"));
	MainWindowClearButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	MainWindowLayout->addWidget(&MainWindowClearButton, 0, 0, 1, 2);

	MainWindowBuildButton.setFixedHeight(36);
	MainWindowBuildButton.setText(" Build!");
	MainWindowBuildButton.setIcon(QIcon("static/hammer.svg"));
	MainWindowBuildButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
	MainWindowLayout->addWidget(&MainWindowBuildButton, 0, 2, 1, 2);

	MainWindowBRComboButton.setFixedHeight(36);
	MainWindowBRComboButton.setText("Build and Run!");
	MainWindowBRComboButton.setIcon(QIcon("static/run-build.svg"));
	MainWindowBRComboButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
	MainWindowLayout->addWidget(&MainWindowBRComboButton, 0, 4, 1 ,2);

	MainWindowQuitButton.setFixedHeight(36);
	MainWindowQuitButton.setText("Quit Application!");
	MainWindowQuitButton.setIcon(QIcon("static/logout.svg"));
	MainWindowQuitButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
	MainWindowLayout->addWidget(&MainWindowQuitButton, 0, 6, 1 ,2);

	QObject::connect(&MainWindowClearButton, SIGNAL (clicked()), this, SLOT (resetDrawArea()));
	QObject::connect(&MainWindowBRComboButton, SIGNAL (clicked()), this, SLOT (buildAndRun()));
	QObject::connect(&MainWindowQuitButton, SIGNAL (clicked()), this, SLOT (QuitApp()));
}

MainWindow::~MainWindow(){
	std::ofstream logFile ("debug.log");
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
}

void DrawArea::mousePressEvent(QMouseEvent *event){
	if (this->NWMode){
		if (this->isNew){
			QRect* StartBox = new QRect(0, 0, 200, 100);
			ProgramStart* PStart = new ProgramStart(this, this->ParentMainWindow);
			this->ProgStart = PStart;
			PStart->setGeometry(*StartBox);
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
		QRect GPIOBoundBox = QRect(QPoint(event->x(), event->y()), QPoint(event->x() + 200, event->y() + 100));
		switch(this->activeGPIO){
			case 1:{
				LED* GPIOD = new LED(this, ParentMainWindow, event->x(), event->y(), ("LED " + std::to_string(LEDCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
				GPIOD->show();
				GPIOCodeVector.push_back(GPIOD);
				break;
			}
			case 2:{
				Buzzer* GPIOD = new Buzzer(this, ParentMainWindow, event->x(), event->y(), ("Buzzer " + std::to_string(BUZZERCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
				GPIOD->show();
				GPIOCodeVector.push_back(GPIOD);
				break;
			}	
		}
		this->setStyleSheet("background-color : #ffffff;");
		this->NWMode = false;
	}
}

void DrawArea::paintEvent(QPaintEvent* event)
{
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
}

void DrawArea::OnGPIODeviceSignal(int GPIOID){
	this->activeGPIO = GPIOID;
	this->NWMode = true;
}

void DrawArea::resetSelf(){
	this->Lines.clear();
	this->setStyleSheet("background-color : #ffffff;");
	this->isNew = true;
	this->ParentMainWindow->log("--------------------");	
	this->ParentMainWindow->log("Resetting Project!");	
	GPIOCodeVector.clear();
	LEDCount = 1;
	BUZZERCount = 1;
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
	PinEdit(this),
	VarnameEdit(this){
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		QLabel* DisplayLabel = new QLabel(convertToQString(this->GPIOName));
		DisplayLabel->setFixedSize(180, 20);
		QLabel* PinLabel = new QLabel("Pin : ");
		PinLabel->setStyleSheet("border : 0px;");
		this->PinEdit.setStyleSheet("background-color : #cceecc;");
		QLabel* NameLabel = new QLabel("Name : ");
		NameLabel->setStyleSheet("border : 0px;");
		this->VarnameEdit.setStyleSheet("background-color : #cceecc;");
		this->SelfLayout.addWidget(DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinEdit, 1, 2);
		this->SelfLayout.addWidget(NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		LEDCount++;
}

void LED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string LED::build(){
	if (!(this->PinEdit.text() == "" || this->VarnameEdit.text() == "")){
		return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.LED(" + convertToStdString(this->PinEdit.text()) +
		")\n";
	} else {
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No suitable variable name or pin number provided\n";
	}
}

Buzzer::Buzzer(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) : 
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinEdit(this),
	VarnameEdit(this)
	{
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		QLabel* DisplayLabel = new QLabel(convertToQString(this->GPIOName));
		DisplayLabel->setFixedSize(180, 20);
		QLabel* PinLabel = new QLabel("Pin : ");
		PinLabel->setStyleSheet("border : 0px;");
		this->PinEdit.setStyleSheet("background-color : #cceecc;");
		QLabel* NameLabel = new QLabel("Name : ");
		NameLabel->setStyleSheet("border : 0px;");
		this->VarnameEdit.setStyleSheet("background-color : #cceecc;");
		this->SelfLayout.addWidget(DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinEdit, 1, 2);
		this->SelfLayout.addWidget(NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		BUZZERCount++;
}

void Buzzer::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string Buzzer::build(){
	if (!(this->PinEdit.text() == "" || this->VarnameEdit.text() == "")){
		return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.Buzzer(" + convertToStdString(this->PinEdit.text()) +
		")\n";
	} else {
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No suitable variable name or pin number provided\n";
	}
}

/*  
  ____ ____ ___ ___ _____ ___   ___  _     ____    _    ____
 / ___|  _ |_ _/ _ |_   _/ _ \ / _ \| |   | __ )  / \  |  _ \
| |  _| |_) | | | | || || | | | | | | |   |  _ \ / _ \ | |_) |
| |_| |  __/| | |_| || || |_| | |_| | |___| |_) / ___ \|  _ <
 \____|_|  |___\___/ |_| \___/ \___/|_____|____/_/   \_|_| \_\ */

 GPIOToolBar::GPIOToolBar(QWidget* parent, MainWindow* parentMainWindow) : QWidget(parent){
	this->ParentMainWindow = parentMainWindow;
	this->MainWindowDrawArea = &this->ParentMainWindow->MainWindowDrawArea;
	QVBoxLayout* GPIOToolBarLayout = new QVBoxLayout(this);
	GPIOToolBarLayout->setSpacing(0);
	GPIOToolBarLayout->setMargin(0);
	QMap<int, QString> GPIOSignalMap;
	for (int i = 1; i < 3; i++){
		GPIOButton* LEDButton = new GPIOButton(convertToQString(this->ParentMainWindow->MainWindowDrawArea.ButtonLabelMap.at(i)), i,  this, this->ParentMainWindow);
		LEDButton->setFixedSize(232, 36);
		GPIOToolBarLayout->addWidget(LEDButton);
		QObject::connect(LEDButton, SIGNAL (GPIOButtonPressed(int)), &this->ParentMainWindow->MainWindowDrawArea, SLOT(OnGPIODeviceSignal(int)));
	}
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
	this->ParentMainWindow->log("--------------------");
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
	return  "import gpiozero\n"
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