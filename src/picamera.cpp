#include "main.hpp"
#include "config.cpp"

PiCamera::PiCamera(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X,Y, name),
	SelfLayout(this),
	DisplayLabel(this),
	FileNameLabel("Enter file to save : ", this),
	FileNameEdit(this){
		this->id = PICAMERA_ID;
		this->backgroundcolor = convertToQString(Config::PiCameraColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::PiCameraColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::PiCameraColor.at("text").get<std::string>());
		DisplayLabel.setText(convertToQString(name));
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		FileNameLabel.setStyleSheet("border : 0px;");
		FileNameEdit.setStyleSheet("background-color : #ffffff;");
		FileNameEdit.setMaximumHeight(25);
		SelfLayout.addWidget(&DisplayLabel, 1, 1);
		SelfLayout.addWidget(&FileNameLabel, 2, 1);
		SelfLayout.addWidget(&FileNameEdit, 3, 1);
		this->ParentMainWindow->warn("Pi Cameras are not supported over Remote GPIO!");
		this->ParentMainWindow->warn("Remote Run is disabled.");
		this->ParentMainWindow->MainWindowRemoteButton.setDisabled(true);
}

void PiCamera::deleteSelf(){
	this->ParentMainWindow->MainWindowRemoteButton.setDisabled(false);
	delete this;
}

json PiCamera::toJson(){
	json DataJSON;
	DataJSON["id"] = PICAMERA_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["fileName"] = convertToStdString(this->FileNameEdit.text());
	return DataJSON;
}

bool PiCamera::validateInput(){
	return !(this->FileNameEdit.text().simplified().isEmpty());
}

std::string PiCamera::remoteBuild(){
	return  "__camera = picamera.PiCamera()\n"
	"__camera.start_preview()\n"
	"time.sleep(2)\n"
	"__camera.capture(\"" + convertToStdString(this->FileNameEdit.text().simplified()) + "\")\n"
	"__camera.stop_preview()\n"
	"__camera.close()\n";
}

std::string PiCamera::simpleBuild(){
	return this->remoteBuild();
}