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
#include <QTextBrowser>

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
#define REVISION 6

#define PSTART_ID 1
#define LED_ID 2
#define PWMLED_ID 3
#define RGBLED_ID 4
#define BUZ_ID 5
#define FUNC_ID 6
#define BTN_ID 7
#define SLEEP_ID 8
#define LEDCTRL_ID 9
#define PWMLEDCTRL_ID 10
#define RGBLEDCTRL_ID 11
#define BUZCTRL_ID 12
#define FCTRL_ID 13
#define BTNCTRL_ID 14

// Function Prototypes
QString getVersionInfo();
QString convertToQString(std::string in);
std::string convertToStdString(QString in);

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
	int RGBLEDCount = 1;
	int RGBLEDCTRLCount = 1;
	int PWMLEDCount = 1;
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
		RGBLEDCount = 1;
		RGBLEDCTRLCount = 1;
		PWMLEDCount = 1;
	}
}

namespace Config{
	int defaultSleepTime = 5; // Default time in SleepTimer
	int keepaliveSleepTime = 1; // Default time in keepalive
	std::string defaultPiIP = ""; // Default pi IP in Remote
	int legacyMode = 2;
	/* 
	LEGACY MODE DOC
	Reference - https://elinux.org/RPi_Low-level_peripherals
	legacy mode is defined by an integer - 
	0 - Raspberry Pi Model A, B -  revision 1.0 (26 pin header)
		[2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27]
	1 - Raspberry Pi Model A, B - revision 2.0 (26 pin + 8 pin extra (4 GPIO Extra))
		[0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31]
	2 - Raspberry Pi Model 2, 2B+ .... (Full 40 Pins)
		[2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27]
	 */
	void LoadConfig(MainWindow* MainWin){
		std::ifstream configFile;
		configFile.open("config.json", std::ios::in);
		if (!configFile.is_open()){
			// Revert to defaults
			MainWin->err("Error opening config.json file!");
			MainWin->err("Falling back to default configuration");
			defaultSleepTime = 5;
			keepaliveSleepTime = 1;
			defaultPiIP = "";
			legacyMode = 2;
			configFile.close();
		} else {
			// Load Config from JSON File
			json JsonConfigData;
			configFile >> JsonConfigData;
			defaultPiIP = JsonConfigData["defaultPiIP"].get<std::string>();
			defaultSleepTime = JsonConfigData["defaultSleepTime"].get<int64_t>();
			keepaliveSleepTime = JsonConfigData["keepaliveSleepTime"].get<int64_t>();
			legacyMode = JsonConfigData["legacyMode"].get<int64_t>();
			MainWin->RaspiIPEdit.setText(convertToQString(defaultPiIP));
			configFile.close();
		}
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
	RemoteWindow(nullptr),
	RaspiIPEdit(&RemoteWindow),
	RemoteRunButton("Run Script", &RemoteWindow),
	RWHideButton("Hide Window", &RemoteWindow),
	HelpWindow(nullptr),
	AboutWindow(nullptr),
	AboutWindowContent(&AboutWindow),
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
	this->RaspiIPEdit.setText(convertToQString(Config::defaultPiIP));
	RWLayout->addWidget(&this->RaspiIPEdit, 1, 2);
	RWLayout->addWidget(new QLabel("Raspberry Pi IP Address : ", &this->RemoteWindow), 1, 1);
	RWLayout->addWidget(&this->RWHideButton, 2, 1);
	RWLayout->addWidget(&this->RemoteRunButton, 2 ,2);
	this->RemoteWindow.hide();

	// About Window Setup
	this->AboutWindow.setFixedSize(320, 160);
	this->AboutWindow.setWindowTitle("About GPIO Studio");
	this->AboutWindowContent.setGeometry(10, 10 ,300, 140);
	this->AboutWindowContent.setReadOnly(true);
	this->AboutWindowContent.setOpenExternalLinks(true);
	this->AboutWindowContent.append(
		"<p><h2>GPIO Studio v" + getVersionInfo() + "</h2>"
		"A simple Flow-Based GUI Tool for creating GPIO Scripts on Raspberry Pi<br>"
		"Developed in Qt 5.15.2 , C++11<br>"
		"Distributed under the <b>GNU GPLv3 License</b><br>"
		"The source code of this program can be found at<br>"
		"<a href=\"https://www.github.com/arnitdo/GPIOStudio/\">https://www.github.com/arnitdo/GPIOStudio/</a><br></p>"
	);

	// Finishing up
	// Adding to Layouts
	this->setWindowTitle("GPIO Studio v" + getVersionInfo());
	this->setFixedSize(1280, 720);

	// Delete Last Button
	MainWindowDeleteLastButton.setFixedHeight(36);
	MainWindowDeleteLastButton.setText(" Delete Last");
	MainWindowDeleteLastButton.setIcon(QIcon("static/undo.svg"));
	MainWindowDeleteLastButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
	MainWindowLayout.addWidget(&MainWindowDeleteLastButton, 0, 0);
	
	// Clear All Button
	MainWindowClearButton.setFixedHeight(36);
	MainWindowClearButton.setText(" Clear All");
	MainWindowClearButton.setIcon(QIcon("static/clear.svg"));
	MainWindowClearButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	MainWindowLayout.addWidget(&MainWindowClearButton, 0, 1);

	// Refresh Button
	MainWindowRefreshButton.setFixedHeight(36);
	MainWindowRefreshButton.setText(" Refresh");
	MainWindowRefreshButton.setIcon(QIcon("static/refresh.svg"));
	MainWindowRefreshButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
	MainWindowLayout.addWidget(&MainWindowRefreshButton, 0, 2);

	// Build Button
	MainWindowBuildButton.setFixedHeight(36);
	MainWindowBuildButton.setText(" Build");
	MainWindowBuildButton.setIcon(QIcon("static/hammer.svg"));
	MainWindowBuildButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
	MainWindowLayout.addWidget(&MainWindowBuildButton, 0, 3);

	// Remote Run Button
	MainWindowRemoteButton.setFixedHeight(36);
	MainWindowRemoteButton.setText(" Run Remote");
	MainWindowRemoteButton.setIcon(QIcon("static/remote.svg"));
	MainWindowRemoteButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
	MainWindowLayout.addWidget(&MainWindowRemoteButton, 0, 4);

	// Load Button
	MainWindowLoadButton.setFixedHeight(36);
	MainWindowLoadButton.setText(" Load File");
	MainWindowLoadButton.setIcon(QIcon("static/package.svg"));
	MainWindowLoadButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
	MainWindowLayout.addWidget(&MainWindowLoadButton, 0, 5);

	// Save Button
	MainWindowSaveButton.setFixedHeight(36);
	MainWindowSaveButton.setText(" Save File");
	MainWindowSaveButton.setIcon(QIcon("static/save.svg"));
	MainWindowSaveButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
	MainWindowLayout.addWidget(&MainWindowSaveButton, 0, 6);

	// Help Button
	MainWindowHelpButton.setFixedHeight(36);
	MainWindowHelpButton.setText(" Help");
	MainWindowHelpButton.setIcon(QIcon("static/help.svg"));
	MainWindowHelpButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
	MainWindowLayout.addWidget(&MainWindowHelpButton, 0, 7);

	// About Button
	MainWindowAboutButton.setFixedHeight(36);
	MainWindowAboutButton.setText(" About");
	MainWindowAboutButton.setIcon(QIcon("static/info.svg"));
	MainWindowAboutButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
	MainWindowLayout.addWidget(&MainWindowAboutButton, 0, 8);

	// Quit Button
	MainWindowQuitButton.setFixedHeight(36);
	MainWindowQuitButton.setText(" Quit Application!");
	MainWindowQuitButton.setIcon(QIcon("static/logout.svg"));
	MainWindowQuitButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Q));
	MainWindowLayout.addWidget(&MainWindowQuitButton, 0, 9);

	// Connecting SIGNALS and SLOTS
	QObject::connect(&MainWindowDeleteLastButton, SIGNAL (clicked()), this, SLOT (DeleteLast()));
	QObject::connect(&MainWindowClearButton, SIGNAL (clicked()), this, SLOT (resetDrawArea()));
	QObject::connect(&MainWindowRefreshButton, SIGNAL (clicked()), this, SLOT (RefreshDrawSelects()));
	QObject::connect(&MainWindowRemoteButton, SIGNAL (clicked()), this, SLOT (showRemoteWindow()));
	QObject::connect(&RWHideButton, SIGNAL (clicked()), this, SLOT (hideRemoteWindow()));
	QObject::connect(&MainWindowLoadButton, SIGNAL (clicked()), this, SLOT (OpenJSON()));
	QObject::connect(&MainWindowSaveButton, SIGNAL (clicked()), this, SLOT (SaveToJSON()));
	QObject::connect(&MainWindowHelpButton, SIGNAL (clicked()), this, SLOT (ShowHelpWindow()));
	QObject::connect(&MainWindowAboutButton, SIGNAL (clicked()), this, SLOT (ShowAboutWindow()));
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

void MainWindow::DeleteLast(){
	// Calls MainWindowDrawArea.deleteLast();
	// Deletes The last created object
	this->MainWindowDrawArea.deleteLast();
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
		#if defined(_WIN32)
			if (!system("python.exe script.py")){
				this->log("Remote Success!");
			} else {
				this->err("An error occured when running remote script!");
			}
		#elif defined(__linux__)
			if (!system("python3 script.py")){
				this->log("Remote Success!");
			} else {
				this->err("An error occured when running remote script!");
			}
		#else
			// Fallback attempts.
			this->log("Non Windows or Non Linux System Detected. Attempting to run python3");
			if (system("python3 script.py")){
				this->log("python3 failed! Now trying python.exe");
				if (system("python.exe script.py")){
					this->log("python.exe failed!");
					this->err("Unable to run any python3 binary!");
					this->log("You may need to simply build the project, and run it manually");
				}
				else {
					this->log("python.exe succeeded!");
				}
			} else {
				this->log("python3 succeeded!");
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
	void(0);
}

void MainWindow::ShowAboutWindow(){
	this->AboutWindow.show();
}

void MainWindow::QuitApp(){
	this->close();
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
	this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
	ButtonLabelMap.insert({PSTART_ID, "Program Start"});
	ButtonLabelMap.insert({LED_ID, "Simple LED"});
	ButtonLabelMap.insert({PWMLED_ID, "PWM LED"});
	ButtonLabelMap.insert({RGBLED_ID, "RGB LED"});
	ButtonLabelMap.insert({BUZ_ID, "Simple Buzzer"});
	ButtonLabelMap.insert({FUNC_ID, "Custom Function"});
	ButtonLabelMap.insert({BTN_ID, "Simple Button"});
	ButtonLabelMap.insert({SLEEP_ID, "Sleep Timer"});
	ButtonLabelMap.insert({LEDCTRL_ID, "LED Controls"});
	ButtonLabelMap.insert({PWMLEDCTRL_ID, "PWM LED Controls"});
	ButtonLabelMap.insert({RGBLEDCTRL_ID, "RGB LED Controls"});
	ButtonLabelMap.insert({BUZCTRL_ID, "Buzzer Controls"});
	ButtonLabelMap.insert({FCTRL_ID, "Function Controls"});
	ButtonLabelMap.insert({BTNCTRL_ID, "Button Controls"});
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
				id = GPIOJSON["id"].get<int64_t>();
				x = GPIOJSON["x"].get<int64_t>(); 
				y = GPIOJSON["y"].get<int64_t>(); 
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
	OpenFileDialog.setDefaultSuffix("json");
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
		WriteFile << JsonWrite.dump(4); // Indentation!
		WriteFile.close();
	}
}

void DrawArea::deleteLast(){
	if (this->GPIOCodeVector.size() > 0){
		GPIODevice* GPIOD = this->GPIOCodeVector.back();
		if (GPIOD->id == PSTART_ID){
			this->ParentMainWindow->warn("Deleting Program Start Block! Entire Project will be reset!");
			this->ParentMainWindow->MainWindowClearButton.click();
			this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
		} else {
			switch(GPIOD->id){
				case LED_ID:{
					Counters::LEDCount--;
					this->LEDVec.pop_back();
					break;
				}
				case PWMLED_ID:{
					Counters::PWMLEDCount--;
					this->PWMLEDVec.pop_back();
					break;
				}
				case RGBLED_ID:{
					Counters::RGBLEDCount--;
					this->RGBLEDVec.pop_back();
					break;
				}
				case BUZ_ID:{
					Counters::BUZZERCount--;
					this->BUZVec.pop_back();
					break;
				}
				case FUNC_ID:{
					Counters::FUNCTIONCount--;
					this->FUNCVec.pop_back();
					break;
				}
				case BTN_ID:{
					Counters::BUTTONCount--;
					this->BTNVec.pop_back();
					break;
				}
				case SLEEP_ID:{
					Counters::SLEEPCount--;
					break;
				}
				case LEDCTRL_ID:{
					Counters::LEDCTRLCount--;
					this->LEDCTRLVec.pop_back();
					break;
				}
				case PWMLEDCTRL_ID:{
					Counters::PWMLEDCount--;
					this->PWMLEDVec.pop_back();
					break;
				}
				case RGBLEDCTRL_ID:{
					Counters::RGBLEDCTRLCount--;
					this->RGBLEDCTRLVec.pop_back();
					break;
				}
				case BUZCTRL_ID:{
					Counters::BUZZERCTRLCount--;
					this->BUZCTRLVec.pop_back();
					break;
				}
				case FCTRL_ID:{
					Counters::FUNCTRLCount--;
					this->FUNCTRLVec.pop_back();
					break;
				}
				case BTNCTRL_ID:{
					Counters::BTNCTRLCount--;
					this->BTNCTRLVec.pop_back();
					break;
				}
			}
			GPIOD->deleteSelf(); // Delete GPIOD
			this->GPIOCodeVector.pop_back(); // Delete Reference to GPIOD
			this->Lines.pop_back(); // Delete last line
			this->LastPoint = QPoint(
				this->GPIOCodeVector.back()->x() + 200, /*Get last GPIODevice, get X coord, add 200 to X*/
				this->GPIOCodeVector.back()->y() + 50 /*Get last GPIODevice, get Y coord, add 50 to Y*/
			); 
				// Line will be redrawn on next paintEvent.
			this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
		}
	} else {
		this->resetSelf();
	}
}

bool DrawArea::checkForPStart(){
	for (GPIODevice* GPIOD : this->GPIOCodeVector){
		if (GPIOD->id == PSTART_ID){
			return true; // ProgramStart exists, somewhere in the area
		}
	}
	return false;
}

void DrawArea::mousePressEvent(QMouseEvent *event){
	// Set of actions to do on mouse press
	if (this->NWMode){
		// Check if New Widget Mode is active
		// I.e if NWMode == false
		// No click action
		// Prevents unnecessary clicks and GPIO Constructions
		this->createGPIODevice(this->activeGPIO, event->x(), event->y());
		this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
		this->NWMode = false;
	}
}

void DrawArea::createGPIODevice(int active, int X, int Y){
	// Constructs a GPIO Device at the given coordinates
	// This function has been specifically created to allow loading of GPIO Devices from a JSON File
	// CSS Doc - Qt likes to inherit background images on its own, despite having 'background-image : none;' in css.
	// Solution - GPIODevice gets a completely blank PNG file
	// Horrible fix, no other option. :-(
	this->activeGPIO = active;
	switch(this->activeGPIO){
		case PSTART_ID:{
			this->ParentMainWindow->log("New Program Start block created! Project will be reset.");
			this->ParentMainWindow->MainWindowClearButton.click(); // Reset self, so that any programstart created will be at top of code
			QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
			ProgramStart* GPIOD = new ProgramStart(this, ParentMainWindow, X, Y, ("Program Start"));
			GPIOD->setGeometry(GPIOBoundBox);
			GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
			GPIOD->show();
			this->LastPoint = QPoint(X + 200, Y + 50);
			this->Lines.push_back(std::make_pair(this->LastPoint, this->LastPoint));
			this->GPIOCodeVector.push_back(GPIOD);
			break;
		}
		case LED_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				LED* GPIOD = new LED(this, ParentMainWindow, X, Y, ("LED " + std::to_string(Counters::LEDCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->LEDVec.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case PWMLED_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				PWMLED* GPIOD = new PWMLED(this, ParentMainWindow, X, Y, ("PWM LED " + std::to_string(Counters::PWMLEDCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->PWMLEDVec.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case RGBLED_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				RGBLED* GPIOD = new RGBLED(this, ParentMainWindow, X, Y, ("RGB LED " + std::to_string(Counters::BTNCTRLCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 100);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->RGBLEDVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BUZ_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Buzzer* GPIOD = new Buzzer(this, ParentMainWindow, X, Y, ("Buzzer " + std::to_string(Counters::BUZZERCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->BUZVec.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}			
		}
		case FUNC_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Function* GPIOD = new Function(this, ParentMainWindow, X, Y, ("Function " + std::to_string(Counters::FUNCTIONCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->FUNCVec.push_back(GPIOD);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BTN_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Button* GPIOD = new Button(this, ParentMainWindow, X, Y, ("Button " + std::to_string(Counters::BUTTONCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->BTNVec.push_back(GPIOD);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case SLEEP_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Sleep* GPIOD = new Sleep(this, ParentMainWindow, X, Y, ("Sleep Timer " + std::to_string(Counters::SLEEPCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}		
		case LEDCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				LEDCtrl* GPIOD = new LEDCtrl(this, ParentMainWindow, X, Y, ("LED Controls " + std::to_string(Counters::LEDCTRLCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->LEDCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case PWMLEDCTRL_ID:{
			break;
		}
		case RGBLEDCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				RGBLEDCtrl* GPIOD = new RGBLEDCtrl(this, ParentMainWindow, X, Y, ("RGB LED Controls " + std::to_string(Counters::RGBLEDCTRLCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 100);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->RGBLEDCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BUZCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				BuzzerCtrl* GPIOD = new BuzzerCtrl(this, ParentMainWindow, X, Y, ("Buzzer Controls " + std::to_string(Counters::BUZZERCTRLCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->BUZCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}

		case FCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				FunctionControl* GPIOD = new FunctionControl(this, ParentMainWindow, X, Y, ("Function Controls " + std::to_string(Counters::FUNCTRLCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->FUNCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BTNCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				ButtonControl* GPIOD = new ButtonControl(this, ParentMainWindow, X, Y, ("Button Controls " + std::to_string(Counters::BTNCTRLCount)));
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; background-color : " + convertToQString(GPIOD->Color) + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 100);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y());
				this->GPIOCodeVector.push_back(GPIOD);
				this->BTNCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		default:{
			this->ParentMainWindow->err("Invalid parameter passed!");
			this->ParentMainWindow->warn("The file you have loaded has been incorrectly modified");
		}
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
	// For each std::pair of points, draw line from p1 to p2;
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
	this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
	this->isNew = true;
	this->GPIOCodeVector.clear();
	this->BUZVec.clear();
	this->LEDVec.clear();
	this->BTNVec.clear();
	this->LEDCTRLVec.clear();
	this->BUZCTRLVec.clear();
	this->FUNCVec.clear();
	this->BTNCTRLVec.clear();
	this->RGBLEDVec.clear();
	Counters::reset();
}

void DrawArea::RefreshSelects(){
	QStringList LEDNames, BuzzerNames, FuncNames, FunctionNames, ButtonNames, RGBLEDNames;
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

	// RGBLEDCTRL Refresh
	for (RGBLED* RGB : this->RGBLEDVec){
		RGBLEDNames << RGB->VarnameEdit.text();
	}

	for (RGBLEDCtrl* RGBC : this->RGBLEDCTRLVec){
		RGBC->RGBLEDSelect.clear();
		RGBC->RGBLEDSelect.insertItems(0, RGBLEDNames);
		RGBC->RGBLEDSelect.setMaxCount(RGBC->RGBLEDSelect.count());
	}

	FunctionNames.clear();
	FuncNames.clear();
	ButtonNames.clear();
	LEDNames.clear();
	BuzzerNames.clear();
	RGBLEDNames.clear();
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

std::string GPIODevice::remoteBuild(){return "";}
std::string GPIODevice::simpleBuild(){return "";}
bool GPIODevice::validateInput(){return false;}

LED::LED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("Name : ", this){
		this->id = LED_ID;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		// Based on Config::legacyMode, change number of pins available
		if (Config::legacyMode == 0){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
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

std::string LED::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.LED(" + convertToStdString(this->PinSelect.currentText()) +
		", pin_factory=__remote_pin_factory)\n";
}

std::string LED::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.LED(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool LED::validateInput(){
	if (this->PinSelect.currentText().isEmpty() || this->VarnameEdit.text().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
}

PWMLED::PWMLED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("Name : ", this){
		this->id = PWMLED_ID;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		// Based on Config::legacyMode, change number of pins available
		if (Config::legacyMode == 0){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : #cceecc;");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyPWMLED" + convertToQString(std::to_string(Counters::PWMLEDCount)));
		this->VarnameEdit.setStyleSheet("background-color : #cceecc;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::PWMLEDCount++;
}

void PWMLED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string PWMLED::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.PWMLED(" + convertToStdString(this->PinSelect.currentText()) +
		", pin_factory=__remote_pin_factory)\n";
}

std::string PWMLED::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.PWMLED(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool PWMLED::validateInput(){
	if (this->PinSelect.currentText().isEmpty() || this->VarnameEdit.text().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
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
		this->id = BUZ_ID;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
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

std::string Buzzer::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.Buzzer(" + convertToStdString(this->PinSelect.currentText()) +
		", pin_factory=__remote_pin_factory)\n";
}

std::string Buzzer::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.Buzzer(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool Buzzer::validateInput(){
	if (this->PinSelect.currentText() == "" || this->VarnameEdit.text() == ""){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
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
		this->id = LEDCTRL_ID;
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

std::string LEDCtrl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->LEDSelect.currentText()) + "." + convertToStdString(this->StateSelect.currentText()) + "()\n";
}

std::string LEDCtrl::simpleBuild(){
	return this->remoteBuild();
}

bool LEDCtrl::validateInput(){
	if (this->LEDSelect.currentText().isEmpty()){
		this->ParentMainWindow->err("No LED Selected for " + this->GPIOName);
		return false;
	}
	return true;
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
		this->id = BUZCTRL_ID;
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

std::string BuzzerCtrl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->BuzzerSelect.currentText()) + "." + convertToStdString(this->StateSelect.currentText()) + "()\n";
}

std::string BuzzerCtrl::simpleBuild(){
	return this->remoteBuild();
}

bool BuzzerCtrl::validateInput(){
	if (this->BuzzerSelect.currentText() == ""){
		this->ParentMainWindow->err("No Buzzer Selected for " + this->GPIOName);
		return false;
	}
	return true;
}

Sleep::Sleep(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DurationEdit(this),
	DisplayLabel(convertToQString(name), this),
	DurationLabel("Duration : ", this){
		this->id = SLEEP_ID;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		DurationLabel.setStyleSheet("border : 0px;");
		this->DurationEdit.setStyleSheet("background-color : #FFFFE0;");
		this->DurationEdit.setText(convertToQString(std::to_string(Config::defaultSleepTime)));
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

std::string Sleep::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return "time.sleep(" + convertToStdString(this->DurationEdit.text()) + ")\n";
}

std::string Sleep::simpleBuild(){
	return this->remoteBuild();
}

bool Sleep::validateInput(){
	if (this->DurationEdit.text() == ""){
		this->ParentMainWindow->err("No Sleep Duration specified for " + this->GPIOName);
		return false;
	}	
	return true;
}

Button::Button(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("Name : ", this){
		this->id = BTN_ID;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
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

std::string Button::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.Button(" + convertToStdString(this->PinSelect.currentText()) +
	", pin_factory=__remote_pin_factory)\n";
}

std::string Button::simpleBuild(){
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.Button(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool Button::validateInput(){
	if (this->PinSelect.currentText().isEmpty() || this->VarnameEdit.text().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
}

Function::Function(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	NameLabel("Name : ", this),
	NameEdit(this),
	BodyLabel("Body : ", this),
	BodyButton("Edit Function Body", this){
		this->id = FUNC_ID;
		QWidget* BodyWindow = new QWidget;
		QTextEdit* FunctionBodyEdit = new QTextEdit(BodyWindow);
		QPushButton* CBody = new QPushButton("Close Window", BodyWindow);
		BodyWindow->setWindowFlags(Qt::WindowMaximizeButtonHint);
		BodyWindow->setFixedSize(640, 480);
		FunctionBodyEdit->setGeometry(50, 20, 540, 380);
		CBody->setGeometry(270, 425, 100, 25);
		FunctionBodyEdit->setFontPointSize(12);
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

std::string Function::remoteBuild(){
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

std::string Function::simpleBuild(){
	return this->remoteBuild();
}

bool Function::validateInput(){
	if (this->NameEdit.text().isEmpty()){
		this->ParentMainWindow->err("No function name provided for " + this->GPIOName);
		return false;
	}
	return true;
}

FunctionControl::FunctionControl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	FunctionSelect(this),
	DisplayLabel(convertToQString(name), this),
	ExecuteLabel("Select Function to execute : ", this)
	{
		this->id = FCTRL_ID;
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

std::string FunctionControl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->FunctionSelect.currentText()) + "()\n";
}

std::string FunctionControl::simpleBuild(){
	return this->remoteBuild();
}

bool FunctionControl::validateInput(){
	if (this->FunctionSelect.currentText() == ""){
		this->ParentMainWindow->err("No function selected for " + this->GPIOName);
		return false;
	}
	return true;
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
		this->id = BTNCTRL_ID;
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

std::string ButtonControl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->ButtonSelect.currentText() + "." + this->StateSelect.currentText() + " = " + this->FunctionSelect.currentText() + "\n");
}

std::string ButtonControl::simpleBuild(){
	return this->remoteBuild();
}

bool ButtonControl::validateInput(){
	if (this->FunctionSelect.currentText() == ""){
		this->ParentMainWindow->err("No Function Selected for " + this->GPIOName);
		return false;
	}
	if (this->ButtonSelect.currentText() == ""){
		this->ParentMainWindow->err("No Button Selected for " + this->GPIOName);
		return false;
	}
	return true;	
}

RGBLED::RGBLED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	RPinSelect(this),
	GPinSelect(this),
	BPinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	RPinLabel("Red : ", this),
	GPinLabel("Green : ", this),
	BPinLabel("Blue : ", this),
	NameLabel("Name : ", this){
		this->id = RGBLED_ID;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		// Based on Config::legacyMode, change number of pins available
		if (Config::legacyMode == 0){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				RPinSelect.addItem(convertToQString(std::to_string(i)));
				GPinSelect.addItem(convertToQString(std::to_string(i)));
				BPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				RPinSelect.addItem(convertToQString(std::to_string(i)));
				GPinSelect.addItem(convertToQString(std::to_string(i)));
				BPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				RPinSelect.addItem(convertToQString(std::to_string(i)));
				GPinSelect.addItem(convertToQString(std::to_string(i)));
				BPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		
		DisplayLabel.setFixedSize(180, 20);
		RPinLabel.setStyleSheet("border : 0px;");
		this->RPinSelect.setStyleSheet("background-color : #F7D479;");
		GPinLabel.setStyleSheet("border : 0px;");
		this->GPinSelect.setStyleSheet("background-color : #F7D479;");
		BPinLabel.setStyleSheet("border : 0px;");
		this->BPinSelect.setStyleSheet("background-color : #F7D479;");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyRGBLED" + convertToQString(std::to_string(Counters::RGBLEDCount)));
		this->VarnameEdit.setStyleSheet("background-color : #F7D479;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&RPinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->RPinSelect, 1, 2);
		this->SelfLayout.addWidget(&GPinLabel, 2, 1);
		this->SelfLayout.addWidget(&this->GPinSelect, 2, 2);
		this->SelfLayout.addWidget(&BPinLabel, 3, 1);
		this->SelfLayout.addWidget(&this->BPinSelect, 3, 2);
		this->SelfLayout.addWidget(&NameLabel, 4, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 4, 2);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		Counters::RGBLEDCount++;
}

void RGBLED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string RGBLED::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.RGBLED(" + convertToStdString(this->RPinSelect.currentText()) +
		", " + convertToStdString(this->GPinSelect.currentText()) +
		", " + convertToStdString(this->BPinSelect.currentText()) +
		", pin_factory=__remote_pin_factory)\n";	
}

std::string RGBLED::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.RGBLED(" + convertToStdString(this->RPinSelect.currentText()) +
	", " + convertToStdString(this->GPinSelect.currentText()) +
	", " + convertToStdString(this->BPinSelect.currentText()) + ")\n";
}

bool RGBLED::validateInput(){
	if (this->VarnameEdit.text().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	if (this->RPinSelect.currentText() == this->GPinSelect.currentText() || 
		this->BPinSelect.currentText() == this->GPinSelect.currentText() ||
		this->RPinSelect.currentText() == this->BPinSelect.currentText()){
			this->ParentMainWindow->err("Duplicate pins provided for " + this->GPIOName);
			return false;
		}
	return true;
}

RGBLEDCtrl::RGBLEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	RPinSlider(Qt::Horizontal, this),
	GPinSlider(Qt::Horizontal, this),
	BPinSlider(Qt::Horizontal, this),
	NameLabel("RGB LED : ", this),
	DisplayLabel(convertToQString(name), this),
	RLabel("Red : ", this),
	GLabel("Green : ", this),
	BLabel("Blue : ", this),
	RGBLEDSelect(this),
	RValueEdit(this),
	GValueEdit(this),
	BValueEdit(this){
		this->id = RGBLEDCTRL_ID;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		NameLabel.setStyleSheet("border : 0px; margin-top : 5px;");
		RLabel.setStyleSheet("border : 0px;");
		GLabel.setStyleSheet("border : 0px;");
		BLabel.setStyleSheet("border : 0px;");
		RPinSlider.setStyleSheet("border : 0px; color : ");
		GPinSlider.setStyleSheet("border : 0px;");
		BPinSlider.setStyleSheet("border : 0px;");
		RValueEdit.setStyleSheet("background-color : #cceecc;");
		GValueEdit.setStyleSheet("background-color : #cceecc;");
		BValueEdit.setStyleSheet("background-color : #cceecc;");
		RPinSlider.setFixedWidth(100);
		RPinSlider.setMaximum(255);
		RPinSlider.setMinimum(0);
		RPinSlider.setValue(0);
		GPinSlider.setFixedWidth(100);
		GPinSlider.setMaximum(255);
		GPinSlider.setMinimum(0);
		GPinSlider.setValue(0);
		BPinSlider.setFixedWidth(100);
		BPinSlider.setMaximum(255);
		BPinSlider.setMinimum(0);
		BPinSlider.setValue(0);
		RValueEdit.setFixedWidth(60);
		GValueEdit.setFixedWidth(60);
		BValueEdit.setFixedWidth(60);
		RValueEdit.setText("0");
		GValueEdit.setText("0");
		BValueEdit.setText("0");
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 3, 1, 1, 1);
		this->SelfLayout.addWidget(&RGBLEDSelect, 4, 1, 1, 2);
		this->SelfLayout.addWidget(&RLabel, 5, 1, 1, 1);
		this->SelfLayout.addWidget(&RPinSlider, 6, 1, 1, 1);
		this->SelfLayout.addWidget(&RValueEdit, 6, 2, 1, 1);
		this->SelfLayout.addWidget(&GLabel, 7, 1, 1, 1);
		this->SelfLayout.addWidget(&GPinSlider, 8, 1, 1, 1);
		this->SelfLayout.addWidget(&GValueEdit, 8, 2, 1, 1);
		this->SelfLayout.addWidget(&BLabel, 9, 1, 1, 1);
		this->SelfLayout.addWidget(&BPinSlider, 10, 1, 1, 1);
		this->SelfLayout.addWidget(&BValueEdit, 10, 2, 1, 1);
		QObject::connect(&ParentMainWindow->MainWindowClearButton, SIGNAL (clicked()), this, SLOT( deleteSelf()));
		QObject::connect(&this->RPinSlider, SIGNAL (valueChanged(int)), this, SLOT (RSliderValueUpdated(int)));
		QObject::connect(&this->GPinSlider, SIGNAL (valueChanged(int)), this, SLOT (GSliderValueUpdated(int)));
		QObject::connect(&this->BPinSlider, SIGNAL (valueChanged(int)), this, SLOT (BSliderValueUpdated(int)));
		QObject::connect(&this->RValueEdit, SIGNAL (textEdited(QString)), this, SLOT (RTextValueUpdated(QString)));
		QObject::connect(&this->GValueEdit, SIGNAL (textEdited(QString)), this, SLOT (GTextValueUpdated(QString)));
		QObject::connect(&this->BValueEdit, SIGNAL (textEdited(QString)), this, SLOT (BTextValueUpdated(QString)));
		Counters::RGBLEDCTRLCount++;
	}

void RGBLEDCtrl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string RGBLEDCtrl::remoteBuild(){
	float red, blue, green;
	red = (float) this->RPinSlider.value() / 255;
	green = (float) this->GPinSlider.value() / 255;
	blue = (float) this->BPinSlider.value() / 255;
	return convertToStdString(this->RGBLEDSelect.currentText()) + ".color = (" +
	std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue) + ")\n";
}

std::string RGBLEDCtrl::simpleBuild(){
	return this->remoteBuild();
	// Same thing, no need to copy.
}

bool RGBLEDCtrl::validateInput(){
	if (this->RGBLEDSelect.currentText().isEmpty()){
		this->ParentMainWindow->err("No RGB LED selected for " + this->GPIOName);
		return false;
	}
	return true;
}

void RGBLEDCtrl::RSliderValueUpdated(int value){
	this->RValueEdit.setText(convertToQString(std::to_string(value)));
}
void RGBLEDCtrl::GSliderValueUpdated(int value){
	this->GValueEdit.setText(convertToQString(std::to_string(value)));
}
void RGBLEDCtrl::BSliderValueUpdated(int value){
	this->BValueEdit.setText(convertToQString(std::to_string(value)));
}
void RGBLEDCtrl::RTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 256){
		this->RPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Red value provided for " + this->GPIOName);
	}	
}
void RGBLEDCtrl::GTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 256){
		this->GPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Green value provided for " + this->GPIOName);
	}	
}
void RGBLEDCtrl::BTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 256){
		this->BPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Blue value provided for " + this->GPIOName);
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

/* DOC
After program has finished executing, it will go into a keepalive state
Various print statements, such as distance sensor print statements will be executed in the keepalive loop
Thus, the user will actually be able to see the output
*/

void ProgramStart::TriggerRemoteBuild(){
	this->ParentMainWindow->log("Validating Project!");
	if (this->validateCode()){
		this->ParentMainWindow->log("Program Validated Succesfully! No errors found!");
		this->ParentMainWindow->log("Building Project, Config : Remote!");
		// Main Code start
		std::ofstream outfile ("script.py"); // Open File 
		for (GPIODevice* GPD : this->ParentDrawArea->GPIOCodeVector)
		{
			outfile << GPD->remoteBuild(); // Write each GPIO Device's build to file
		}
		outfile << "\n";
		this->ParentDrawArea->LoopCode.clear(); // Clear Loopcode
		for (std::string lc : this->ParentDrawArea->LOOPCodeVector){
			this->ParentDrawArea->LoopCode << lc; // Append to stringstream
		}
		// Main Code End
		// Keepalive code start
		std::string tempstr;
		outfile << "print(\"The program will now run in 'keepalive' mode.\\nI.e, you can stop it at any time by pressing CTRL + C\\nAll actions such as Button Actions, Distance Measurements, etc will work.\")\n";
		outfile << "while True:\n";
		outfile << "\ttry:\n";
		while (std::getline(this->ParentDrawArea->LoopCode, tempstr)){
			outfile << "\t\t" + tempstr;
		}
		outfile << "\t\ttime.sleep(" + std::to_string(Config::keepaliveSleepTime) + ")\n";
		outfile << "\texcept KeyboardInterrupt:\n";
		outfile << "\t\timport sys\n";
		outfile << "\t\tsys.exit()\n";
		// Script no longer errs when interrupted by keyboard
		outfile.close();
		// Keepalive code end;
		this->ParentMainWindow->log("Finished Building!");
		// Emit buildcompleted signal
		// MainWindow will run program only after build is completed
		// Could have caused errors with large program files
		emit buildCompleted();
	} else {
		this->ParentMainWindow->warn("Invalid inputs found! Check console!");
	}
}

void ProgramStart::TriggerSimpleBuild(){
	this->ParentMainWindow->log("Validating Project!");
	if (this->validateCode()){
		this->ParentMainWindow->log("Program Validated Succesfully! No errors found!");
		this->ParentMainWindow->log("Building Project, Config : Local!");
		// Main Code start
		std::ofstream outfile ("script.py"); // Open File 
		for (GPIODevice* GPD : this->ParentDrawArea->GPIOCodeVector)
		{
			outfile << GPD->simpleBuild(); // Write each GPIO Device's build to file
		}
		outfile << "\n";
		this->ParentDrawArea->LoopCode.clear(); // Clear Loopcode
		for (std::string lc : this->ParentDrawArea->LOOPCodeVector){
			this->ParentDrawArea->LoopCode << lc; // Append to stringstream
		}
		// Main Code End
		// Keepalive code start
		std::string tempstr;
		outfile << "print(\"The program will now run in 'keepalive' mode.\\nI.e, you can stop it at any time by pressing CTRL + C\\nAll actions such as Button Actions, Distance Measurements, etc will work.\")\n";
		outfile << "while True:\n";
		outfile << "\ttry:\n";
		while (std::getline(this->ParentDrawArea->LoopCode, tempstr)){
			outfile << "\t\t" + tempstr;
		}
		outfile << "\t\ttime.sleep(" + std::to_string(Config::keepaliveSleepTime) + ")\n";
		outfile << "\texcept KeyboardInterrupt:\n";
		outfile << "\t\timport sys\n";
		outfile << "\t\tsys.exit()\n";
		// Script no longer errs when interrupted by keyboard
		outfile.close();
		// Keepalive code end;
		this->ParentMainWindow->log("Finished Building!");
	} else {
		this->ParentMainWindow->warn("Invalid inputs found! Check console!");
	}
}

bool ProgramStart::validateInput(){
	return true;
}

bool ProgramStart::validateCode(){
	for (GPIODevice* GPIOD : this->ParentDrawArea->GPIOCodeVector){
		if (GPIOD->validateInput() == false){
			return false;
			// If any GPIO Device reports invalid input, return false immediately
		}
	}
	return true;
}

ProgramStart::ProgramStart(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X,Y, name),
	DisplayText(this),
	SelfLayout(this){
		this->id = PSTART_ID;
		DisplayText.setText(convertToQString(name));
		DisplayText.setFixedWidth(200);
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		QObject::connect(&(this->ParentMainWindow->RemoteRunButton), SIGNAL (clicked()), this, SLOT (TriggerRemoteBuild()));
		QObject::connect(this, SIGNAL (buildCompleted()), this->ParentMainWindow, SLOT (runRemote()));
		QObject::connect(&(this->ParentMainWindow->MainWindowBuildButton), SIGNAL (clicked()), this, SLOT (TriggerSimpleBuild()));
}

void ProgramStart::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

std::string ProgramStart::remoteBuild(){
	return  "# script.py generated by " + convertToStdString("GPIO Studio v" + getVersionInfo()) + "\n"
			"import gpiozero\n"
			"import time\n"
			"__remote_pin_factory = gpiozero.pins.pigpio.PiGPIOFactory(host=\"" + convertToStdString(this->ParentMainWindow->RaspiIPEdit.text()) + "\")\n";
}

std::string ProgramStart::simpleBuild(){
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
	Config::LoadConfig(&w);
	return app.exec();
}
