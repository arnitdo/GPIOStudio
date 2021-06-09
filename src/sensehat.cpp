#include "main.hpp"
#include "config.cpp"

SenseHat::SenseHat(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X,Y, name),
	SelfLayout(this),
	DisplayLabel(this){
		this->id = SENSE_ID;
		DisplayLabel.setText(convertToQString(name));
		DisplayLabel.setFixedWidth(200);
		this->ParentDrawArea = parent;
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		QObject::connect(&(this->ParentMainWindow->RemoteRunButton), SIGNAL (clicked()), this, SLOT (TriggerRemoteBuild()));
		QObject::connect(&(this->ParentMainWindow->MainWindowBuildButton), SIGNAL (clicked()), this, SLOT (TriggerSimpleBuild()));
		this->ParentMainWindow->MainWindowRemoteButton.setDisabled(true);
		// Sense HATs are not supported over remote GPIO.
		this->ParentMainWindow->warn("Sense HATs are not supported over Remote GPIO!");
		this->ParentMainWindow->warn("Remote Run is disabled.");
}

void SenseHat::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	this->ParentMainWindow->MainWindowRemoteButton.setDisabled(false);
	delete this;
}

json SenseHat::toJson(){
	json DataJSON;
	DataJSON["id"] = SENSE_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	return DataJSON;
}

bool SenseHat::validateInput(){
	return true;
}

std::string SenseHat::remoteBuild(){
	return  "__sense_hat = sense_hat.SenseHat()\n";
}

std::string SenseHat::simpleBuild(){
	return this->remoteBuild();
}