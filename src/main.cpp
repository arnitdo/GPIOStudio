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
#include <QFileDialog>
#include <QDesktopServices>
#include <QUrl>

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
#define REVISION 5

namespace Counters{
	int BUZZERCount = 1;
	int LEDCount = 1;
	int LEDCTRLCount = 1;
	int BUZZERCTRLCount = 1;
	int SLEEPCount = 1;
	int BUTTONCount = 1;
	int FUNCTIONCount = 1;
	int FUNCTRLCount = 1;
	int BTNCTRLCount = 1;
	void reset(){
		BUZZERCount = 1;
		LEDCount = 1;
		LEDCTRLCount = 1;
		BUZZERCTRLCount = 1;
		SLEEPCount = 1;
		BUTTONCount = 1;
		FUNCTIONCount = 1;
		FUNCTRLCount = 1;
		BTNCTRLCount = 1;
	}
}
/*
  ____ _____ _   _ _____ ____  ___ ____
 / ___| ____| \ | | ____|  _ \|_ _/ ___|
| |  _|  _| |  \| |  _| | |_) || | |
| |_| | |___| |\  | |___|  _ < | | |___
 \____|_____|_| \_|_____|_| \_|___\____| */


QString getVersionInfo(){
	// Returns "MAJOR_VERSION.MINOR_VERSION.REVISION"
	std::string VInfo = std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(REVISION);
	return QString(VInfo.c_str());
}

QString convertToQString(std::string in){
	// Converts std::string to QString
	return QString(in.c_str());
}

std::string convertToStdString(QString in){
	// Vice Versa, converts QString to std::string
	return in.toUtf8().constData();
}

/*
 __  __    _    ___ _   ___        _____ _   _ ____   _____        __
|  \/  |  / \  |_ _| \ | \ \      / |_ _| \ | |  _ \ / _ \ \      / /
| |\/| | / _ \  | ||  \|  \ \ /\ / / | ||  \| | | | | | | \ \ /\ / /
| |  | |/ ___ \ | || |\    \ V  V /  | || |\  | |_| | |_| |\ V  V /
|_|  |_/_/   \_|___|_| \_   \_/\_/  |___|_| \_|____/ \___/  \_/\_/ */

MainWindow::MainWindow(QApplication* parentApplication) : 
	RemoteWindow(nullptr, Qt::WindowMaximizeButtonHint),
	RaspiIPEdit(&RemoteWindow),
	RemoteRunButton("Run Script", &RemoteWindow),
	RWHideButton("Hide Window", &RemoteWindow),
	HelpWindow(nullptr),
	AboutWindow(nullptr),
	MainWindowDrawArea (this),
	MainWindowConsole (this),
	MainWindowScrollArea(this),
	MainWindowGPIOScrollArea(this),
	MainWindowGPIOToolBar(&MainWindowGPIOScrollArea, this),
	MainWindowClearButton(this),
	MainWindowRefreshButton(this),
	MainWindowBuildButton(this),
	MainWindowRemoteButton(this),
	MainWindowLoadButton(this),
	MainWindowSaveButton(this),
	MainWindowHelpButton(this),
	MainWindowAboutButton(this),
	MainWindowQuitButton(this),
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

	// Remote Window Setup
	this->RemoteWindow.setFixedSize(320, 160);
	QGridLayout* RWLayout = new QGridLayout(&RemoteWindow);
	this->RemoteWindow.setLayout(RWLayout);
	this->RaspiIPEdit.setFixedWidth(150);
	RWLayout->addWidget(&this->RaspiIPEdit, 1, 2);
	RWLayout->addWidget(new QLabel("Raspberry Pi IP Address : ", &this->RemoteWindow), 1, 1);
	RWLayout->addWidget(&this->RWHideButton, 2, 1);
	RWLayout->addWidget(&this->RemoteRunButton, 2 ,2);
	this->RemoteWindow.hide();
	// Finishing up
	// Adding to Layouts
	this->setWindowTitle("GPIO Studio v" + getVersionInfo());
	this->setFixedSize(1280, 720);
	MainWindowClearButton.setFixedHeight(36);
	MainWindowClearButton.setText(" Clear All");
	MainWindowClearButton.setIcon(QIcon("static/clear.svg"));
	MainWindowClearButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	MainWindowLayout.addWidget(&MainWindowClearButton, 0, 0);

	MainWindowRefreshButton.setFixedHeight(36);
	MainWindowRefreshButton.setText(" Refresh");
	MainWindowRefreshButton.setIcon(QIcon("static/refresh.svg"));
	MainWindowRefreshButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
	MainWindowLayout.addWidget(&MainWindowRefreshButton, 0, 1);

	MainWindowBuildButton.setFixedHeight(36);
	MainWindowBuildButton.setText(" Build");
	MainWindowBuildButton.setIcon(QIcon("static/hammer.svg"));
	MainWindowBuildButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
	MainWindowLayout.addWidget(&MainWindowBuildButton, 0, 2);

	MainWindowRemoteButton.setFixedHeight(36);
	MainWindowRemoteButton.setText(" Run Remote");
	MainWindowRemoteButton.setIcon(QIcon("static/remote.svg"));
	MainWindowRemoteButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
	MainWindowLayout.addWidget(&MainWindowRemoteButton, 0, 3);

	MainWindowLoadButton.setFixedHeight(36);
	MainWindowLoadButton.setText(" Load File");
	MainWindowLoadButton.setIcon(QIcon("static/package.svg"));
	MainWindowLoadButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
	MainWindowLayout.addWidget(&MainWindowLoadButton, 0, 4);

	MainWindowSaveButton.setFixedHeight(36);
	MainWindowSaveButton.setText(" Save File");
	MainWindowSaveButton.setIcon(QIcon("static/save.svg"));
	MainWindowSaveButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
	MainWindowLayout.addWidget(&MainWindowSaveButton, 0, 5);

	MainWindowHelpButton.setFixedHeight(36);
	MainWindowHelpButton.setText(" Help");
	MainWindowHelpButton.setIcon(QIcon("static/help.svg"));
	MainWindowHelpButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
	MainWindowLayout.addWidget(&MainWindowHelpButton, 0, 6);

	MainWindowAboutButton.setFixedHeight(36);
	MainWindowAboutButton.setText(" About");
	MainWindowAboutButton.setIcon(QIcon("static/help-about.svg"));
	MainWindowAboutButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
	MainWindowLayout.addWidget(&MainWindowAboutButton, 0, 7);

	MainWindowGithubButton.setFixedHeight(36);
	MainWindowGithubButton.setText(" GitHub");
	MainWindowGithubButton.setIcon(QIcon("static/github.svg"));
	MainWindowGithubButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_G));
	MainWindowLayout.addWidget(&MainWindowGithubButton, 0, 8);

	MainWindowQuitButton.setFixedHeight(36);
	MainWindowQuitButton.setText(" Quit Application!");
	MainWindowQuitButton.setIcon(QIcon("static/logout.svg"));
	MainWindowQuitButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
	MainWindowLayout.addWidget(&MainWindowQuitButton, 0, 9);

	QObject::connect(&MainWindowClearButton, SIGNAL (clicked()), this, SLOT (resetDrawArea()));
	QObject::connect(&MainWindowRefreshButton, SIGNAL (clicked()), this, SLOT (RefreshDrawSelects()));
	QObject::connect(&MainWindowRemoteButton, SIGNAL (clicked()), this, SLOT (showRemoteWindow()));
	QObject::connect(&RWHideButton, SIGNAL (clicked()), this, SLOT (hideRemoteWindow()));
	QObject::connect(&RemoteRunButton, SIGNAL (clicked()), this, SLOT (runRemote()));
	QObject::connect(&MainWindowLoadButton, SIGNAL (clicked()), this, SLOT (OpenJSON()));
	QObject::connect(&MainWindowSaveButton, SIGNAL (clicked()), this, SLOT (SaveToJSON()));
	QObject::connect(&MainWindowHelpButton, SIGNAL (clicked()), this, SLOT (ShowHelpWindow()));
	QObject::connect(&MainWindowAboutButton, SIGNAL (clicked()), this, SLOT (ShowAboutWindow()));
	QObject::connect(&MainWindowGithubButton, SIGNAL (clicked()), this, SLOT (OpenGithub()));
	QObject::connect(&MainWindowQuitButton, SIGNAL (clicked()), this, SLOT (QuitApp()));
}

MainWindow::~MainWindow(){
	// Dump Console Content to log file
	std::ofstream logFile ("GPIOStudio.log");
	this->log("Qutting Application!");
	logFile << convertToStdString(this->MainWindowConsole.toPlainText());
	logFile.close();
}

void MainWindow::log(std::string value){
	// LOGS Value e.g LOG : <content>
	this->MainWindowConsole.setTextColor(QColor("black"));
	this->MainWindowConsole.append("LOG : " + convertToQString(value));
}

void MainWindow::warn(std::string value){
	// WARNS user e.g WARN : <content>
	this->MainWindowConsole.setTextColor(QColor("#E34234"));
	this->MainWindowConsole.append("WARNING : " + convertToQString(value));
}

void MainWindow::debug(std::string value){
	// Debugging usage, use when developing e.g DEBUG : <content>
	this->MainWindowConsole.setTextColor(QColor("green"));
	this->MainWindowConsole.append("DEBUG : " + convertToQString(value));
}

void MainWindow::err(std::string value){
	// Use for invalid inputs or fatal errors e.g ERROR : <content>
	this->MainWindowConsole.setTextColor(QColor("red"));
	this->MainWindowConsole.append("ERROR : " + convertToQString(value));
}

void MainWindow::RefreshDrawSelects(){
	// Refresh Combo Boxes, lock combo box content length,
	// So that forward function declarations are not included in them
	// Python being interpreted does not play well with undefined functions
	this->MainWindowDrawArea.RefreshSelects();
}

void MainWindow::resetDrawArea(){
	// Clean Slate Draw Area
	this->MainWindowDrawArea.resetSelf();
}

void MainWindow::showRemoteWindow(){
	this->RemoteWindow.show();
}

void MainWindow::hideRemoteWindow(){
	this->RemoteWindow.hide();
}

void MainWindow::runRemote(){
	if (this->RaspiIPEdit.text() != ""){
		this->RemoteIP = convertToStdString(this->RaspiIPEdit.text());
		this->MainWindowBuildButton.click();
		#ifdef _WIN32
		if (!system("python.exe script.py")){
			this->log("Remote Success!");
		} else {
			this->err("An error occured when running remote script!");
		}
		#endif
		#ifdef __linux__
		if (!system("python3 script.py")){
			this->log("Remote Success!");
		} else {
			this->err("An error occured when running remote script!");
		}
		#endif
	} else {
		this->err("No valid IP Address Provided for Remote Raspberry Pi!");
	}
	this->hideRemoteWindow();
}

void MainWindow::OpenJSON(){
	this->MainWindowDrawArea.loadJson();
}

void MainWindow::SaveToJSON(){
	this->MainWindowDrawArea.saveToJson();
}

void MainWindow::ShowHelpWindow(){
	// this->HelpWindow.show();
	void(0);
}

void MainWindow::ShowAboutWindow(){
	void(0);
}

void MainWindow::OpenGithub(){
	QDesktopServices::openUrl(QUrl("https://www.github.com/arnitdo/GPIOStudio/"));
}

void MainWindow::QuitApp(){
	// Calls QApplication::quit()
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
	// Insert Values for GPIOToolBar
	this->setStyleSheet("background-color : white;");
	ButtonLabelMap.insert({1, "Program Start"});
	ButtonLabelMap.insert({2, "Simple LED"});
	ButtonLabelMap.insert({3, "Simple Buzzer"});
	ButtonLabelMap.insert({4, "LED Controls"});
	ButtonLabelMap.insert({5, "Buzzer Controls"});
	ButtonLabelMap.insert({6, "Sleep Timer"});
	ButtonLabelMap.insert({7, "Simple Button"});
	ButtonLabelMap.insert({8, "Custom Function"});
	ButtonLabelMap.insert({9, "Function Controls"});
	ButtonLabelMap.insert({10, "Button Controls"});
}

void DrawArea::loadJson(){
	QString fname = QFileDialog::getOpenFileName(this, "Open GPIO JSON File", "", "JSON (*.json)");
	if (!fname.isEmpty()){
		this->ParentMainWindow->log("Opening file " + convertToStdString(fname));
		this->ParentMainWindow->MainWindowClearButton.click();
		// Emulate button click, clears the entire board
		std::ifstream JSONFileIn (convertToStdString(fname));
		json JSON;
		JSONFileIn >> JSON;
		if (JSON["version"].get<std::string>() == convertToStdString(getVersionInfo())){
			// Version Matches, Proceed!
			for (auto GPIOJSON : JSON["json"]){
				int id, x, y;
				id = GPIOJSON["id"].get<int>();
				x = GPIOJSON["x"].get<int>();
				y = GPIOJSON["y"].get<int>();
				this->createGPIODevice(id, x, y);
			}
		} else {
			this->ParentMainWindow->err("Version Mismatch!");
			this->ParentMainWindow->err("The version of the file you have provided does not match the current version of GPIO Studio!");
		}
	}
};

void DrawArea::saveToJson(){
	QFileDialog OpenFileDialog;
	QString fname = OpenFileDialog.getSaveFileName(this, "Save GPIO Project", "", "JSON Files (*.json)");
	if (!fname.isEmpty()){
		this->ParentMainWindow->log("Saving to File " + convertToStdString(fname));
		json JsonWrite;
		JsonWrite["version"] = convertToStdString(getVersionInfo());
		json GPIOArray = json::array();
		for (GPIODevice* GPIOD : this->GPIOCodeVector){
			json GPIOJSON;
			GPIOJSON["id"] = GPIOD->id;
			GPIOJSON["x"] = GPIOD->XCoord;
			GPIOJSON["y"] = GPIOD->YCoord;
			GPIOArray.push_back(GPIOJSON);
		}
		JsonWrite["json"] = GPIOArray;
		std::ofstream WriteFile;
		WriteFile.open(convertToStdString(fname), std::ios::trunc);
		WriteFile << JsonWrite.dump(4);
		WriteFile.close();
	}
}

void DrawArea::mousePressEvent(QMouseEvent *event){
	// Set of actions to do on mouse press
	if (this->NWMode){
		// Check if New Widget Mode is active
		// I.e if NWMode == false
		// No click action
		// Prevents unnecessary clicks and GPIO Constructions
		this->createGPIODevice(this->activeGPIO, event->x(), event->y());
		this->setStyleSheet("background-color : #ffffff;");
		this->NWMode = false;
	}
}

void DrawArea::createGPIODevice(int active, int X, int Y){
	// Constructs a GPIO Device at the given coordinates
	// This function has been specifically created to allow loading of GPIO Devices from a JSON File
	this->activeGPIO = active;
	switch(this->activeGPIO){
		case 1:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			ProgramStart* GPIOD = new ProgramStart(this, ParentMainWindow, X, Y, ("Program Start"));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->LastPoint = QPoint(X + 200, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->LastPoint));
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
		case 2:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			LED* GPIOD = new LED(this, ParentMainWindow, X, Y, ("LED " + std::to_string(Counters::LEDCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->GPIOCodeVector.push_back(GPIOD);
			this->LEDVec.push_back(GPIOD);
			break;
		}
		case 3:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Buzzer* GPIOD = new Buzzer(this, ParentMainWindow, X, Y, ("Buzzer " + std::to_string(Counters::BUZZERCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->GPIOCodeVector.push_back(GPIOD);
			this->BUZVec.push_back(GPIOD);
			break;
		}
		case 4:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			LEDCtrl* GPIOD = new LEDCtrl(this, ParentMainWindow, X, Y, ("LED Controls " + std::to_string(Counters::LEDCTRLCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->GPIOCodeVector.push_back(GPIOD);
			this->LEDCTRLVec.push_back(GPIOD);
			this->RefreshSelects();
			break;
		}
		case 5:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			BuzzerCtrl* GPIOD = new BuzzerCtrl(this, ParentMainWindow, X, Y, ("Buzzer Controls " + std::to_string(Counters::BUZZERCTRLCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->GPIOCodeVector.push_back(GPIOD);
			this->BUZCTRLVec.push_back(GPIOD);
			this->RefreshSelects();
			break;
		}
		case 6:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Sleep* GPIOD = new Sleep(this, ParentMainWindow, X, Y, ("Sleep Timer " + std::to_string(Counters::SLEEPCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
		case 7:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Button* GPIOD = new Button(this, ParentMainWindow, X, Y, ("Button " + std::to_string(Counters::BUTTONCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->BTNVec.push_back(GPIOD);
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
		case 8:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			Function* GPIOD = new Function(this, ParentMainWindow, X, Y, ("Function " + std::to_string(Counters::FUNCTIONCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->FUNCVec.push_back(GPIOD);
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
		case 9:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			FunctionControl* GPIOD = new FunctionControl(this, ParentMainWindow, X, Y, ("Function Controls " + std::to_string(Counters::FUNCTRLCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->GPIOCodeVector.push_back(GPIOD);
			this->FUNCTRLVec.push_back(GPIOD);
			this->RefreshSelects();
			break;
		}
		case 10:{
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
			ButtonControl* GPIOD = new ButtonControl(this, ParentMainWindow, X, Y, ("Button Controls " + std::to_string(Counters::BTNCTRLCount)));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + ";");
			GPIOD->show();
			this->CurrentPoint = QPoint(X, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
			this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
			this->GPIOCodeVector.push_back(GPIOD);
			this->BTNCTRLVec.push_back(GPIOD);
			this->RefreshSelects();
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
	for (std::pair<QPoint, QPoint> PointPair : this->Lines){
		p->setPen(QPen(Qt::black, 2));
		p->drawLine(std::get<0>(PointPair), std::get<1>(PointPair));
	};
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
	this->LastPoint = QPoint(0, 0);
	this->setStyleSheet("background-color : #ffffff;");
	this->isNew = true;	
	this->ParentMainWindow->log("Resetting Project!");	
	this->GPIOCodeVector.clear();
	this->BUZVec.clear();
	this->LEDVec.clear();
	this->BTNVec.clear();
	this->LEDCTRLVec.clear();
	this->BUZCTRLVec.clear();
	this->FUNCVec.clear();
	this->BTNCTRLVec.clear();
	Counters::reset();
}

void DrawArea::RefreshSelects(){
	QStringList LEDNames, BuzzerNames, FuncNames, FunctionNames, ButtonNames;
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

	for (FunctionControl* FCTRL : this->FUNCTRLVec){
		FCTRL->FunctionSelect.clear();
		FCTRL->FunctionSelect.insertItems(0, FuncNames);
		FCTRL->FunctionSelect.setMaxCount(FCTRL->FunctionSelect.count());
	}

	// ButtonControl Refresh
	for (Button* BTN : this->BTNVec){
		ButtonNames << BTN->VarnameEdit.text();
	}

	for (Function* F : this->FUNCVec){
		FunctionNames << F->NameEdit.text();
	}

	 for (ButtonControl* BCTRL : this->BTNCTRLVec){
		BCTRL->FunctionSelect.clear();
		BCTRL->ButtonSelect.clear();
		BCTRL->ButtonSelect.insertItems(0, ButtonNames);
		BCTRL->ButtonSelect.setMaxCount(BCTRL->ButtonSelect.count());
		BCTRL->FunctionSelect.insertItems(0, FunctionNames);
		BCTRL->FunctionSelect.setMaxCount(BCTRL->FunctionSelect.count());
	 }
	FunctionNames.clear();
	FuncNames.clear();
	ButtonNames.clear();
	LEDNames.clear();
	BuzzerNames.clear();
}

/* 
  ____ ____ ___ ___
 / ___|  _ |_ _/ _ \
| |  _| |_) | | | | |
| |_| |  __/| | |_| |
 \____|_|  |___\___/ */


/* COMMON NOTE
	Most GPIO Devices will have 3 functions -
		- Constructor
		- build()
		- deleteSelf()

	* Constructor -
		: Sets layout and properties

	* build() - 
		: Returns equivalent python code
	* deleteSelf() -
		: logs into console deletion
		: Literally deletes itself

 */

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
		this->id = 2;
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
		", pin_factory=remote_pin_factory)\n";
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
		this->id = 3;
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
		", pin_factory=remote_pin_factory)\n";
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
		this->id = 4;
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
		this->id = 5;
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
		this->id = 6;
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
		this->id = 7;
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
		", pin_factory=remote_pin_factory)\n";
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
		this->id = 8;
		QWidget* BodyWindow = new QWidget;
		QTextEdit* FunctionBodyEdit = new QTextEdit(BodyWindow);
		QPushButton* CBody = new QPushButton("Close Window", BodyWindow);
		BodyWindow->setWindowFlags(Qt::WindowMaximizeButtonHint);
		BodyWindow->setFixedSize(640, 480);
		FunctionBodyEdit->setGeometry(50, 20, 540, 380);
		CBody->setGeometry(270, 425, 100, 25);
		FunctionBodyEdit->setFontPointSize(14);
		FunctionBodyEdit->append("# Write your function code here\n# You do not need to indent the code body\nprint(\"Hello, World!\")\n");
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
	this->FunctionBodyWindow->setWindowTitle("Edit Function " + this->NameEdit.text());
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
	this->FunctionText.clear();
	this->FunctionText << convertToStdString(this->FunctionBody->toPlainText());
	out = "def " + convertToStdString(this->NameEdit.text()) + "():\n";
	while (std::getline(this->FunctionText, templine)){
		out += "\t" + templine + "\n";
	}
	return out;
}

FunctionControl::FunctionControl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	FunctionSelect(this),
	DisplayLabel(convertToQString(name), this),
	ExecuteLabel("Select Function to execute : ", this)
	{
		this->id = 9;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		ExecuteLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&ExecuteLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&FunctionSelect, 2, 1, 1, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::FUNCTRLCount++;
	}

void FunctionControl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string FunctionControl::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (this->FunctionSelect.currentText() == ""){
		this->ParentMainWindow->err("No Function Selected for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No Function Selected\n";
	} else {
		return convertToStdString(this->FunctionSelect.currentText()) + "()\n";
	}
}

ButtonControl::ButtonControl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	ButtonSelect(this),
	StateSelect(this),
	FunctionSelect(this),
	DisplayLabel(convertToQString(name), this),
	ButtonLabel("Select Button to target : ", this),
	StateLabel("Select Button Trigger State : ", this),
	ExecuteLabel("Select Function to execute : ", this)
	{
		this->id = 10;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		StateSelect.addItem("when_held");
		StateSelect.addItem("when_pressed");
		StateSelect.addItem("when_released");
		DisplayLabel.setFixedSize(180, 20);
		ButtonLabel.setStyleSheet("border : 0px;");
		StateLabel.setStyleSheet("border : 0px;");
		ExecuteLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&ButtonLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&ButtonSelect, 2, 1, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 3, 1, 1 ,2);
		this->SelfLayout.addWidget(&StateSelect, 4, 1, 1, 2);
		this->SelfLayout.addWidget(&ExecuteLabel, 5, 1, 1, 2);
		this->SelfLayout.addWidget(&FunctionSelect, 6, 1, 1, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::BTNCTRLCount++;
	}

void ButtonControl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string ButtonControl::build(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	if (this->FunctionSelect.currentText() == ""){
		this->ParentMainWindow->err("No Function Selected for " + this->GPIOName);
		return "# GPIOStudio - " + this->GPIOName + " : No Function Selected\n";
	} else {
		if (this->ButtonSelect.currentText() == ""){
			this->ParentMainWindow->err("No Button Selected for " + this->GPIOName);
			return "# GPIOStudio - " + this->GPIOName + " : No Function Selected\n";
		} else {
			return convertToStdString(this->ButtonSelect.currentText() + "." + this->StateSelect.currentText() + " = " + this->FunctionSelect.currentText() + "\n");
		 }
	}
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
	outfile << "\n";
	this->ParentDrawArea->LoopCode.clear();
	for (std::string lc : this->ParentDrawArea->LOOPCodeVector){
		this->ParentDrawArea->LoopCode << lc;
	}
	std::string tempstr;
	outfile << "while True:\n";
	while (std::getline(this->ParentDrawArea->LoopCode, tempstr)){
		outfile << "\t" + tempstr;
	}
	outfile << "\ttime.sleep(1)\n";
	outfile.close();
	this->ParentMainWindow->log("Finished Building!");
}

ProgramStart::ProgramStart(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X,Y, name),
	DisplayText(this),
	SelfLayout(this){
		this->id = 1;
		DisplayText.setText(convertToQString(name));
		DisplayText.setFixedWidth(200);
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		QObject::connect(&(this->ParentMainWindow->MainWindowBuildButton), SIGNAL (clicked()), this, SLOT ( TriggerBuild() ));
}

void ProgramStart::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string ProgramStart::build(){
	return  "# script.py generated by " + convertToStdString("GPIO Studio v" + getVersionInfo()) + "\n"
			"import gpiozero\n"
			"import time\n"
			"remote_pin_factory = gpiozero.pins.pigpio.PiGPIOFactory(host=\"" + this->ParentMainWindow->RemoteIP + "\")\n";
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
