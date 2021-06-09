#include "main.hpp"
#include "config.cpp"

DistanceSensor::DistanceSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	TrigPinLabel("Trigger Pin : ", this),
	EchoPinLabel("Echo Pin : ", this),
	TrigPinSelect(this),
	EchoPinSelect(this),
	NameLabel("Name : ", this),
	VarnameEdit(this){
		this->id = DISTS_ID;
		this->backgroundcolor = convertToQString(Config::DistanceSensorColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::DistanceSensorColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::DistanceSensorColor.at("text").get<std::string>());
		this->ParentDrawArea = parent; 
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				TrigPinSelect.addItem(convertToQString(std::to_string(i)));
				EchoPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				TrigPinSelect.addItem(convertToQString(std::to_string(i)));
				EchoPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				TrigPinSelect.addItem(convertToQString(std::to_string(i)));
				EchoPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		TrigPinLabel.setStyleSheet("border : 0px;");
		EchoPinLabel.setStyleSheet("border : 0px;");
		NameLabel.setStyleSheet("border : 0px;");
		TrigPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		EchoPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setText("MyDistanceSensor" + convertToQString(std::to_string(Counters::DISTSCount)));
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&EchoPinLabel, 2, 1);
		this->SelfLayout.addWidget(&EchoPinSelect, 2, 2);
		this->SelfLayout.addWidget(&TrigPinLabel, 3, 1);
		this->SelfLayout.addWidget(&TrigPinSelect, 3, 2);
		this->SelfLayout.addWidget(&NameLabel, 4, 1);
		this->SelfLayout.addWidget(&VarnameEdit, 4, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::DISTSCount++;
	}

void DistanceSensor::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json DistanceSensor::toJson(){
	json DataJSON;
	DataJSON["id"] = DISTS_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["distanceEchoPin"] = std::stoi(convertToStdString(this->EchoPinSelect.currentText()));
	DataJSON["distanceTrigPin"] = std::stoi(convertToStdString(this->TrigPinSelect.currentText()));
	DataJSON["distanceName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string DistanceSensor::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Distance recorded by " + convertToStdString(this->VarnameEdit.text()) + " in centimeters : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".distance * 100)))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);	
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.DistanceSensor(" + convertToStdString(this->EchoPinSelect.currentText()) +
	", " + convertToStdString(this->TrigPinSelect.currentText()) + ", pin_factory=__remote_pin_factory)\n";
}

std::string DistanceSensor::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Distance recorded by " + convertToStdString(this->VarnameEdit.text()) + " in centimeters : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".distance * 100)))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.DistanceSensor(" + convertToStdString(this->EchoPinSelect.currentText()) +
	", " + convertToStdString(this->TrigPinSelect.currentText()) + ")\n";
}

bool DistanceSensor::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name provided for " + this->GPIOName);
		return false;
	}
	if (this->EchoPinSelect.currentText() == this->TrigPinSelect.currentText()){
		this->ParentMainWindow->err("Duplicate Pins provided for " + this->GPIOName);
		return false;
	}
	return true;
}