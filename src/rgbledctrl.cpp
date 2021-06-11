#include "main.hpp"
#include "config.cpp"

RGBLEDCtrl::RGBLEDCtrl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	RPinSlider(Qt::Horizontal, this),
	GPinSlider(Qt::Horizontal, this),
	BPinSlider(Qt::Horizontal, this),
	NameLabel("RGB LED : ", this),
	DisplayLabel(convertToQString(name), this),
	RLabel("Red : ", this),
	GLabel("Green : ", this),
	BLabel("Blue : ", this),
	RGBLEDSelect(this),
	RValueEdit(this),
	GValueEdit(this),
	BValueEdit(this){
		this->id = RGBLEDCTRL_ID;
		this->backgroundcolor = convertToQString(Config::RGBLEDCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::RGBLEDCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::RGBLEDCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		NameLabel.setStyleSheet("border : 0px; margin-top : 5px;");
		RLabel.setStyleSheet("border : 0px;");
		GLabel.setStyleSheet("border : 0px;");
		BLabel.setStyleSheet("border : 0px;");
		for (QLineEdit* ValueEdit : {&RValueEdit, &GValueEdit, &BValueEdit}){
			ValueEdit->setFixedWidth(60);
			ValueEdit->setText("0");
			ValueEdit->setStyleSheet("background-color : " + this->foreground + ";");
		}
		for (QSlider* PinSlider : {&RPinSlider, &GPinSlider, &BPinSlider}){
			PinSlider->setFixedWidth(100);
			PinSlider->setMaximum(255);
			PinSlider->setMinimum(0);
			PinSlider->setValue(0);
			PinSlider->setStyleSheet("border : 0px;");
		}
		this->SelfLayout.addWidget(&DisplayLabel, 1, 1, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 3, 1, 1, 1);
		this->SelfLayout.addWidget(&RGBLEDSelect, 4, 1, 1, 2);
		this->SelfLayout.addWidget(&RLabel, 5, 1, 1, 1);
		this->SelfLayout.addWidget(&RPinSlider, 6, 1, 1, 1);
		this->SelfLayout.addWidget(&RValueEdit, 6, 2, 1, 1);
		this->SelfLayout.addWidget(&GLabel, 7, 1, 1, 1);
		this->SelfLayout.addWidget(&GPinSlider, 8, 1, 1, 1);
		this->SelfLayout.addWidget(&GValueEdit, 8, 2, 1, 1);
		this->SelfLayout.addWidget(&BLabel, 9, 1, 1, 1);
		this->SelfLayout.addWidget(&BPinSlider, 10, 1, 1, 1);
		this->SelfLayout.addWidget(&BValueEdit, 10, 2, 1, 1);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		QObject::connect(&this->RPinSlider, SIGNAL (valueChanged(int)), this, SLOT (RSliderValueUpdated(int)));
		QObject::connect(&this->GPinSlider, SIGNAL (valueChanged(int)), this, SLOT (GSliderValueUpdated(int)));
		QObject::connect(&this->BPinSlider, SIGNAL (valueChanged(int)), this, SLOT (BSliderValueUpdated(int)));
		QObject::connect(&this->RValueEdit, SIGNAL (textEdited(QString)), this, SLOT (RTextValueUpdated(QString)));
		QObject::connect(&this->GValueEdit, SIGNAL (textEdited(QString)), this, SLOT (GTextValueUpdated(QString)));
		QObject::connect(&this->BValueEdit, SIGNAL (textEdited(QString)), this, SLOT (BTextValueUpdated(QString)));
		Counters::RGBLEDCTRLCount++;
	}

void RGBLEDCtrl::deleteSelf(){
	delete this;
}

json RGBLEDCtrl::toJson(){
	json DataJSON;
	DataJSON["id"] = RGBLEDCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetRGBLEDName"] = convertToStdString(this->RGBLEDSelect.currentText());
	DataJSON["targetRGBLEDRed"] = this->RPinSlider.value();
	DataJSON["targetRGBLEDGreen"] = this->GPinSlider.value();
	DataJSON["targetRGBLEDBlue"] = this->GPinSlider.value();
	return DataJSON;
}

std::string RGBLEDCtrl::remoteBuild(){
	float red, blue, green;
	red = (float) this->RPinSlider.value() / 255;
	green = (float) this->GPinSlider.value() / 255;
	blue = (float) this->BPinSlider.value() / 255;
	return convertToStdString(this->RGBLEDSelect.currentText()) + ".color = (" +
	std::to_string(red) + ", " + std::to_string(green) + ", " + std::to_string(blue) + ")\n";
}

std::string RGBLEDCtrl::simpleBuild(){
	return this->remoteBuild();
	// Same thing, no need to copy.
}

bool RGBLEDCtrl::validateInput(){
	if (this->RGBLEDSelect.currentText().simplified().isEmpty()){
		this->ParentMainWindow->err("No RGB LED selected for " + this->GPIOName);
		return false;
	}
	return true;
}

void RGBLEDCtrl::RSliderValueUpdated(int value){
	this->RValueEdit.setText(convertToQString(std::to_string(value)));
}
void RGBLEDCtrl::GSliderValueUpdated(int value){
	this->GValueEdit.setText(convertToQString(std::to_string(value)));
}
void RGBLEDCtrl::BSliderValueUpdated(int value){
	this->BValueEdit.setText(convertToQString(std::to_string(value)));
}
void RGBLEDCtrl::RTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 255){
		this->RPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Red value provided for " + this->GPIOName);
	}	
}
void RGBLEDCtrl::GTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 255){
		this->GPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Green value provided for " + this->GPIOName);
	}	
}
void RGBLEDCtrl::BTextValueUpdated(QString text){
	int val = std::stoi(convertToStdString(text));
	if (val >= 0 && val <= 255){
		this->BPinSlider.setValue(val);
	} else {
		this->ParentMainWindow->err("Invalid Blue value provided for " + this->GPIOName);
	}	
}