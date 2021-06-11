#include "main.hpp"
#include "config.cpp"

FunctionControl::FunctionControl(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	FunctionSelect(this),
	DisplayLabel(convertToQString(name), this),
	ExecuteLabel("Execute : ", this),
	LoopLabel("Run only in loop : ", this),
	LoopCheckBox(this)
	{
		this->id = FCTRL_ID;
		this->backgroundcolor = convertToQString(Config::FunctionCtrlColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::FunctionCtrlColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::FunctionCtrlColor.at("text").get<std::string>());
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		ExecuteLabel.setStyleSheet("border : 0px;");
		LoopLabel.setStyleSheet("border : 0px;");
		FunctionSelect.setStyleSheet("background-color : " + this->foreground + ";");
		LoopCheckBox.setStyleSheet("background-color : " + this->foreground + "; border : 0px;");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&ExecuteLabel, 1, 1);
		this->SelfLayout.addWidget(&FunctionSelect, 1, 2);
		this->SelfLayout.addWidget(&LoopLabel, 2, 1);
		this->SelfLayout.addWidget(&LoopCheckBox, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
		Counters::FUNCTRLCount++;
	}

void FunctionControl::deleteSelf(){
	delete this;
}

json FunctionControl::toJson(){
	json DataJSON;
	DataJSON["id"] = FCTRL_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["targetFunctionName"] = convertToStdString(this->FunctionSelect.currentText());
	DataJSON["targetFunctionLoopState"] = this->LoopCheckBox.isChecked();
	return DataJSON;
}

std::string FunctionControl::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string TempBuild = convertToStdString(this->FunctionSelect.currentText()) + "()\n";
	if (this->LoopCheckBox.isChecked()){
		this->ParentDrawArea->LoopCodeVector.push_back(TempBuild);
		return "";
		// Return blank string
	} else {
		return TempBuild;
	}
	
}

std::string FunctionControl::simpleBuild(){
	return this->remoteBuild();
}

bool FunctionControl::validateInput(){
	if (this->FunctionSelect.currentText() == ""){
		this->ParentMainWindow->err("No function selected for " + this->GPIOName);
		return false;
	}
	return true;
}