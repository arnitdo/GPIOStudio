#include "main.hpp"

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define REVISION 7

#define GPIOD_ID 0
#define PSTART_ID 1
#define LED_ID 2
#define PWMLED_ID 3
#define RGBLED_ID 4
#define BUZ_ID 5
#define FUNC_ID 6
#define BTN_ID 7
#define LINES_ID 8
#define MOTION_ID 9
#define LIGHTS_ID 10
#define DISTS_ID 11
#define SLEEP_ID 12
#define LEDCTRL_ID 13
#define PWMLEDCTRL_ID 14
#define RGBLEDCTRL_ID 15
#define BUZCTRL_ID 16
#define FCTRL_ID 17
#define BTNCTRL_ID 18

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
	int DISTSCount = 1;
	int LIGHTSCount = 1;
	int MOTIONCount = 1;
	int LINESCount = 1;
	int FUNCTIONCount = 1;
	int FUNCTRLCount = 1;
	int BTNCTRLCount = 1;
	int RGBLEDCount = 1;
	int RGBLEDCTRLCount = 1;
	int PWMLEDCount = 1;
	int PWMLEDCTRLCount = 1;
	void reset(){
		BUZZERCount = 1;
		LEDCount = 1;
		LEDCTRLCount = 1;
		BUZZERCTRLCount = 1;
		SLEEPCount = 1;
		BUTTONCount = 1;
		DISTSCount = 1;
		LIGHTSCount = 1;
		MOTIONCount = 1;
		LINESCount = 1;
		FUNCTIONCount = 1;
		FUNCTRLCount = 1;
		BTNCTRLCount = 1;
		RGBLEDCount = 1;
		RGBLEDCTRLCount = 1;
		PWMLEDCount = 1;
		PWMLEDCTRLCount = 1;
	}
}

namespace Config{
	int defaultSleepTime; // Default time in SleepTimer
	int keepaliveSleepTime; // Default time in keepalive
	std::string defaultPiIP; // Default pi IP in Remote
	// Default Colors (Stored as JSON{
	//	"background" : "backgroundcolor",
	// 	"foreground" : "foregroundcolor",
	// "text" : "textcolor"
	// })
	bool overrideVersionWarnings = false;
	json LEDColor,
		PWMLEDColor,
		BuzzerColor,
		LEDCtrlColor,
		PWMLEDCtrlColor,
		BuzzerCtrlColor,
		SleepColor,
		ButtonColor,
		FunctionColor,
		DistanceSensorColor,
		LightSensorColor,
		MotionSensorColor,
		LineSensorColor,
		FunctionCtrlColor,
		ButtonCtrlColor,
		RGBLEDColor,
		RGBLEDCtrlColor,
		ProgramStartColor;
	// End Colors
	int legacyMode;
	/* 
	LEGACY MODE DOC
	Reference - https://elinux.org/RPi_Low-level_peripherals
	legacy mode is defined by an integer - 
	0 - Raspberry Pi Model A, B -  revision 1.0 (26 pin header)
		[0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27]
	1 - Raspberry Pi Model A, B - revision 2.0 (26 pin + 8 pin extra (4 GPIO Extra))
		[2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31]
	2 - Raspberry Pi Model 2, 2B+ .... (Full 40 Pins)
		[2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27]
	 */
	void resetConfig(){
		defaultSleepTime = 5;
		keepaliveSleepTime = 1; 
		defaultPiIP = ""; 
		LEDColor = {
			{"background", "#00ffff"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		PWMLEDColor = {
			{"background", "#69b5e2"},
			{"foreground", "#4ae4f4"},
			{"text", "#000000"}
		},
		BuzzerColor = {
			{"background", "#98fb98"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		LEDCtrlColor = {
			{"background", "#ffb473"},
			{"foreground", "#fffa73"},
			{"text", "#000000"}
		},
		PWMLEDCtrlColor = {
			{"background", "#abe587"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		BuzzerCtrlColor = {
			{"background", "#cacdff"},
			{"foreground", "#cae8ff"},
			{"text", "#000000"}
		},
		SleepColor = {
			{"background", "#fedf00"},
			{"foreground", "#defe00"},
			{"text", "#000000"}
		},
		ButtonColor = {
			{"background", "#aadf0a"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		} ,
		FunctionColor = {
			{"background", "#00cc99"},
			{"foreground", "#ccffd3"},
			{"text", "#000000"}
		},
		DistanceSensorColor = {
			{"background", "#bccc38"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		LightSensorColor = {
			{"background", "#00cc99"},
			{"foreground", "#00CC33"},
			{"text", "#000000"}
		},
		MotionSensorColor = {
			{"background" , "#a2d0d7"},
			{"foreground" , "#a1d6c3"},
			{"text", "#000000"}
		},
		LineSensorColor = {
			{"background", "#98a20b"},
			{"foreground", "#4da30b"},
			{"text", "#000000"}
		},
		FunctionCtrlColor =  {
			{"background", "#fedbf8"},
			{"foreground", "#fedbe7"},
			{"text", "#000000"}
		},
		ButtonCtrlColor = {
			{"background", "#8da3f3"},
			{"foreground", "#8dd6f3"},
			{"text", "#000000"}
		},
		RGBLEDColor = {
			{"background", "#f7957a"},
			{"foreground", "#dbf779"},
			{"text", "#000000"}
		},
		RGBLEDCtrlColor = {
			{"background", "#9acd32"},
			{"foreground", "#dbf779"},
			{"text", "#000000"}
		},
		ProgramStartColor = {
			{"background", "#aaaaaa"},
			{"foreground", "#aaaaaa"},
			{"text", "#000000"}
		};
	// End Colors
	legacyMode = 2;		
	}
	void LoadConfig(MainWindow* MainWin){
		resetConfig();
		std::ifstream configFile;
		configFile.open("config.json", std::ios::in);
		if (!configFile.is_open()){
			// Revert to defaults
			MainWin->err("Error opening config.json file!");
			MainWin->err("Falling back to default configuration");
			resetConfig();
			configFile.close();
		} else {
			// Load Config from JSON File
			json JsonConfigData;
			configFile >> JsonConfigData;
			try {
				defaultPiIP = JsonConfigData.at("defaultPiIP").get<std::string>();
				defaultSleepTime = JsonConfigData.at("defaultSleepTime").get<int64_t>();
				keepaliveSleepTime = JsonConfigData.at("keepaliveSleepTime").get<int64_t>();
				legacyMode = JsonConfigData.at("legacyMode").get<int64_t>();
				overrideVersionWarnings = JsonConfigData.at("overrideVersionWarnings").get<bool>();
				LEDColor = JsonConfigData.at("colors").at("LED");
				PWMLEDColor = JsonConfigData.at("colors").at("PWMLED");
				BuzzerColor = JsonConfigData.at("colors").at("Buzzer");
				LEDCtrlColor = JsonConfigData.at("colors").at("LEDCtrl");
				PWMLEDCtrlColor = JsonConfigData.at("colors").at("PWMLEDCtrl");
				BuzzerCtrlColor = JsonConfigData.at("colors").at("BuzzerCtrl");
				SleepColor = JsonConfigData.at("colors").at("Sleep");
				ButtonColor = JsonConfigData.at("colors").at("Button");
				FunctionColor = JsonConfigData.at("colors").at("Function");
				DistanceSensorColor = JsonConfigData.at("colors").at("DistanceSensor");
				LineSensorColor = JsonConfigData.at("colors").at("LineSensor");
				MotionSensorColor = JsonConfigData.at("colors").at("MotionSensor");
				LightSensorColor = JsonConfigData.at("colors").at("LightSensor");
				FunctionCtrlColor = JsonConfigData.at("colors").at("FunctionControl");
				ButtonCtrlColor = JsonConfigData.at("colors").at("ButtonControl");
				RGBLEDColor = JsonConfigData.at("colors").at("RGBLED");
				RGBLEDCtrlColor = JsonConfigData.at("colors").at("RGBLEDCtrl");
				ProgramStartColor = JsonConfigData.at("colors").at("ProgramStart");
			} catch (json::out_of_range& JSONExcept){
				if (JSONExcept.id == 403){
					MainWin->err("Unable to set configuration. config.json file is invalid");
					MainWin->err("Falling back to default configuration");
					resetConfig();
				}
			}
			if (overrideVersionWarnings) MainWin->warn("Overriding version warnings. This may cause undefined behaviour!");
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
	this->setWindowIcon(QIcon("static/icon.png"));
	this->RemoteWindow.setWindowIcon(QIcon("static/icon.png"));
	this->AboutWindow.setWindowIcon(QIcon("static/icon.png"));
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
	this->RemoteWindow.setWindowTitle("Run Script Remotely");
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
	MainWindowDeleteLastButton.setIcon(QIcon("static/menubar/undo.svg"));
	MainWindowDeleteLastButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
	MainWindowLayout.addWidget(&MainWindowDeleteLastButton, 0, 0);
	
	// Clear All Button
	MainWindowClearButton.setFixedHeight(36);
	MainWindowClearButton.setText(" Clear All");
	MainWindowClearButton.setIcon(QIcon("static/menubar/clear.svg"));
	MainWindowClearButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	MainWindowLayout.addWidget(&MainWindowClearButton, 0, 1);

	// Refresh Button
	MainWindowRefreshButton.setFixedHeight(36);
	MainWindowRefreshButton.setText(" Refresh");
	MainWindowRefreshButton.setIcon(QIcon("static/menubar/refresh.svg"));
	MainWindowRefreshButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
	MainWindowLayout.addWidget(&MainWindowRefreshButton, 0, 2);

	// Build Button
	MainWindowBuildButton.setFixedHeight(36);
	MainWindowBuildButton.setText(" Build");
	MainWindowBuildButton.setIcon(QIcon("static/menubar/hammer.svg"));
	MainWindowBuildButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
	MainWindowLayout.addWidget(&MainWindowBuildButton, 0, 3);

	// Remote Run Button
	MainWindowRemoteButton.setFixedHeight(36);
	MainWindowRemoteButton.setText(" Run Remote");
	MainWindowRemoteButton.setIcon(QIcon("static/menubar/remote.svg"));
	MainWindowRemoteButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
	MainWindowLayout.addWidget(&MainWindowRemoteButton, 0, 4);

	// Load Button
	MainWindowLoadButton.setFixedHeight(36);
	MainWindowLoadButton.setText(" Load File");
	MainWindowLoadButton.setIcon(QIcon("static/menubar/package.svg"));
	MainWindowLoadButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
	MainWindowLayout.addWidget(&MainWindowLoadButton, 0, 5);

	// Save Button
	MainWindowSaveButton.setFixedHeight(36);
	MainWindowSaveButton.setText(" Save File");
	MainWindowSaveButton.setIcon(QIcon("static/menubar/save.svg"));
	MainWindowSaveButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
	MainWindowLayout.addWidget(&MainWindowSaveButton, 0, 6);

	// Help Button
	MainWindowHelpButton.setFixedHeight(36);
	MainWindowHelpButton.setText(" Help");
	MainWindowHelpButton.setIcon(QIcon("static/menubar/help.svg"));
	MainWindowHelpButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_H));
	MainWindowLayout.addWidget(&MainWindowHelpButton, 0, 7);

	// About Button
	MainWindowAboutButton.setFixedHeight(36);
	MainWindowAboutButton.setText(" About");
	MainWindowAboutButton.setIcon(QIcon("static/menubar/info.svg"));
	MainWindowAboutButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_A));
	MainWindowLayout.addWidget(&MainWindowAboutButton, 0, 8);

	// Quit Button
	MainWindowQuitButton.setFixedHeight(36);
	MainWindowQuitButton.setText(" Quit Application!");
	MainWindowQuitButton.setIcon(QIcon("static/menubar/logout.svg"));
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
	QMessageBox ResetMessageBox (this);
	ResetMessageBox.setText("Clearing entire workbench!\nDo you want to proceed?");
	ResetMessageBox.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
	ResetMessageBox.setIcon(QMessageBox::Question);
	if (ResetMessageBox.exec() == QMessageBox::Yes){
		this->MainWindowDrawArea.resetSelf();
		emit deleteGPIO();
	}	
}

void MainWindow::showRemoteWindow(){
	this->RemoteWindow.show();
}

void MainWindow::hideRemoteWindow(){
	this->RemoteWindow.hide();
}

void MainWindow::runRemote(){
	if (!this->RaspiIPEdit.text().simplified().isEmpty()){
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
	ButtonLabelMap.insert({DISTS_ID, "Distance Sensor"});
	ButtonLabelMap.insert({LIGHTS_ID, "Light Sensor"});
	ButtonLabelMap.insert({MOTION_ID, "Motion Sensor"});
	ButtonLabelMap.insert({LINES_ID, "Line Sensor"});
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
	if (!fname.simplified().isEmpty() && !fname.endsWith("config.json", Qt::CaseSensitive)){
		this->ParentMainWindow->log("Opening file " + convertToStdString(fname));
		std::ifstream JSONFileIn (convertToStdString(fname));
		json JSON;
		JSONFileIn >> JSON;
		bool versionMatch = JSON["version"].get<std::string>() == convertToStdString(getVersionInfo());
		if (versionMatch || Config::overrideVersionWarnings){
			// Version Matches, Proceed!
			// Emulate button click, clears the entire board
			for (json GPIOJSON : JSON["json"]){
				try {
					this->createGPIODevice(GPIOJSON);
				} catch (json::out_of_range& JSONExcept){
					if (JSONExcept.id == 403){
						this->ParentMainWindow->log("Invalid file provided!Project will be reset!");
						emit this->ParentMainWindow->deleteGPIO();
						this->resetSelf();
					}
				}
			}
		} else {
			this->ParentMainWindow->err("Version Mismatch!");
			this->ParentMainWindow->err("The version of the file you have provided does not match the current version of GPIO Studio!");
		}
	} else {
		if (fname.simplified().isEmpty()){
			this->ParentMainWindow->log("No file selected for opening!");
		} else if (fname.endsWith("config.json", Qt::CaseSensitive)){
			this->ParentMainWindow->err("Config files cannot be opened!");
		}
	}
};

void DrawArea::saveToJson(){
	QFileDialog OpenFileDialog;
	OpenFileDialog.setDefaultSuffix("json");
	QString fname = OpenFileDialog.getSaveFileName(this, "Save GPIO Project", "", "JSON Files (*.json)");
	if (!fname.simplified().isEmpty()){
		this->ParentMainWindow->log("Saving to File " + convertToStdString(fname));
		json JsonWrite;
		JsonWrite["version"] = convertToStdString(getVersionInfo());
		json GPIOArray = json::array();
		for (GPIODevice* GPIOD : this->GPIOCodeVector){
			GPIOArray.push_back(GPIOD->toJson());
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
			emit this->ParentMainWindow->deleteGPIO();
			this->resetSelf();
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
				case DISTS_ID:{
					Counters::DISTSCount--;
					break;
				}
				case LIGHTS_ID:{
					Counters::LIGHTSCount--;
					break;
				}
				case MOTION_ID:{
					Counters::MOTIONCount--;
					break;
				}
				case LINES_ID:{
					Counters::LINESCount--;
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
					Counters::PWMLEDCTRLCount--;
					this->PWMLEDCTRLVec.pop_back();
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
			int YOffset = 75; // Default Y offset for 200 * 100 blocks
			// Increases for 200 * 200 blocks by 75 (Y = 150)
			GPIODevice* LastGPIO = this->GPIOCodeVector.back();
			if (LastGPIO->id == RGBLED_ID || LastGPIO->id == RGBLEDCTRL_ID || LastGPIO->id == BTNCTRL_ID){
				YOffset += 75;
			}
			this->LastPoint = QPoint(
				LastGPIO->x() + 200, /*Get last GPIODevice, get X coord, add 200 to X*/
				LastGPIO->y() + YOffset /*Get last GPIODevice, get Y coord, add Offset to Y*/
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
	if (this->NWMode && event->button() == Qt::LeftButton){
		// Check if New Widget Mode is active
		// I.e if NWMode == false
		// No click action
		// Prevents unnecessary clicks and GPIO Constructions
		json ClickJSON;
		ClickJSON["id"] = this->activeGPIO;
		ClickJSON["x"] = event->x();
		ClickJSON["y"] = event->y();
		// Pack mouse event into JSON, and pass as reference, not copy
		this->createGPIODevice(ClickJSON);
		this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
		this->NWMode = false;
	}
}

void DrawArea::createGPIODevice(json& GPIOData){
	// Constructs a GPIO Device at the given coordinates
	// This function has been specifically created to allow loading of GPIO Devices from a JSON File
	// CSS Doc - Qt likes to inherit background images on its own, despite having 'background-image : none;' in css.
	// Solution - GPIODevice gets a completely blank PNG file
	// Horrible fix, no other option. :-(
	int id = GPIOData.at("id").get<int64_t>();
	int X = GPIOData.at("x").get<int64_t>();
	int Y = GPIOData.at("y").get<int64_t>();
	this->activeGPIO = id;
	switch(this->activeGPIO){
		case PSTART_ID:{
			QMessageBox NewPStartDialog (this->ParentMainWindow);
			NewPStartDialog.setIcon(QMessageBox::Warning);
			NewPStartDialog.setText("You are creating a new Program Start Block.\nAll previous blocks will be removed.\nDo you want to continue?");
			NewPStartDialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			switch(NewPStartDialog.exec()){
				case QMessageBox::Yes:{
					emit this->ParentMainWindow->deleteGPIO();
					this->resetSelf();
					// Reset self, so that any programstart created will be at top of code
					QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
					ProgramStart* GPIOD = new ProgramStart(this, ParentMainWindow, X, Y, ("Program Start"));
					GPIOD->setGeometry(GPIOBoundBox);
					GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
					GPIOD->show();
					this->LastPoint = QPoint(X + 200, Y + 25);
					this->Lines.push_back(std::make_pair(this->LastPoint, this->LastPoint));
					this->GPIOCodeVector.push_back(GPIOD);
					break;
				}
				case QMessageBox::No:{
					this->ParentMainWindow->log("Cancelled creation of Program Start Block");
				}
			}
			break;
		}
		case LED_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				LED* GPIOD = new LED(this, ParentMainWindow, X, Y, ("LED " + std::to_string(Counters::LEDCount)));
				if (GPIOData.contains("LEDPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("LEDPin").get<int64_t>()))
					);
				}
				// Get LED Pin and LED Name property, if exists, and set text
				if (GPIOData.contains("LEDName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("LEDName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("PWMLEDPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("PWMLEDPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("PWMLEDName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("PWMLEDName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				RGBLED* GPIOD = new RGBLED(this, ParentMainWindow, X, Y, ("RGB LED " + std::to_string(Counters::RGBLEDCount)));
				if (GPIOData.contains("RGBLEDPinR")){
					GPIOD->RPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("RGBLEDPinR").get<int64_t>()))
					);
				}
				if (GPIOData.contains("RGBLEDPinG")){
					GPIOD->GPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("RGBLEDPinG").get<int64_t>()))
					);
				}
				if (GPIOData.contains("RGBLEDPinB")){
					GPIOD->BPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("RGBLEDPinB").get<int64_t>()))
					);
				}
				if (GPIOData.contains("RGBLEDName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("RGBLEDName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 100);
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
				if (GPIOData.contains("buzzerPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("buzzerPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("buzzerName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("buzzerName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("functionName")){
					GPIOD->NameEdit.setText(
						convertToQString(GPIOData.at("functionName").get<std::string>())
					);
				}
				if (GPIOData.contains("functionBody")){
					GPIOD->FunctionBody->setText(
						convertToQString(GPIOData.at("functionBody").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("buttonPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("buttonPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("buttonName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("buttonName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->BTNVec.push_back(GPIOD);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case DISTS_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				DistanceSensor* GPIOD = new DistanceSensor(this, ParentMainWindow, X, Y, ("Distance Sensor " + std::to_string(Counters::DISTSCount)));
				if (GPIOData.contains("distanceEchoPin")){
					GPIOD->EchoPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("distanceEchoPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("distanceTrigPin")){
					GPIOD->TrigPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("distanceTrigPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("distanceName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("distanceName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case LIGHTS_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				LightSensor* GPIOD = new LightSensor(this, ParentMainWindow, X, Y, ("Light Sensor " + std::to_string(Counters::LIGHTSCount)));
				if (GPIOData.contains("lightPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("lightPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("lightName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("lightName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case MOTION_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				MotionSensor* GPIOD = new MotionSensor(this, ParentMainWindow, X, Y, ("Motion Sensor " + std::to_string(Counters::MOTIONCount)));
				if (GPIOData.contains("motionPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("motionPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("motionName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("motionName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case LINES_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				LineSensor* GPIOD = new LineSensor(this, ParentMainWindow, X, Y, ("Line Sensor " + std::to_string(Counters::LINESCount)));
				if (GPIOData.contains("linePin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("linePin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("lineName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("lineName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("sleepTime")){
					GPIOD->DurationEdit.setText(
						convertToQString(std::to_string(GPIOData.at("sleepTime").get<int64_t>()))
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("targetLEDName")){
					GPIOD->LEDSelect.setCurrentText(
						convertToQString(GPIOData.at("targetLEDName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetLEDState")){
					GPIOD->StateSelect.setCurrentText(
						convertToQString(GPIOData.at("targetLEDState").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				PWMLEDCtrl* GPIOD = new PWMLEDCtrl(this, ParentMainWindow, X, Y, ("PWM LED Controls " + std::to_string(Counters::PWMLEDCTRLCount)));
				if (GPIOData.contains("targetPWMLEDName")){
					GPIOD->PWMLEDSelect.setCurrentText(
						convertToQString(GPIOData.at("targetPWMLEDName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetPWMLEDvalue")){
					GPIOD->ValueEdit.setText(
						convertToQString(std::to_string(GPIOData.at("targetPWMLEDvalue").get<int64_t>()))
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->PWMLEDCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case RGBLEDCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				RGBLEDCtrl* GPIOD = new RGBLEDCtrl(this, ParentMainWindow, X, Y, ("RGB LED Controls " + std::to_string(Counters::RGBLEDCTRLCount)));
				if (GPIOData.contains("targetRGBLEDName")){
					GPIOD->RGBLEDSelect.setCurrentText(
						convertToQString(GPIOData.at("targetRGBLEDName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetRGBLEDRed")){
					GPIOD->RPinSlider.setValue(
						GPIOData.at("targetRGBLEDRed").get<int64_t>()
					);
				}
				if (GPIOData.contains("targetRGBLEDGreen")){
					GPIOD->GPinSlider.setValue(
						GPIOData.at("targetRGBLEDGreen").get<int64_t>()
					);
				}
				if (GPIOData.contains("targetRGBLEDBlue")){
					GPIOD->BPinSlider.setValue(
						GPIOData.at("targetRGBLEDBlue").get<int64_t>()
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("targetBuzzerName")){
					GPIOD->BuzzerSelect.setCurrentText(
						convertToQString(GPIOData.at("targetBuzzerName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetBuzzerState")){
					GPIOD->StateSelect.setCurrentText(
						convertToQString(GPIOData.at("targetBuzzerState").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("targetFunctionName")){
					GPIOD->FunctionSelect.setCurrentText(
						convertToQString(GPIOData.at("targetFunctionName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetFunctionLoopState")){
					GPIOD->LoopCheckBox.setCheckState(
						GPIOData.at("targetFunctionLoopState").get<bool>() ? Qt::Checked : Qt::Unchecked
						// Quick ternary operator
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
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
				if (GPIOData.contains("targetButtonName")){
					GPIOD->ButtonSelect.setCurrentText(
						convertToQString(GPIOData.at("targetButtonName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetButtonState")){
					GPIOD->StateSelect.setCurrentText(
						convertToQString(GPIOData.at("targetButtonState").get<std::string>())
					);
				}
				if (GPIOData.contains("targetFunctionName")){
					GPIOD->FunctionSelect.setCurrentText(
						convertToQString(GPIOData.at("targetFunctionName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 100);
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
		QPoint Point1 = std::get<0>(PointPair),
			   Point2 = std::get<1>(PointPair);
		// Upper Mid Point
		QPoint UMidPoint = QPoint(
			(Point1.x() + ((Point2.x() - Point1.x()) / 2)),
			Point1.y()
		);
		QPoint LMidPoint = QPoint(
			(Point1.x() + ((Point2.x() - Point1.x()) / 2)),
			Point2.y()
		);
		p->drawLine(Point1, UMidPoint);
		p->drawLine(UMidPoint, LMidPoint);
		p->drawLine(LMidPoint, Point2);
	};
	p->end();
	event->ignore();
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
	this->FUNCTRLVec.clear();
	this->BTNCTRLVec.clear();
	this->RGBLEDVec.clear();
	this->RGBLEDCTRLVec.clear();
	this->PWMLEDVec.clear();
	this->PWMLEDCTRLVec.clear();
	Counters::reset();
}

void DrawArea::RefreshSelects(){
	QStringList LEDNames,
		BuzzerNames,
		FuncNames,
		FunctionNames,
		ButtonNames,
		RGBLEDNames,
		PWMLEDNames;
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

	// PWMLED Refresh
	for (PWMLED* PWM : this->PWMLEDVec){
		PWMLEDNames << PWM->VarnameEdit.text();
	}

	for (PWMLEDCtrl* PWMC : this->PWMLEDCTRLVec){
		PWMC->PWMLEDSelect.clear();
		PWMC->PWMLEDSelect.insertItems(0, PWMLEDNames);
		PWMC->PWMLEDSelect.setMaxCount(PWMC->PWMLEDSelect.count());
	}

	FunctionNames.clear();
	FuncNames.clear();
	ButtonNames.clear();
	LEDNames.clear();
	BuzzerNames.clear();
	RGBLEDNames.clear();
	PWMLEDNames.clear();
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
	this->ParentDrawArea = parent;
	this->ParentMainWindow = parentMainWindow;
	this->XCoord = X;
	this->YCoord = Y;
	this->GPIOName = name;
	QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
}

void GPIODevice::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json GPIODevice::toJson(){
	json DataJSON;
	DataJSON["id"] = GPIOD_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	return DataJSON;
	// Default JSON return
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
		this->backgroundcolor = convertToQString(Config::LEDColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::LEDColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::LEDColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		// Based on Config::legacyMode, change number of pins available
		if (Config::legacyMode == 0){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyLED" + convertToQString(std::to_string(Counters::LEDCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::LEDCount++;
}

void LED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json LED::toJson(){
	json DataJSON;
	DataJSON["id"] = LED_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["LEDPin"] = std::stoi(
		convertToStdString(this->PinSelect.currentText())
	);
	DataJSON["LEDName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
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
	if (this->PinSelect.currentText().simplified().isEmpty() || this->VarnameEdit.text().simplified().isEmpty()){
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
		this->backgroundcolor = convertToQString(Config::PWMLEDColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::PWMLEDColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::PWMLEDColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		// Based on Config::legacyMode, change number of pins available
		if (Config::legacyMode == 0){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyPWMLED" + convertToQString(std::to_string(Counters::PWMLEDCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::PWMLEDCount++;
}

void PWMLED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json PWMLED::toJson(){
	json DataJSON;
	DataJSON["id"] = PWMLED_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["PWMLEDPin"] = std::stoi(
		convertToStdString(this->PinSelect.currentText())
	);
	DataJSON["PWMLEDName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
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
	if (this->PinSelect.currentText().simplified().isEmpty() || this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
}

PWMLEDCtrl::PWMLEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PWMLEDSelect(this),
	ValueEdit(this),
	DisplayLabel(convertToQString(name), this),
	PWMLEDLabel("PWMLED : ", this),
	ValueLabel("Value : ", this)
	{
		this->id = PWMLEDCTRL_ID;
		this->backgroundcolor = convertToQString(Config::PWMLEDColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::PWMLEDColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::PWMLEDColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		PWMLEDLabel.setStyleSheet("border : 0px;");
		ValueEdit.setMaxLength(3);
		ValueEdit.setText("0");
		PWMLEDSelect.setStyleSheet("background-color : " + this->foreground + ";");
		ValueEdit.setStyleSheet("background-color : " + this->foreground + ";");
		ValueLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PWMLEDLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PWMLEDSelect, 1, 2);
		this->SelfLayout.addWidget(&ValueLabel, 2, 1);
		this->SelfLayout.addWidget(&this->ValueEdit, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::PWMLEDCTRLCount++;
	}

void PWMLEDCtrl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json PWMLEDCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = PWMLEDCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetPWMLEDName"] = convertToStdString(this->PWMLEDSelect.currentText());
	DataJSON["targetPWMLEDvalue"] = std::stoi(convertToStdString(this->ValueEdit.text()));
	return DataJSON;
}

std::string PWMLEDCtrl::remoteBuild(){
	float value = std::stof(convertToStdString(this->ValueEdit.text())) / 100; 
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->PWMLEDSelect.currentText()) + ".value = " + std::to_string(value) + "\n";
}

std::string PWMLEDCtrl::simpleBuild(){
	return this->remoteBuild();
}

bool PWMLEDCtrl::validateInput(){
	int value = std::stoi(convertToStdString(this->ValueEdit.text()));
	bool valueInLimits = (0 <= value && value <= 100) ? true : false; 
	if (this->PWMLEDSelect.currentText().simplified().isEmpty()){
		this->ParentMainWindow->err("No PWM LED Selected for " + this->GPIOName);
		return false;
	}
	if (!valueInLimits){
		this->ParentMainWindow->err("Invalid value provided for " + this->GPIOName);
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
		this->backgroundcolor = convertToQString(Config::BuzzerColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::BuzzerColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::BuzzerColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyBuzzer" + convertToQString(std::to_string(Counters::BUZZERCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
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

json Buzzer::toJson(){
	json DataJSON;
	DataJSON["id"] = BUZ_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["buzzerPin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["buzzerName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
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
		this->backgroundcolor = convertToQString(Config::LEDCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::LEDCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::LEDCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		LEDLabel.setStyleSheet("border : 0px;");
		LEDSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.addItem("on");
		StateSelect.addItem("off");
		StateLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&LEDLabel, 1, 1);
		this->SelfLayout.addWidget(&this->LEDSelect, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 2, 1);
		this->SelfLayout.addWidget(&this->StateSelect, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::LEDCTRLCount++;
	}

void LEDCtrl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json LEDCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = LEDCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetLEDName"] = convertToStdString(this->LEDSelect.currentText());
	DataJSON["targetLEDState"] = convertToStdString(this->StateSelect.currentText());
	return DataJSON;
}

std::string LEDCtrl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->LEDSelect.currentText()) + "." + convertToStdString(this->StateSelect.currentText()) + "()\n";
}

std::string LEDCtrl::simpleBuild(){
	return this->remoteBuild();
}

bool LEDCtrl::validateInput(){
	if (this->LEDSelect.currentText().simplified().isEmpty()){
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
		this->backgroundcolor = convertToQString(Config::BuzzerCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::BuzzerCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::BuzzerCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		BuzzerLabel.setStyleSheet("border : 0px;");
		BuzzerSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.addItem("on");
		StateSelect.addItem("off");
		StateLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&BuzzerLabel, 1, 1);
		this->SelfLayout.addWidget(&this->BuzzerSelect, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 2, 1);
		this->SelfLayout.addWidget(&this->StateSelect, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::BUZZERCTRLCount++;
	}

void BuzzerCtrl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json BuzzerCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = BUZCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetBuzzerName"] = convertToStdString(this->BuzzerSelect.currentText());
	DataJSON["targetBuzzerState"] = convertToStdString(this->StateSelect.currentText());
	return DataJSON;
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
		this->backgroundcolor = convertToQString(Config::SleepColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::SleepColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::SleepColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		DurationLabel.setStyleSheet("border : 0px;");
		this->DurationEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->DurationEdit.setText(convertToQString(std::to_string(Config::defaultSleepTime)));
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&DurationLabel, 1, 1);
		this->SelfLayout.addWidget(&this->DurationEdit, 1, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::SLEEPCount++;
}

void Sleep::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json Sleep::toJson(){
	json DataJSON;
	DataJSON["id"] = SLEEP_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["sleepTime"] = std::stoi(convertToStdString(this->DurationEdit.text()));
	return DataJSON;
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
		this->backgroundcolor = convertToQString(Config::ButtonColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::ButtonColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::ButtonColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyButton" + convertToQString(std::to_string(Counters::BUTTONCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::BUTTONCount++;
}

void Button::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json Button::toJson(){
	json DataJSON;
	DataJSON["id"] = BTN_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["buttonPin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["buttonName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
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
	if (this->PinSelect.currentText().simplified().isEmpty() || this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
}

DistanceSensor::DistanceSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	TrigPinLabel("Trigger Pin : ", this),
	EchoPinLabel("Echo Pin : ", this),
	TrigPinSelect(this),
	EchoPinSelect(this),
	NameLabel("Name : ", this),
	VarnameEdit(this){
		this->id = DISTS_ID;
		this->backgroundcolor = convertToQString(Config::DistanceSensorColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::DistanceSensorColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::DistanceSensorColor.at("text").get<std::string>());
		this->ParentDrawArea = parent; 
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				TrigPinSelect.addItem(convertToQString(std::to_string(i)));
				EchoPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				TrigPinSelect.addItem(convertToQString(std::to_string(i)));
				EchoPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				TrigPinSelect.addItem(convertToQString(std::to_string(i)));
				EchoPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		TrigPinLabel.setStyleSheet("border : 0px;");
		EchoPinLabel.setStyleSheet("border : 0px;");
		NameLabel.setStyleSheet("border : 0px;");
		TrigPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		EchoPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setText("MyDistanceSensor" + convertToQString(std::to_string(Counters::DISTSCount)));
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&EchoPinLabel, 2, 1);
		this->SelfLayout.addWidget(&EchoPinSelect, 2, 2);
		this->SelfLayout.addWidget(&TrigPinLabel, 3, 1);
		this->SelfLayout.addWidget(&TrigPinSelect, 3, 2);
		this->SelfLayout.addWidget(&NameLabel, 4, 1);
		this->SelfLayout.addWidget(&VarnameEdit, 4, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::DISTSCount++;
	}

void DistanceSensor::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json DistanceSensor::toJson(){
	json DataJSON;
	DataJSON["id"] = DISTS_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["distanceEchoPin"] = std::stoi(convertToStdString(this->EchoPinSelect.currentText()));
	DataJSON["distanceTrigPin"] = std::stoi(convertToStdString(this->TrigPinSelect.currentText()));
	DataJSON["distanceName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string DistanceSensor::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Distance recorded by " + convertToStdString(this->VarnameEdit.text()) + " in centimeters : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".distance * 100)))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);	
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.DistanceSensor(" + convertToStdString(this->EchoPinSelect.currentText()) +
	", " + convertToStdString(this->TrigPinSelect.currentText()) + ", pin_factory=__remote_pin_factory)\n";
}

std::string DistanceSensor::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Distance recorded by " + convertToStdString(this->VarnameEdit.text()) + " in centimeters : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".distance * 100)))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.DistanceSensor(" + convertToStdString(this->EchoPinSelect.currentText()) +
	", " + convertToStdString(this->TrigPinSelect.currentText()) + ")\n";
}

bool DistanceSensor::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name provided for " + this->GPIOName);
		return false;
	}
	if (this->EchoPinSelect.currentText() == this->TrigPinSelect.currentText()){
		this->ParentMainWindow->err("Duplicate Pins provided for " + this->GPIOName);
		return false;
	}
	return true;
}

LightSensor::LightSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	PinSelect(this),
	NameLabel("Name : ", this),
	VarnameEdit(this){
		this->id = LIGHTS_ID;
		this->backgroundcolor = convertToQString(Config::LightSensorColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::LightSensorColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::LightSensorColor.at("text").get<std::string>());
		this->ParentDrawArea = parent; 
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		PinLabel.setStyleSheet("border : 0px;");
		NameLabel.setStyleSheet("border : 0px;");
		DisplayLabel.setFixedHeight(20);
		PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setText("MyLightSensor" + convertToQString(std::to_string(Counters::LIGHTSCount)));
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 2, 1);
		this->SelfLayout.addWidget(&PinSelect, 2, 2);
		this->SelfLayout.addWidget(&NameLabel, 3, 1);
		this->SelfLayout.addWidget(&VarnameEdit, 3, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::LIGHTSCount++;
	}

void LightSensor::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json LightSensor::toJson(){
	json DataJSON;
	DataJSON["id"] = LIGHTS_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["lightPin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["lightName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string LightSensor::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Light Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".value * 100)) + \"%\")\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);	
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.LightSensor(" + convertToStdString(this->PinSelect.currentText()) + ", pin_factory=__remote_pin_factory)\n";
}

std::string LightSensor::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Light Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".value * 100)) + \"%\")\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.LightSensor(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool LightSensor::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name provided for " + this->GPIOName);
		return false;
	}
	return true;
}

MotionSensor::MotionSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	PinSelect(this),
	NameLabel("Name : ", this),
	VarnameEdit(this){
		this->id = MOTION_ID;
		this->backgroundcolor = convertToQString(Config::MotionSensorColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::MotionSensorColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::MotionSensorColor.at("text").get<std::string>());
		this->ParentDrawArea = parent; 
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		PinLabel.setStyleSheet("border : 0px;");
		NameLabel.setStyleSheet("border : 0px;");
		DisplayLabel.setFixedHeight(20);
		PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setText("MyMotionSensor" + convertToQString(std::to_string(Counters::MOTIONCount)));
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 2, 1);
		this->SelfLayout.addWidget(&PinSelect, 2, 2);
		this->SelfLayout.addWidget(&NameLabel, 3, 1);
		this->SelfLayout.addWidget(&VarnameEdit, 3, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::MOTIONCount++;
	}

void MotionSensor::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json MotionSensor::toJson(){
	json DataJSON;
	DataJSON["id"] = MOTION_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["motionPin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["motionName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string MotionSensor::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Motion Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(" + convertToStdString(this->VarnameEdit.text()) + ".value))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);	
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.MotionSensor(" + convertToStdString(this->PinSelect.currentText()) + ", pin_factory=__remote_pin_factory)\n";
}

std::string MotionSensor::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Motion Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(" + convertToStdString(this->VarnameEdit.text()) + ".value))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.MotionSensor(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool MotionSensor::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name provided for " + this->GPIOName);
		return false;
	}
	return true;
}

LineSensor::LineSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	PinSelect(this),
	NameLabel("Name : ", this),
	VarnameEdit(this){
		this->id = LINES_ID;
		this->backgroundcolor = convertToQString(Config::LineSensorColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::LineSensorColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::LineSensorColor.at("text").get<std::string>());
		this->ParentDrawArea = parent; 
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		PinLabel.setStyleSheet("border : 0px;");
		NameLabel.setStyleSheet("border : 0px;");
		DisplayLabel.setFixedHeight(20);
		PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setText("MyLineSensor" + convertToQString(std::to_string(Counters::LINESCount)));
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 2, 1);
		this->SelfLayout.addWidget(&PinSelect, 2, 2);
		this->SelfLayout.addWidget(&NameLabel, 3, 1);
		this->SelfLayout.addWidget(&VarnameEdit, 3, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::LINESCount++;
	}

void LineSensor::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json LineSensor::toJson(){
	json DataJSON;
	DataJSON["id"] = LINES_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["linePin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["lineName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string LineSensor::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Line Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".value * 100)) + \"%\")\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);	
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.LineSensor(" + convertToStdString(this->PinSelect.currentText()) + ", pin_factory=__remote_pin_factory)\n";
}

std::string LineSensor::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Line Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".value * 100)) + \"%\")\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.LineSensor(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool LineSensor::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name provided for " + this->GPIOName);
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
		this->backgroundcolor = convertToQString(Config::FunctionColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::FunctionColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::FunctionColor.at("text").get<std::string>());
		QWidget* BodyWindow = new QWidget;
		BodyWindow->setWindowIcon(QIcon("static/icon.png"));
		QTextEdit* FunctionBodyEdit = new QTextEdit(BodyWindow);
		QPushButton* CBody = new QPushButton("Close Window", BodyWindow);
		BodyWindow->setFixedSize(640, 480);
		FunctionBodyEdit->setGeometry(50, 20, 540, 380);
		CBody->setGeometry(270, 425, 100, 25);
		FunctionBodyEdit->setFontPointSize(12);
		FunctionBodyEdit->append("# Write your function code here\n# You do not need to indent the code body\nprint(\"Hello, World!\")\n");
		this->FunctionBodyWindow = BodyWindow;
		this->FunctionBody = FunctionBodyEdit;
		this->CloseBodyButton = CBody;
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		NameLabel.setStyleSheet("border : 0px;");
		BodyLabel.setStyleSheet("border : 0px;");
		BodyLabel.setFixedHeight(20);
		BodyButton.setFixedHeight(20);
		NameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		NameEdit.setText(convertToQString("MyFunction" + std::to_string(Counters::FUNCTIONCount)));
		BodyButton.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 1, 1);
		this->SelfLayout.addWidget(&NameEdit, 1, 2);
		this->SelfLayout.addWidget(&BodyLabel, 2, 1);
		this->SelfLayout.addWidget(&BodyButton, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT(deleteSelf()));
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

json Function::toJson(){
	json DataJSON;
	DataJSON["id"] = FUNC_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["functionName"] = convertToStdString(this->NameEdit.text());
	DataJSON["functionBody"] = convertToStdString(this->FunctionBody->toPlainText());
	return DataJSON;
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
	if (this->NameEdit.text().simplified().isEmpty()){
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
	ExecuteLabel("Execute : ", this),
	LoopLabel("Run only in loop : ", this),
	LoopCheckBox(this)
	{
		this->id = FCTRL_ID;
		this->backgroundcolor = convertToQString(Config::FunctionCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::FunctionCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::FunctionCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		ExecuteLabel.setStyleSheet("border : 0px;");
		LoopLabel.setStyleSheet("border : 0px;");
		FunctionSelect.setStyleSheet("background-color : " + this->foreground + ";");
		LoopCheckBox.setStyleSheet("background-color : " + this->foreground + "; border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&ExecuteLabel, 1, 1);
		this->SelfLayout.addWidget(&FunctionSelect, 1, 2);
		this->SelfLayout.addWidget(&LoopLabel, 2, 1);
		this->SelfLayout.addWidget(&LoopCheckBox, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::FUNCTRLCount++;
	}

void FunctionControl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json FunctionControl::toJson(){
	json DataJSON;
	DataJSON["id"] = FCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetFunctionName"] = convertToStdString(this->FunctionSelect.currentText());
	DataJSON["targetFunctionLoopState"] = this->LoopCheckBox.isChecked();
	return DataJSON;
}

std::string FunctionControl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string TempBuild = convertToStdString(this->FunctionSelect.currentText()) + "()\n";
	if (this->LoopCheckBox.isChecked()){
		this->ParentDrawArea->LoopCodeVector.push_back(TempBuild);
		return "";
		// Return blank string
	} else {
		return TempBuild;
	}
	
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
		this->backgroundcolor = convertToQString(Config::ButtonCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::ButtonCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::ButtonCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		StateSelect.addItem("when_held");
		StateSelect.addItem("when_pressed");
		StateSelect.addItem("when_released");
		DisplayLabel.setFixedSize(180, 20);
		StateSelect.setStyleSheet("background-color : " + this->foreground + ";");
		ButtonSelect.setStyleSheet("background-color : " + this->foreground + ";");
		FunctionSelect.setStyleSheet("background-color : " + this->foreground + ";");
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
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::BTNCTRLCount++;
	}

void ButtonControl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json ButtonControl::toJson(){
	json DataJSON;
	DataJSON["id"] = BTNCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetButtonName"] = convertToStdString(this->ButtonSelect.currentText());
	DataJSON["targetButtonState"] = convertToStdString(this->StateSelect.currentText());
	DataJSON["targetFunctionName"] = convertToStdString(this->FunctionSelect.currentText());
	return DataJSON;
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
		this->backgroundcolor = convertToQString(Config::RGBLEDColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::RGBLEDColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::RGBLEDColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		// Based on Config::legacyMode, change number of pins available
		if (Config::legacyMode == 0){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				RPinSelect.addItem(convertToQString(std::to_string(i)));
				GPinSelect.addItem(convertToQString(std::to_string(i)));
				BPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
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
		this->RPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		GPinLabel.setStyleSheet("border : 0px;");
		this->GPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		BPinLabel.setStyleSheet("border : 0px;");
		this->BPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyRGBLED" + convertToQString(std::to_string(Counters::RGBLEDCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&RPinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->RPinSelect, 1, 2);
		this->SelfLayout.addWidget(&GPinLabel, 2, 1);
		this->SelfLayout.addWidget(&this->GPinSelect, 2, 2);
		this->SelfLayout.addWidget(&BPinLabel, 3, 1);
		this->SelfLayout.addWidget(&this->BPinSelect, 3, 2);
		this->SelfLayout.addWidget(&NameLabel, 4, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 4, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::RGBLEDCount++;
}

void RGBLED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json RGBLED::toJson(){
	json DataJSON;
	DataJSON["id"] = RGBLED_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["RGBLEDPinR"] = std::stoi(convertToStdString(this->RPinSelect.currentText()));
	DataJSON["RGBLEDPinG"] = std::stoi(convertToStdString(this->GPinSelect.currentText()));
	DataJSON["RGBLEDPinB"] = std::stoi(convertToStdString(this->BPinSelect.currentText()));
	DataJSON["RGBLEDName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
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
	if (this->VarnameEdit.text().simplified().isEmpty()){
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
		this->backgroundcolor = convertToQString(Config::RGBLEDCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::RGBLEDCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::RGBLEDCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		NameLabel.setStyleSheet("border : 0px; margin-top : 5px;");
		RLabel.setStyleSheet("border : 0px;");
		GLabel.setStyleSheet("border : 0px;");
		BLabel.setStyleSheet("border : 0px;");
		for (QLineEdit* ValueEdit : {&RValueEdit, &GValueEdit, &BValueEdit}){
			ValueEdit->setFixedWidth(60);
			ValueEdit->setText("0");
			ValueEdit->setStyleSheet("background-color : " + this->foreground + ";");
		}
		for (QSlider* PinSlider : {&RPinSlider, &GPinSlider, &BPinSlider}){
			PinSlider->setFixedWidth(200);
			PinSlider->setMaximum(255);
			PinSlider->setMinimum(0);
			PinSlider->setValue(0);
			PinSlider->setStyleSheet("border : 0px;");
		}
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
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
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

json RGBLEDCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = RGBLEDCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetRGBLEDName"] = convertToStdString(this->RGBLEDSelect.currentText());
	DataJSON["targetRGBLEDRed"] = this->RPinSlider.value();
	DataJSON["targetRGBLEDGreen"] = this->GPinSlider.value();
	DataJSON["targetRGBLEDBlue"] = this->GPinSlider.value();
	return DataJSON;
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
	if (this->RGBLEDSelect.currentText().simplified().isEmpty()){
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
	if (val >= 0 && val <= 255){
		this->RPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Red value provided for " + this->GPIOName);
	}	
}
void RGBLEDCtrl::GTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 255){
		this->GPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Green value provided for " + this->GPIOName);
	}	
}
void RGBLEDCtrl::BTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 255){
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
	std::map<int, std::string> ButtonIconMap;
	ButtonIconMap.insert({1, "debug-start.svg"});
	ButtonIconMap.insert({2, "led-variant-on.svg"});
	ButtonIconMap.insert({3, "wave-square-solid.svg"});
	ButtonIconMap.insert({4, "rgbled.png"});
	ButtonIconMap.insert({5, "buzzer.png"});
	ButtonIconMap.insert({6, "function.svg"});
	ButtonIconMap.insert({7, "button.png"});
	for (int i = 1; i < (int)this->ParentMainWindow->MainWindowDrawArea.ButtonLabelMap.size() + 1; i++){
		GPIOButton* GPIOSelectButton = new GPIOButton(convertToQString("  " + this->ParentMainWindow->MainWindowDrawArea.ButtonLabelMap.at(i)), i,  this, this->ParentMainWindow);
		GPIOSelectButton->setFixedSize(232, 36);
		GPIOSelectButton->setIcon(QIcon(
			convertToQString("static/toolbar/" + ButtonIconMap[i])
		));
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

GPIOButton::GPIOButton(QString label, int GPIOID, GPIOToolBar* mainWindowGPIOToolBar, MainWindow* parentMainWindow)
	: QPushButton(label, mainWindowGPIOToolBar){
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
		this->ParentDrawArea->LoopCodeVector.clear();
		std::ofstream outfile ("script.py"); // Open File 
		for (GPIODevice* GPD : this->ParentDrawArea->GPIOCodeVector)
		{
			outfile << GPD->remoteBuild(); // Write each GPIO Device's build to file
		}
		outfile << "\n";
		// Main Code End
		// Keepalive code start
		outfile << "print(\"The program will now run in 'keepalive' mode.\")\n";
		outfile << "print(\"I.e, you can stop it at any time by pressing CTRL + C\")\n";
		outfile << "print(\"All actions such as Button Actions, Distance Measurements, etc will work.\")\n";
		outfile << "while True:\n";
		outfile << "\ttry:\n";
		for (std::string lcstring : this->ParentDrawArea->LoopCodeVector){
			outfile << "\t\t" << lcstring;
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
		// Could have caused errors with large project files
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
		this->ParentDrawArea->LoopCodeVector.clear();
		std::ofstream outfile ("script.py"); // Open File 
		for (GPIODevice* GPD : this->ParentDrawArea->GPIOCodeVector)
		{
			outfile << GPD->simpleBuild(); // Write each GPIO Device's build to file
		}
		outfile << "\n";
		// Main Code End
		// Keepalive code start
		outfile << "print(\"The program will now run in 'keepalive' mode.\\nI.e, you can stop it at any time by pressing CTRL + C\\nAll actions such as Button Actions, Distance Measurements, etc will work.\")\n";
		outfile << "while True:\n";
		outfile << "\ttry:\n";
		for (std::string lcstring : this->ParentDrawArea->LoopCodeVector){
			outfile << "\t\t" << lcstring;
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

json ProgramStart::toJson(){
	json DataJSON;
	DataJSON["id"] = PSTART_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	return DataJSON;
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
