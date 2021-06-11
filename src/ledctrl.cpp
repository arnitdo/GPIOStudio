#include "main.hpp"
#include "config.cpp"

LEDCtrl::LEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	LEDSelect(this),
	StateSelect(this),
	DisplayLabel(convertToQString(name), this),
	LEDLabel("LED : ", this),
	StateLabel("Pin State : ", this)
	{
		this->id = LEDCTRL_ID;
		this->backgroundcolor = convertToQString(Config::LEDCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::LEDCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::LEDCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		LEDLabel.setStyleSheet("border : 0px;");
		LEDSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.addItem("on");
		StateSelect.addItem("off");
		StateLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&LEDLabel, 1, 1);
		this->SelfLayout.addWidget(&this->LEDSelect, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 2, 1);
		this->SelfLayout.addWidget(&this->StateSelect, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::LEDCTRLCount++;
	}

void LEDCtrl::deleteSelf(){
	delete this;
}

json LEDCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = LEDCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetLEDName"] = convertToStdString(this->LEDSelect.currentText());
	DataJSON["targetLEDState"] = convertToStdString(this->StateSelect.currentText());
	return DataJSON;
}

std::string LEDCtrl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->LEDSelect.currentText()) + "." + convertToStdString(this->StateSelect.currentText()) + "()\n";
}

std::string LEDCtrl::simpleBuild(){
	return this->remoteBuild();
}

bool LEDCtrl::validateInput(){
	if (this->LEDSelect.currentText().simplified().isEmpty()){
		this->ParentMainWindow->err("No LED Selected for " + this->GPIOName);
		return false;
	}
	return true;
}