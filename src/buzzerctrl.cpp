#include "main.hpp"
#include "config.cpp"

BuzzerCtrl::BuzzerCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	BuzzerSelect(this),
	StateSelect(this),
	DisplayLabel(convertToQString(this->GPIOName), this),
	BuzzerLabel("Buzzer : ", this),
	StateLabel("Pin State : ", this)
	{
		this->id = BUZCTRL_ID;
		this->backgroundcolor = convertToQString(Config::BuzzerCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::BuzzerCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::BuzzerCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		BuzzerLabel.setStyleSheet("border : 0px;");
		BuzzerSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.setStyleSheet("background-color : " + this->foreground + ";");
		StateSelect.addItem("on");
		StateSelect.addItem("off");
		StateLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&BuzzerLabel, 1, 1);
		this->SelfLayout.addWidget(&this->BuzzerSelect, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 2, 1);
		this->SelfLayout.addWidget(&this->StateSelect, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::BUZZERCTRLCount++;
	}

void BuzzerCtrl::deleteSelf(){
	delete this;
}

json BuzzerCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = BUZCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetBuzzerName"] = convertToStdString(this->BuzzerSelect.currentText());
	DataJSON["targetBuzzerState"] = convertToStdString(this->StateSelect.currentText());
	return DataJSON;
}

std::string BuzzerCtrl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->BuzzerSelect.currentText()) + "." + convertToStdString(this->StateSelect.currentText()) + "()\n";
}

std::string BuzzerCtrl::simpleBuild(){
	return this->remoteBuild();
}

bool BuzzerCtrl::validateInput(){
	if (this->BuzzerSelect.currentText() == ""){
		this->ParentMainWindow->err("No Buzzer Selected for " + this->GPIOName);
		return false;
	}
	return true;
}