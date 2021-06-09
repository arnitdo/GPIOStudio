#include "main.hpp"
#include "config.cpp"

RGBLED::RGBLED(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	RPinSelect(this),
	GPinSelect(this),
	BPinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	RPinLabel("Red : ", this),
	GPinLabel("Green : ", this),
	BPinLabel("Blue : ", this),
	NameLabel("Name : ", this){
		this->id = RGBLED_ID;
		this->backgroundcolor = convertToQString(Config::RGBLEDColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::RGBLEDColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::RGBLEDColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		// Based on Config::legacyMode, change number of pins available
		if (Config::legacyMode == 0){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				RPinSelect.addItem(convertToQString(std::to_string(i)));
				GPinSelect.addItem(convertToQString(std::to_string(i)));
				BPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				RPinSelect.addItem(convertToQString(std::to_string(i)));
				GPinSelect.addItem(convertToQString(std::to_string(i)));
				BPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				RPinSelect.addItem(convertToQString(std::to_string(i)));
				GPinSelect.addItem(convertToQString(std::to_string(i)));
				BPinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		
		DisplayLabel.setFixedSize(180, 20);
		RPinLabel.setStyleSheet("border : 0px;");
		this->RPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		GPinLabel.setStyleSheet("border : 0px;");
		this->GPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		BPinLabel.setStyleSheet("border : 0px;");
		this->BPinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyRGBLED" + convertToQString(std::to_string(Counters::RGBLEDCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&RPinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->RPinSelect, 1, 2);
		this->SelfLayout.addWidget(&GPinLabel, 2, 1);
		this->SelfLayout.addWidget(&this->GPinSelect, 2, 2);
		this->SelfLayout.addWidget(&BPinLabel, 3, 1);
		this->SelfLayout.addWidget(&this->BPinSelect, 3, 2);
		this->SelfLayout.addWidget(&NameLabel, 4, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 4, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::RGBLEDCount++;
}

void RGBLED::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json RGBLED::toJson(){
	json DataJSON;
	DataJSON["id"] = RGBLED_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["RGBLEDPinR"] = std::stoi(convertToStdString(this->RPinSelect.currentText()));
	DataJSON["RGBLEDPinG"] = std::stoi(convertToStdString(this->GPinSelect.currentText()));
	DataJSON["RGBLEDPinB"] = std::stoi(convertToStdString(this->BPinSelect.currentText()));
	DataJSON["RGBLEDName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string RGBLED::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.RGBLED(" + convertToStdString(this->RPinSelect.currentText()) +
		", " + convertToStdString(this->GPinSelect.currentText()) +
		", " + convertToStdString(this->BPinSelect.currentText()) +
		", pin_factory=__remote_pin_factory)\n";	
}

std::string RGBLED::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.RGBLED(" + convertToStdString(this->RPinSelect.currentText()) +
	", " + convertToStdString(this->GPinSelect.currentText()) +
	", " + convertToStdString(this->BPinSelect.currentText()) + ")\n";
}

bool RGBLED::validateInput(){
	if (this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	if (this->RPinSelect.currentText() == this->GPinSelect.currentText() || 
		this->BPinSelect.currentText() == this->GPinSelect.currentText() ||
		this->RPinSelect.currentText() == this->BPinSelect.currentText()){
			this->ParentMainWindow->err("Duplicate pins provided for " + this->GPIOName);
			return false;
		}
	return true;
}