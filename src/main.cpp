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

#include <string>
#include <iostream>
#include <fstream>

//end system imports

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define REVISION 1

int GPIODCount = 0;

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

MainWindow::MainWindow() : 
	MainWindowDrawArea (this),
	MainWindowConsole (this),
	MainWindowScrollArea(this),
	MainWindowGPIOScrollArea(this),
	MainWindowGPIOToolBar(&MainWindowGPIOScrollArea, this),
	MainWindowClearButton(this),
	MainWindowBuildButton(this),
	MainWindowBRComboButton(this)
{;
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
	MainWindowConsole.setFontPointSize(12);
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

	QObject::connect(&MainWindowClearButton, SIGNAL (clicked()), this, SLOT (resetDrawArea()));
	QObject::connect(&MainWindowBRComboButton, SIGNAL (clicked()), this, SLOT (buildAndRun()));
}

MainWindow::~MainWindow(){
	std::ofstream logFile ("debug.log");
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
}

void DrawArea::mousePressEvent(QMouseEvent *event){
	if (this->NWMode){
		if (this->isNew){
			QRect* StartBox = new QRect(0, 0, 200, 100);
			ProgramStart* PStart = new ProgramStart(this, this->ParentMainWindow);
			this->ProgStart = PStart;
			PStart->setGeometry(*StartBox);
			PStart->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(PStart->Color) + "; border-radius : 5px;");
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
		GPIODevice* GPIOD = new GPIODevice(this, ParentMainWindow, event->x(), event->y(), ("GPIO Device " + std::to_string(GPIODCount)), true);
		GPIOD->setGeometry(GPIOBoundBox);
		GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
		this->setStyleSheet("background-color : #ffffff;");
		GPIOD->show();
		GPIOCodeVector.push_back(GPIOD);
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
	GPIODCount = 0;
	GPIOCodeVector.clear();
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

GPIODevice::GPIODevice(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name, bool log) : QWidget(parent), DisplayText(this){
	this->ParentMainWindow = parentMainWindow;
	this->XCoord = X;
	this->YCoord = Y;
	DisplayText.setText(convertToQString(name));
	DisplayText.setFixedWidth(200);
	this->GPIOName = name;
	if (log){
		this->ParentMainWindow->log("Created new GPIO Device at " + std::to_string(this->XCoord) + ", " + std::to_string(this->YCoord));
	}
	QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
	GPIODCount++;
}

void GPIODevice::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string GPIODevice::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return "# " + this->GPIOName + "\n";
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
	for (int i = 0; i < 20; i++){
		GPIOButton* LEDButton = new GPIOButton(convertToQString("New LED " + std::to_string(i)), i,  this, this->ParentMainWindow);
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
	outfile << "\n_ = input(\"Press ENTER or RETURN to continue...\")"; 
	outfile.close();
	this->ParentMainWindow->log("Finished Building!");
}

ProgramStart::ProgramStart(DrawArea* parent, MainWindow* parentMainWindow) :
	GPIODevice(parent, parentMainWindow, 0, 0, "", false),
	DisplayText(this){
		DisplayText.setText("Program Start!");
		DisplayText.setFixedWidth(200);
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		QObject::connect(&(this->ParentMainWindow->MainWindowBuildButton), SIGNAL (clicked()), this, SLOT ( TriggerBuild() ));
}


std::string ProgramStart::build(){
	return  "# import gpiozero\n"
			"import time\n"
			;
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