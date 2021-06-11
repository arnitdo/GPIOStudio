#include "main.hpp"
#include "config.cpp"

MotionSensor::MotionSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	PinSelect(this),
	NameLabel("Name : ", this),
	VarnameEdit(this){
		this->id = MOTION_ID;
		this->backgroundcolor = convertToQString(Config::MotionSensorColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::MotionSensorColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::MotionSensorColor.at("text").get<std::string>());
		this->ParentDrawArea = parent; 
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		PinLabel.setStyleSheet("border : 0px;");
		NameLabel.setStyleSheet("border : 0px;");
		DisplayLabel.setFixedHeight(20);
		PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		VarnameEdit.setText("MyMotionSensor" + convertToQString(std::to_string(Counters::MOTIONCount)));
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 2, 1);
		this->SelfLayout.addWidget(&PinSelect, 2, 2);
		this->SelfLayout.addWidget(&NameLabel, 3, 1);
		this->SelfLayout.addWidget(&VarnameEdit, 3, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::MOTIONCount++;
	}

void MotionSensor::deleteSelf(){
	delete this;
}

json MotionSensor::toJson(){
	json DataJSON;
	DataJSON["id"] = MOTION_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["motionPin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["motionName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string MotionSensor::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Motion Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(" + convertToStdString(this->VarnameEdit.text()) + ".value))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);	
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.MotionSensor(" + convertToStdString(this->PinSelect.currentText()) + ", pin_factory=__remote_pin_factory)\n";
}

std::string MotionSensor::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Motion Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(" + convertToStdString(this->VarnameEdit.text()) + ".value))\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.MotionSensor(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool MotionSensor::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name provided for " + this->GPIOName);
		return false;
	}
	return true;
}