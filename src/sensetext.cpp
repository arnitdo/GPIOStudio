#include "main.hpp"
#include "config.cpp"

SenseText::SenseText(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X,Y, name),
	SelfLayout(this),
	DisplayLabel(this),
	TextLabel("Enter text to display : ", this),
	TextEdit(this){
		this->id = SENSE_TEXT_ID;
		this->backgroundcolor = convertToQString(Config::SenseTextColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::SenseTextColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::SenseTextColor.at("text").get<std::string>());
		DisplayLabel.setText(convertToQString(name));
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		TextLabel.setStyleSheet("border : 0px;");
		TextEdit.setStyleSheet("background-color : #ffffff;");
		TextEdit.setMaximumHeight(25);
		SelfLayout.addWidget(&DisplayLabel, 1, 1);
		SelfLayout.addWidget(&TextLabel, 2, 1);
		SelfLayout.addWidget(&TextEdit, 3, 1);
		QObject::connect(&(this->ParentMainWindow->RemoteRunButton), SIGNAL (clicked()), this, SLOT (TriggerRemoteBuild()));
		QObject::connect(&(this->ParentMainWindow->MainWindowBuildButton), SIGNAL (clicked()), this, SLOT (TriggerSimpleBuild()));
		this->ParentMainWindow->MainWindowRemoteButton.setDisabled(true);
}

void SenseText::deleteSelf(){
	this->ParentMainWindow->MainWindowRemoteButton.setDisabled(false);
	delete this;
}

json SenseText::toJson(){
	json DataJSON;
	DataJSON["id"] = SENSE_TEXT_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["displayText"] = convertToStdString(this->TextEdit.text());
	return DataJSON;
}

bool SenseText::validateInput(){
	return true;
}

std::string SenseText::remoteBuild(){
	return  "__sense_hat.show_message(\"" + convertToStdString(this->TextEdit.text()) + "\")\n";
}

std::string SenseText::simpleBuild(){
	return this->remoteBuild();
}