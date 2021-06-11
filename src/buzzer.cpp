#include "main.hpp"
#include "config.cpp"

Buzzer::Buzzer(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) : 
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	PinSelect(this),
	VarnameEdit(this),
	DisplayLabel(convertToQString(name), this),
	PinLabel("Pin : ", this),
	NameLabel("State : ", this)
	{
		this->id = BUZ_ID;
		this->backgroundcolor = convertToQString(Config::BuzzerColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::BuzzerColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::BuzzerColor.at("text").get<std::string>());
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
		VarnameEdit.setText("MyBuzzer" + convertToQString(std::to_string(Counters::BUZZERCount)));
		this->VarnameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&PinLabel, 1, 1);
		this->SelfLayout.addWidget(&this->PinSelect, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 2, 1);
		this->SelfLayout.addWidget(&this->VarnameEdit, 2, 2);
		Counters::BUZZERCount++;
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
}

void Buzzer::deleteSelf(){
	delete this;
}

json Buzzer::toJson(){
	json DataJSON;
	DataJSON["id"] = BUZ_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["buzzerPin"] = std::stoi(convertToStdString(this->PinSelect.currentText()));
	DataJSON["buzzerName"] = convertToStdString(this->VarnameEdit.text());
	return DataJSON;
}

std::string Buzzer::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.Buzzer(" + convertToStdString(this->PinSelect.currentText()) +
		", pin_factory=__remote_pin_factory)\n";
}

std::string Buzzer::simpleBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	return convertToStdString(this->VarnameEdit.text()) +
		" = gpiozero.Buzzer(" + convertToStdString(this->PinSelect.currentText()) + ")\n";
}

bool Buzzer::validateInput(){
	if (this->PinSelect.currentText() == "" || this->VarnameEdit.text() == ""){
		this->ParentMainWindow->err("No suitable variable name or pin number provided for " + this->GPIOName);
		return false;
	}
	return true;
}