#include "main.hpp"
#include "config.cpp"

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