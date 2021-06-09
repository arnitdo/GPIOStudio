#include "main.hpp"
#include "config.cpp"

LineSensor::LineSensor(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	PinSelect(this),
	NameLabel("Name : ", this),
	VarnameEdit(this){
		this->id = LINES_ID;
		this->backgroundcolor = convertToQString(Config::LineSensorColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::LineSensorColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::LineSensorColor.at("text").get<std::string>());
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
		VarnameEdit.setText("MyLineSensor" + convertToQString(std::to_string(Counters::LINESCount)));
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 2, 1);
		this->SelfLayout.addWidget(&PinSelect, 2, 2);
		this->SelfLayout.addWidget(&NameLabel, 3, 1);
		this->SelfLayout.addWidget(&VarnameEdit, 3, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::LINESCount++;
	}

void LineSensor::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json LineSensor::toJson(){
	json DataJSON;
	DataJSON["id"] = LINES_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["linePin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["lineName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string LineSensor::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Line Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".value * 100)) + \"%\")\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);	
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.LineSensor(" + convertToStdString(this->PinSelect.currentText()) + ", pin_factory=__remote_pin_factory)\n";
}

std::string LineSensor::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string VNamePrint = "print(\"Line Value recorded by " + convertToStdString(this->VarnameEdit.text()) + " : \" + str(int(" + convertToStdString(this->VarnameEdit.text()) + ".value * 100)) + \"%\")\n";
	this->ParentDrawArea->LoopCodeVector.push_back(VNamePrint);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.LineSensor(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool LineSensor::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name provided for " + this->GPIOName);
		return false;
	}
	return true;
}