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

#include <vector>
#include <cstdlib>
#include <string>
#include <iostream>
#include <fstream>
#include <map>

class MainWindow;
class DrawArea;
class GPIODevice;
class Buzzer;
class LED;
class LEDCtrl;
class BuzzerCtrl;
class Sleep;
class MenuBar;
class GPIOButton;
class GPIOToolBar;
class ProgramStart;

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
		// Members
		ProgramStart* ProgStart;
		QPoint LastPoint;
		QPoint CurrentPoint;
		MainWindow* ParentMainWindow;
		std::vector<std::pair<QPoint, QPoint>> Lines;
		std::vector<GPIODevice*> GPIOCodeVector;
		std::vector<LED*> LEDVec;
		std::vector<Buzzer*> BUZVec;
		std::vector<LEDCtrl*> LEDCTRLVec;
		std::vector<BuzzerCtrl*> BUZCTRLVec;
		std::map<int, std::string> ButtonLabelMap;
		int activeGPIO;
		bool isNew = true;
		bool NWMode = false;
	public slots:
		void OnGPIODeviceSignal(int GPIOID);
		void resetSelf();
		void RefreshSelects();
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
		QApplication* ParentApp;
		DrawArea MainWindowDrawArea;
		QTextEdit MainWindowConsole;
		QScrollArea MainWindowScrollArea;
		QScrollArea MainWindowGPIOScrollArea;
		GPIOToolBar MainWindowGPIOToolBar;
		QPushButton MainWindowClearButton;
		QPushButton MainWindowBuildButton;
		QPushButton MainWindowBRComboButton;
		QPushButton MainWindowQuitButton;
		QPushButton MainWindowRefreshButton;
	public slots:
		void resetDrawArea();
		void buildAndRun();
		void QuitApp();
		void RefreshDrawSelects();
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
		virtual std::string build(); // IMPORTANT
		virtual void paintEvent(QPaintEvent* event);
		GPIODevice(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		int XCoord, YCoord;
		std::string GPIOName;
		std::string Color = "#ffff66";
	public slots:
		virtual void deleteSelf();
};

class LED : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string build(); // IMPORTANT
		// virtual void paintEvent(QPaintEvent* event);
		LED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		QGridLayout SelfLayout;
		QComboBox PinSelect;
		QLineEdit VarnameEdit;
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		int XCoord, YCoord;
		std::string GPIOName;
		std::string Color = "#00ffff";
	public slots:
		virtual void deleteSelf();
};

class Buzzer : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		virtual std::string build(); // IMPORTANT
		// virtual void paintEvent(QPaintEvent* event);
		Buzzer(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		// Members
		std::string Color = "#98fb98";
		QGridLayout SelfLayout;
		QComboBox PinSelect;
		QLineEdit VarnameEdit;
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		int XCoord, YCoord;
		std::string GPIOName;
	public slots:
		virtual void deleteSelf();
};

class LEDCtrl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		LEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string build();
		// Members
		std::string Color = "#FFB473";
		QGridLayout SelfLayout;
		QComboBox LEDSelect;
		QComboBox StateSelect;
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		int XCoord, YCoord;
		std::string GPIOName;

	public slots:
		virtual void deleteSelf();
};

class BuzzerCtrl : public GPIODevice{
	Q_OBJECT;
	public:
		// Functions
		BuzzerCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string build();
		// Members
		std::string Color = "#CACDFF";
		QGridLayout SelfLayout;
		QComboBox BuzzerSelect;
		QComboBox StateSelect;
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		int XCoord, YCoord;
		std::string GPIOName;

	public slots:
		virtual void deleteSelf();
};

class Sleep : public GPIODevice{
		Q_OBJECT;
	public:
		// Functions
		Sleep(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name);
		virtual std::string build();
		// Members
		std::string Color = "#FEDF00";
		QGridLayout SelfLayout;
		QLineEdit DurationEdit;
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		int XCoord, YCoord;
		std::string GPIOName;
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
		virtual std::string build(); // IMPORTANT
		ProgramStart(DrawArea* parent, MainWindow* parentMainWindow);
		// Members
		QLabel DisplayText;
		QGridLayout SelfLayout;
		MainWindow* ParentMainWindow;
		DrawArea* ParentDrawArea;
		std::string Color = "#aaaaaa";
	public slots:
		void TriggerBuild();
		virtual void deleteSelf(){}; // Cannot be deleted.
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
