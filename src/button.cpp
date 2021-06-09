#include "main.hpp"
#include "config.cpp"

Button::Button(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("Name : ", this){
		this->id = BTN_ID;
		this->backgroundcolor = convertToQString(Config::ButtonColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::ButtonColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::ButtonColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		if (Config::legacyMode == 0){
			for (int i : {0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 1){
			for (int i : {2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		} else if (Config::legacyMode == 2){
			for (int i : {2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27}){
				PinSelect.addItem(convertToQString(std::to_string(i)));
			}
		}
		DisplayLabel.setFixedSize(180, 20);
		PinLabel.setStyleSheet("border : 0px;");
		this->PinSelect.setStyleSheet("background-color : " + this->foreground + ";");
		NameLabel.setStyleSheet("border : 0px;");
		VarnameEdit.setText("MyButton" + convertToQString(std::to_string(Counters::BUTTONCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::BUTTONCount++;
}

void Button::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json Button::toJson(){
	json DataJSON;
	DataJSON["id"] = BTN_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["buttonPin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["buttonName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string Button::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.Button(" + convertToStdString(this->PinSelect.currentText()) +
	", pin_factory=__remote_pin_factory)\n";
}

std::string Button::simpleBuild(){
	return convertToStdString(this->VarnameEdit.text()) +
	" = gpiozero.Button(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool Button::validateInput(){
	if (this->PinSelect.currentText().simplified().isEmpty() || this->VarnameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
}