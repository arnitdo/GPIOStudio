#include "config.cpp"

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
	MainWindowUndoButton(this),
	MainWindowRedoButton(this),
	MainWindowClearButton(this),
	MainWindowRefreshButton(this),
	MainWindowBuildButton(this),
	MainWindowRemoteButton(this),
	MainWindowLoadButton(this),
	MainWindowSaveButton(this),
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

	// Undo Button
	MainWindowUndoButton.setFixedHeight(36);
	MainWindowUndoButton.setText(" Undo");
	MainWindowUndoButton.setIcon(QIcon("static/menubar/undo.svg"));
	MainWindowUndoButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Z));
	MainWindowLayout.addWidget(&MainWindowUndoButton, 0, 0);
	
	// Redo Button
	MainWindowRedoButton.setFixedHeight(36);
	MainWindowRedoButton.setText(" Redo");
	MainWindowRedoButton.setIcon(QIcon("static/menubar/redo.svg"));
	MainWindowRedoButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_Y));
	MainWindowLayout.addWidget(&MainWindowRedoButton, 0, 1);

	// Clear All Button
	MainWindowClearButton.setFixedHeight(36);
	MainWindowClearButton.setText(" Clear All");
	MainWindowClearButton.setIcon(QIcon("static/menubar/clear.svg"));
	MainWindowClearButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_C));
	MainWindowLayout.addWidget(&MainWindowClearButton, 0, 2);

	// Refresh Button
	MainWindowRefreshButton.setFixedHeight(36);
	MainWindowRefreshButton.setText(" Refresh");
	MainWindowRefreshButton.setIcon(QIcon("static/menubar/refresh.svg"));
	MainWindowRefreshButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_F));
	MainWindowLayout.addWidget(&MainWindowRefreshButton, 0, 3);

	// Build Button
	MainWindowBuildButton.setFixedHeight(36);
	MainWindowBuildButton.setText(" Build");
	MainWindowBuildButton.setIcon(QIcon("static/menubar/hammer.svg"));
	MainWindowBuildButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_B));
	MainWindowLayout.addWidget(&MainWindowBuildButton, 0, 4);

	// Remote Run Button
	MainWindowRemoteButton.setFixedHeight(36);
	MainWindowRemoteButton.setText(" Run Remote");
	MainWindowRemoteButton.setIcon(QIcon("static/menubar/remote.svg"));
	MainWindowRemoteButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_R));
	MainWindowLayout.addWidget(&MainWindowRemoteButton, 0, 5);

	// Load Button
	MainWindowLoadButton.setFixedHeight(36);
	MainWindowLoadButton.setText(" Load File");
	MainWindowLoadButton.setIcon(QIcon("static/menubar/package.svg"));
	MainWindowLoadButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_L));
	MainWindowLayout.addWidget(&MainWindowLoadButton, 0, 6);

	// Save Button
	MainWindowSaveButton.setFixedHeight(36);
	MainWindowSaveButton.setText(" Save File");
	MainWindowSaveButton.setIcon(QIcon("static/menubar/save.svg"));
	MainWindowSaveButton.setShortcut(QKeySequence(Qt::CTRL | Qt::Key_S));
	MainWindowLayout.addWidget(&MainWindowSaveButton, 0, 7);

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

	this->setAcceptDrops(true);
	this->MainWindowGPIOScrollArea.setAcceptDrops(true);
	this->MainWindowDrawArea.setAcceptDrops(true);

	// Connecting SIGNALS and SLOTS
	QObject::connect(&MainWindowUndoButton, SIGNAL (clicked()), this, SLOT (DrawAreaUndo()));
	QObject::connect(&MainWindowRedoButton, SIGNAL (clicked()), this, SLOT (DrawAreaRedo()));
	QObject::connect(&MainWindowClearButton, SIGNAL (clicked()), this, SLOT (resetDrawArea()));
	QObject::connect(&MainWindowRefreshButton, SIGNAL (clicked()), this, SLOT (RefreshDrawSelects()));
	QObject::connect(&MainWindowRemoteButton, SIGNAL (clicked()), this, SLOT (showRemoteWindow()));
	QObject::connect(&RWHideButton, SIGNAL (clicked()), this, SLOT (hideRemoteWindow()));
	QObject::connect(&MainWindowLoadButton, SIGNAL (clicked()), this, SLOT (OpenJSON()));
	QObject::connect(&MainWindowSaveButton, SIGNAL (clicked()), this, SLOT (SaveToJSON()));
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

void MainWindow::DrawAreaUndo(){
	this->MainWindowDrawArea.Undo();
}

void MainWindow::DrawAreaRedo(){
	this->MainWindowDrawArea.Redo();
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
	QString fname = QFileDialog::getOpenFileName(this, "Open GPIO JSON File", "", "JSON (*.json)");
	if (!fname.simplified().isEmpty() && !fname.endsWith("config.json", Qt::CaseSensitive)){
		this->MainWindowDrawArea.loadJson(fname);
	} else {
		if (fname.endsWith("config.json")){
			this->warn("Config files cannot be loaded!");
		}
	}
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