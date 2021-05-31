#include "final.cpp"

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
