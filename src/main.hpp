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

#include <vector>
#include <cstdlib>

class MainWindow;
class DrawArea;
class GPIODevice;
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
		void LinkGPIO();
		// Members
		ProgramStart* ProgStart;
		QPoint LastPoint;
		QPoint CurrentPoint;
		MainWindow* ParentMainWindow;
		std::vector<std::pair<QPoint, QPoint>> Lines;
		std::vector<GPIODevice*> GPIOCodeVector;
		int activeGPIO;
		bool isNew = true;
		bool NWMode = false;
	public slots:
		void OnGPIODeviceSignal(int GPIOID);
		void resetSelf();
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
		MainWindow();
		~MainWindow();
		void log(std::string value);
		void warn(std::string value);
		void debug(std::string value);
		void err(std::string value);
		//Members
		DrawArea MainWindowDrawArea;
		QTextEdit MainWindowConsole;
		QScrollArea MainWindowScrollArea;
		QScrollArea MainWindowGPIOScrollArea;
		GPIOToolBar MainWindowGPIOToolBar;
		QPushButton MainWindowClearButton;
		QPushButton MainWindowBuildButton;
		QPushButton MainWindowBRComboButton;
	public slots:
		void resetDrawArea();
		void buildAndRun();
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
		GPIODevice(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name, bool log);
		// Members
		DrawArea* ParentDrawArea;
		MainWindow* ParentMainWindow;
		QLabel DisplayText;
		int XCoord, YCoord;
		std::string GPIOName;
		std::string Color = "#ffff66";
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
		MainWindow* ParentMainWindow;
		DrawArea* ParentDrawArea;
		GPIODevice* Next = nullptr;
		std::string Color = "#aaaaaa";
	public slots:
		void TriggerBuild();
	public slots:
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
