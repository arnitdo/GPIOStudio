#include "main.hpp"
#include "config.cpp"

PWMLEDCtrl::PWMLEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PWMLEDSelect(this),
	ValueEdit(this),
	DisplayLabel(convertToQString(name), this),
	PWMLEDLabel("PWMLED : ", this),
	ValueLabel("Value : ", this)
	{
		this->id = PWMLEDCTRL_ID;
		this->backgroundcolor = convertToQString(Config::PWMLEDColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::PWMLEDColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::PWMLEDColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		PWMLEDLabel.setStyleSheet("border : 0px;");
		ValueEdit.setMaxLength(3);
		ValueEdit.setText("0");
		PWMLEDSelect.setStyleSheet("background-color : " + this->foreground + ";");
		ValueEdit.setStyleSheet("background-color : " + this->foreground + ";");
		ValueLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PWMLEDLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PWMLEDSelect, 1, 2);
		this->SelfLayout.addWidget(&ValueLabel, 2, 1);
		this->SelfLayout.addWidget(&this->ValueEdit, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::PWMLEDCTRLCount++;
	}

void PWMLEDCtrl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json PWMLEDCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = PWMLEDCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetPWMLEDName"] = convertToStdString(this->PWMLEDSelect.currentText());
	DataJSON["targetPWMLEDvalue"] = std::stoi(convertToStdString(this->ValueEdit.text()));
	return DataJSON;
}

std::string PWMLEDCtrl::remoteBuild(){
	float value = std::stof(convertToStdString(this->ValueEdit.text())) / 100; 
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->PWMLEDSelect.currentText()) + ".value = " + std::to_string(value) + "\n";
}

std::string PWMLEDCtrl::simpleBuild(){
	return this->remoteBuild();
}

bool PWMLEDCtrl::validateInput(){
	int value = std::stoi(convertToStdString(this->ValueEdit.text()));
	bool valueInLimits = (0 <= value && value <= 100) ? true : false; 
	if (this->PWMLEDSelect.currentText().simplified().isEmpty()){
		this->ParentMainWindow->err("No PWM LED Selected for " + this->GPIOName);
		return false;
	}
	if (!valueInLimits){
		this->ParentMainWindow->err("Invalid value provided for " + this->GPIOName);
		return false;
	}
	return true;
}