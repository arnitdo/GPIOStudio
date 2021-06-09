#ifndef GPIOSTUDIO_MAIN_HPP
#define GPIOSTUDIO_MAIN_HPP
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
#include <QFormLayout>
#include <QLineEdit>
#include <QComboBox>
#include <QFileDialog>
#include <QTextBrowser>
#include <QSlider>
#include <QCheckBox>
#include <QMessageBox>

#include <vector>
#include <string>
#include <iostream>
#include <fstream>
#include <map>
#include <sstream>

#include "thirdparty/nlohmann/json.hpp"
using json = nlohmann::json;

class MainWindow;
class DrawArea;
class GPIOButton;
class GPIOToolBar;
class GPIODevice;
class ProgramStart;
class LED;
class PWMLED;
class RGBLED;
class Buzzer;
class Button;
class DistanceSensor;
class MotionSensor;
class LineSensor;
class LightSensor;
class Sleep;
class Function;
class LEDCtrl;
class PWMLEDCtrl;
class RGBLEDCtrl;
class BuzzerCtrl;
class FunctionControl;
class ButtonControl;
class SenseHat;

/*  
  ____ ____ ___ ___ _____ ___   ___  _     ____    _    ____
 / ___|  _ |_ _/ _ |_   _/ _ \ / _ \| |   | __ )  / \  |  _ \
| |  _| |_) | | | | || || | | | | | | |   |  _ \ / _ \ | |_) |
| |_| |  __/| | |_| || || |_| | |_| | |___| |_) / ___ \|  _ <
 \____|_|  |___\___/ |_| \___/ \___/|_____|____/_/   \_|_| \_\ */
class GPIOToolBar : public QWidget{
	Q_OBJECT;
	public:
		GPIOToolBar(QWidget* parent, MainWindow* parentMainWindow);
		MainWindow *ParentMainWindow;
		DrawArea* MainWindowDrawArea;
		QGridLayout GPIOToolBarLayout;
};

/* 
 ____  ____     ___        _    _    ____  _____    _
|  _ \|  _ \   / \ \      /    / \  |  _ \| ____|  / \
| | | | |_) | / _ \ \ /\ / /  / _ \ | |_) |  _|   / _ \
| |_| |  _ < / ___ \ V  V /  / ___ \|  _ <| |___ / ___ \
|____/|_| \_/_/   \_\_/\_/  /_/   \_|_| \_|_____/_/   \_\ */

class DrawArea: public QWidget{
	Q_OBJECT;
	public:
		// Functions
		DrawArea(MainWindow* parent = nullptr);
		virtual void mousePressEvent(QMouseEvent *event);
		virtual void paintEvent(QPaintEvent* event);
		void createGPIODevice(json& GPIOData);
		void RefreshSelects();
		void resetSelf();
		void loadJson(QString fname);
		void saveToJson();
		void Undo();
		void Redo();
		bool checkForPStart();
		bool checkForSenseHat();
		// Members
		ProgramStart* ProgStart;
		QPoint LastPoint;
		QPoint CurrentPoint;
		MainWindow* ParentMainWindow;
		std::vector<json> ActionJSONBuffer;
		std::vector<std::pair<QPoint, QPoint>> Lines;
		std::vector<GPIODevice*> GPIOCodeVector;
		std::vector<std::string> LoopCodeVector;
		std::vector<LED*> LEDVec;
		std::vector<PWMLED*> PWMLEDVec;
		std::vector<Buzzer*> BUZVec;
		std::vector<LEDCtrl*> LEDCTRLVec;
		std::vector<PWMLEDCtrl*> PWMLEDCTRLVec;
		std::vector<BuzzerCtrl*> BUZCTRLVec;
		std::vector<Button*> BTNVec;
		std::vector<Function*> FUNCVec;
		std::vector<FunctionControl*> FUNCTRLVec;
		std::vector<ButtonControl*>BTNCTRLVec;
		std::vector<RGBLED*> RGBLEDVec;
		std::vector<RGBLEDCtrl*> RGBLEDCTRLVec;
		std::map<int, std::string> ButtonLabelMap;
		int activeGPIO;
		bool isNew = true;
		bool NWMode = false;
	public slots:
		void OnGPIODeviceSignal(int GPIOID);
};

/* 
 __  __    _    ___ _   ___        _____ _   _ ____   _____        __
|  \/  |  / \  |_ _| \ | \ \      / |_ _| \ | |  _ \ / _ \ \      / /
| |\/| | / _ \  | ||  \|  \ \ /\ / / | ||  \| | | | | | | \ \ /\ / /
| |  | |/ ___ \ | || |\    \ V  V /  | || |\  | |_| | |_| |\ V  V /
|_|  |_/_/   \_|___|_| \_   \_/\_/  |___|_| \_|____/ \___/  \_/\_/ */
class MainWindow : public QWidget{
	Q_OBJECT;
	public:
		// Functions
		MainWindow(QApplication* parentApplication);
		~MainWindow();
		void log(std::string value);
		void warn(std::string value);
		void debug(std::string value);
		void err(std::string value);
		//Members
		std::string RemoteIP;
		QApplication* ParentApp;
		QWidget RemoteWindow;
		QLineEdit RaspiIPEdit;
		QPushButton RemoteRunButton;
		QPushButton RWHideButton;
		QWidget HelpWindow;
		QWidget AboutWindow;
		QTextBrowser AboutWindowContent;
		DrawArea MainWindowDrawArea;
		QTextEdit MainWindowConsole;
		QScrollArea MainWindowScrollArea;
		QScrollArea MainWindowGPIOScrollArea;
		GPIOToolBar MainWindowGPIOToolBar;
		QPushButton MainWindowUndoButton;
		QPushButton MainWindowRedoButton;
		QPushButton MainWindowClearButton;
		QPushButton MainWindowRefreshButton;
		QPushButton MainWindowBuildButton;
		QPushButton MainWindowRemoteButton;
		QPushButton MainWindowLoadButton;
		QPushButton MainWindowSaveButton;
		QPushButton MainWindowAboutButton;
		QPushButton MainWindowQuitButton;
		QGridLayout MainWindowLayout;
	public slots:
		// Most of these slots are nothing but invocations of the respective DrawArea function
		void DrawAreaUndo();
		void DrawAreaRedo();
		void resetDrawArea();
		void RefreshDrawSelects();
		void showRemoteWindow();
		void hideRemoteWindow();
		void runRemote();
		void OpenJSON();
		void SaveToJSON();
		void ShowHelpWindow();
		void ShowAboutWindow();
		void QuitApp();
	signals:
		void deleteGPIO();
};

/* 
  ____ ____ ___ ___
 / ___|  _ |_ _/ _ \
| |  _| |_) | | | | |
| |_| |  __/| | |_| |
 \____|_|  |___\___/ */

class GPIODevice : public QWidget{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		virtual void paintEvent(QPaintEvent* event);
		GPIODevice(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		int id;
		QString backgroundcolor, foreground, textcolor;
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		int XCoord, YCoord;
		std::string GPIOName;
	public slots:
		virtual void deleteSelf();
};

class LED : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		LED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		QString backgroundcolor = "#00ffff";
		QGridLayout SelfLayout;
		QComboBox PinSelect;
		QLineEdit VarnameEdit;
		QLabel DisplayLabel;
		QLabel PinLabel;
		QLabel NameLabel;
	public slots:
		virtual void deleteSelf();
};

class PWMLED : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		PWMLED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		QString backgroundcolor = "#69b5e2";
		QGridLayout SelfLayout;
		QComboBox PinSelect;
		QLineEdit VarnameEdit;
		QLabel DisplayLabel;
		QLabel PinLabel;
		QLabel NameLabel;
	public slots:
		virtual void deleteSelf();
};

class Buzzer : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		Buzzer(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		QString backgroundcolor = "#98fb98";
		QGridLayout SelfLayout;
		QComboBox PinSelect;
		QLineEdit VarnameEdit;
		QLabel DisplayLabel;
		QLabel PinLabel;
		QLabel NameLabel;
	public slots:
		virtual void deleteSelf();
};

class LEDCtrl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		LEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#FFB473";
		QGridLayout SelfLayout;
		QComboBox LEDSelect;
		QComboBox StateSelect;
		QLabel DisplayLabel;
		QLabel LEDLabel;
		QLabel StateLabel;
	public slots:
		virtual void deleteSelf();
};

class PWMLEDCtrl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		PWMLEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#abe587";
		QGridLayout SelfLayout;
		QComboBox PWMLEDSelect;
		QLineEdit ValueEdit;
		QLabel DisplayLabel;
		QLabel PWMLEDLabel;
		QLabel ValueLabel;
	public slots:
		virtual void deleteSelf();
};

class BuzzerCtrl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		BuzzerCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#CACDFF";
		QGridLayout SelfLayout;
		QComboBox BuzzerSelect;
		QComboBox StateSelect;
		QLabel DisplayLabel;
		QLabel BuzzerLabel;
		QLabel StateLabel;
	public slots:
		virtual void deleteSelf();
};

class Sleep : public GPIODevice{
		Q_OBJECT;
	public:
		// Functions
		Sleep(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#FEDF00";
		QGridLayout SelfLayout;
		QLineEdit DurationEdit;
		QLabel DisplayLabel;
		QLabel DurationLabel;
	public slots:
		virtual void deleteSelf();
};

class Button : public GPIODevice{
		Q_OBJECT;
	public:
		// Functions
		Button(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#AADF0A";
		QGridLayout SelfLayout;
		QComboBox PinSelect;
		QLineEdit VarnameEdit;
		QLabel DisplayLabel;
		QLabel PinLabel;
		QLabel NameLabel;
	public slots:
		virtual void deleteSelf();
};

class Function : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		Function(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#00cc99";
		QWidget* FunctionBodyWindow;
		QTextEdit* FunctionBody;
		QPushButton* CloseBodyButton;
		QGridLayout SelfLayout;
		QLabel DisplayLabel;
		QLabel NameLabel;
		QLineEdit NameEdit;
		QLabel BodyLabel;
		QPushButton BodyButton;
		std::stringstream FunctionText;
	public slots:
		void showBodyWindow();
		void hideBodyWindow();
		virtual void deleteSelf();
		
};

class DistanceSensor : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		DistanceSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#bccc38";
		QGridLayout SelfLayout;
		QLabel DisplayLabel;
		QLabel TrigPinLabel;
		QLabel EchoPinLabel;
		QComboBox TrigPinSelect;
		QComboBox EchoPinSelect;
		QLabel NameLabel;
		QLineEdit VarnameEdit;
	public slots:
		virtual void deleteSelf();
};

class LightSensor : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		LightSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#0073db";
		QGridLayout SelfLayout;
		QLabel DisplayLabel;
		QLabel PinLabel;
		QComboBox PinSelect;
		QLabel NameLabel;
		QLineEdit VarnameEdit;
	public slots:
		virtual void deleteSelf();
};

class MotionSensor : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		MotionSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#a2d0d7";
		QGridLayout SelfLayout;
		QLabel DisplayLabel;
		QLabel PinLabel;
		QComboBox PinSelect;
		QLabel NameLabel;
		QLineEdit VarnameEdit;
	public slots:
		virtual void deleteSelf();
};

class LineSensor : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		LineSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#98a20b";
		QGridLayout SelfLayout;
		QLabel DisplayLabel;
		QLabel PinLabel;
		QComboBox PinSelect;
		QLabel NameLabel;
		QLineEdit VarnameEdit;
	public slots:
		virtual void deleteSelf();
};

class FunctionControl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		FunctionControl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#FEDBF8";
		QGridLayout SelfLayout;
		QComboBox FunctionSelect;
		QLabel DisplayLabel;
		QLabel ExecuteLabel;
		QLabel LoopLabel;
		QCheckBox LoopCheckBox;
	public slots:
		virtual void deleteSelf();
};

class ButtonControl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		ButtonControl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#8DA3F3";
		QGridLayout SelfLayout;
		QComboBox ButtonSelect;
		QComboBox StateSelect;
		QComboBox FunctionSelect;
		QLabel DisplayLabel;
		QLabel ButtonLabel;
		QLabel StateLabel;
		QLabel ExecuteLabel;
	public slots:
		virtual void deleteSelf();
};

class RGBLED : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		RGBLED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		QString backgroundcolor = "#f7957a";
		QGridLayout SelfLayout;
		QComboBox RPinSelect;
		QComboBox GPinSelect;
		QComboBox BPinSelect;
		QLineEdit VarnameEdit;
		QLabel DisplayLabel;
		QLabel RPinLabel;
		QLabel GPinLabel;
		QLabel BPinLabel;
		QLabel NameLabel;
	public slots:
		virtual void deleteSelf();
};

class RGBLEDCtrl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		RGBLEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		QString backgroundcolor = "#9acd32";
		QGridLayout SelfLayout;
		QSlider RPinSlider;
		QSlider GPinSlider;
		QSlider BPinSlider;
		QLabel NameLabel;
		QLabel DisplayLabel;
		QLabel RLabel;
		QLabel GLabel;
		QLabel BLabel;
		QComboBox RGBLEDSelect;
		QLineEdit RValueEdit;
		QLineEdit GValueEdit;
		QLineEdit BValueEdit;
	public slots:
		void RSliderValueUpdated(int value);
		void GSliderValueUpdated(int value);
		void BSliderValueUpdated(int value);
		void RTextValueUpdated(QString text);
		void GTextValueUpdated(QString text);
		void BTextValueUpdated(QString text);
		virtual void deleteSelf();	
};


class SenseHat : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		SenseHat(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson();
		// Members
		QString backgroundcolor = "#8DA3F3";
		QGridLayout SelfLayout;
		QLabel DisplayLabel;
	public slots:
		virtual void deleteSelf();
};
/* 
 ____  ____   ___   ____ ____     _    __  __ ____ _____  _    ____ _____
|  _ \|  _ \ / _ \ / ___|  _ \   / \  |  \/  / ___|_   _|/ \  |  _ |_   _|
| |_) | |_) | | | | |  _| |_) | / _ \ | |\/| \___ \ | | / _ \ | |_) || |
|  __/|  _ <| |_| | |_| |  _ < / ___ \| |  | |___) || |/ ___ \|  _ < | |
|_|   |_| \_\\___/ \____|_| \_/_/   \_|_|  |_|____/ |_/_/   \_|_| \_\|_|
 */

class ProgramStart : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string simpleBuild();
		virtual std::string remoteBuild();
		virtual bool validateInput();
		virtual json toJson(); // Self validation, will always return true
		bool validateCode(); // Full code validation
		ProgramStart(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		QLabel DisplayLabel;
		QGridLayout SelfLayout;
		MainWindow* ParentMainWindow;
		DrawArea* ParentDrawArea;
		QString backgroundcolor = "#aaaaaa";
	signals:
		void buildCompleted();
	public slots:
		void TriggerSimpleBuild();
		void TriggerRemoteBuild();
		virtual void deleteSelf();
};

/*  
  ____ ____ ___ ___  ____  _   _ _____ _____ ___  _   _
 / ___|  _ |_ _/ _ \| __ )| | | |_   _|_   _/ _ \| \ | |
| |  _| |_) | | | | |  _ \| | | | | |   | || | | |  \| |
| |_| |  __/| | |_| | |_) | |_| | | |   | || |_| | |\  |
 \____|_|  |___\___/|____/ \___/  |_|   |_| \___/|_| \_| */

class GPIOButton : public QPushButton{
	Q_OBJECT;
	public:
		GPIOButton(QString label,int GPIOID, GPIOToolBar* mainWindowGPIOToolBar, MainWindow* parentMainWindow);
		MainWindow* ParentMainWindow;
		GPIOToolBar* ParentGPIOToolBar;
		int GPIOID;
	signals:
		void GPIOButtonPressed(int GPIOID);
	public slots:
		void SelfPressed();
};

#endif