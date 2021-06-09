#include "button.cpp"
#include "buttonctrl.cpp"
#include "buzzer.cpp"
#include "buzzerctrl.cpp"
#include "config.cpp"
#include "distancesensor.cpp"
#include "drawarea.cpp"
#include "function.cpp"
#include "functionctrl.cpp"
#include "gpiodevice.cpp"
#include "led.cpp"
#include "ledctrl.cpp"
#include "lightsensor.cpp"
#include "linesensor.cpp"
#include "mainwindow.cpp"
#include "motionsensor.cpp"
#include "programstart.cpp"
#include "pwmled.cpp"
#include "pwmledctrl.cpp"
#include "rgbled.cpp"
#include "rgbledctrl.cpp"
#include "sleep.cpp"
#include "toolbar.cpp"
#include "sensehat.cpp"

int main(int argc, char** argv){
	QApplication app (argc, argv);
	MainWindow AppWindow (&app);
	AppWindow.show();
	Config::LoadConfig(&AppWindow);
	if (argc == 2){
		QString ArgFileName = argv[1];
		if (ArgFileName.endsWith(".json") && !ArgFileName.endsWith("config.json")){
			AppWindow.MainWindowDrawArea.loadJson(ArgFileName);
		}	
	}
	return app.exec();
}
