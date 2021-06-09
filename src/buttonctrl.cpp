#include "main.hpp"
#include "config.cpp"

ButtonControl::ButtonControl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	ButtonSelect(this),
	StateSelect(this),
	FunctionSelect(this),
	DisplayLabel(convertToQString(name), this),
	ButtonLabel("Select Button to target : ", this),
	StateLabel("Select Button Trigger State : ", this),
	ExecuteLabel("Select Function to execute : ", this)
	{
		this->id = BTNCTRL_ID;
		this->backgroundcolor = convertToQString(Config::ButtonCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::ButtonCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::ButtonCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		StateSelect.addItem("when_held");
		StateSelect.addItem("when_pressed");
		StateSelect.addItem("when_released");
		DisplayLabel.setFixedSize(180, 20);
		StateSelect.setStyleSheet("background-color : " + this->foreground + ";");
		ButtonSelect.setStyleSheet("background-color : " + this->foreground + ";");
		FunctionSelect.setStyleSheet("background-color : " + this->foreground + ";");
		ButtonLabel.setStyleSheet("border : 0px;");
		StateLabel.setStyleSheet("border : 0px;");
		ExecuteLabel.setStyleSheet("border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&ButtonLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&ButtonSelect, 2, 1, 1, 2);
		this->SelfLayout.addWidget(&StateLabel, 3, 1, 1 ,2);
		this->SelfLayout.addWidget(&StateSelect, 4, 1, 1, 2);
		this->SelfLayout.addWidget(&ExecuteLabel, 5, 1, 1, 2);
		this->SelfLayout.addWidget(&FunctionSelect, 6, 1, 1, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::BTNCTRLCount++;
	}

void ButtonControl::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json ButtonControl::toJson(){
	json DataJSON;
	DataJSON["id"] = BTNCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetButtonName"] = convertToStdString(this->ButtonSelect.currentText());
	DataJSON["targetButtonState"] = convertToStdString(this->StateSelect.currentText());
	DataJSON["targetFunctionName"] = convertToStdString(this->FunctionSelect.currentText());
	return DataJSON;
}

std::string ButtonControl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->ButtonSelect.currentText() + "." + this->StateSelect.currentText() + " = " + this->FunctionSelect.currentText() + "\n");
}

std::string ButtonControl::simpleBuild(){
	return this->remoteBuild();
}

bool ButtonControl::validateInput(){
	if (this->FunctionSelect.currentText() == ""){
		this->ParentMainWindow->err("No Function Selected for " + this->GPIOName);
		return false;
	}
	if (this->ButtonSelect.currentText() == ""){
		this->ParentMainWindow->err("No Button Selected for " + this->GPIOName);
		return false;
	}
	return true;	
}