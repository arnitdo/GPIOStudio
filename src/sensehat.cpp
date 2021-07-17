#include "main.hpp"
#include "config.cpp"

SenseHat::SenseHat(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X,Y, name),
	SelfLayout(this),
	DisplayLabel(this),
	ReportLabel("Sensor Reports : ",this),
	HumidityBox("Humidity", this),
	TemperatureHBox("Temperature (H)", this),
	TemperaturePBox("Temperature (P)", this),
	PressureBox("Pressure", this){
		this->id = SENSE_ID;
		this->backgroundcolor = convertToQString(Config::SenseHatColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::SenseHatColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::SenseHatColor.at("text").get<std::string>());
		DisplayLabel.setText(convertToQString(name));
		DisplayLabel.setFixedHeight(30);
		ReportLabel.setStyleSheet("border : 0px;");
		ReportLabel.setFixedHeight(30);
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->ParentMainWindow->MainWindowRemoteButton.setDisabled(true);
		// Sense HATs are not supported over remote GPIO.
		this->ParentMainWindow->warn("Sense HATs are not supported over Remote GPIO!");
		this->ParentMainWindow->warn("Remote Run is disabled.");
		for (QCheckBox* Box : {&HumidityBox, &TemperatureHBox, &PressureBox, &TemperaturePBox}){
			Box->setStyleSheet("border : 0px;");
		}
		SelfLayout.addWidget(&DisplayLabel, 1, 1);
		SelfLayout.addWidget(&ReportLabel, 2, 1);
		SelfLayout.addWidget(&HumidityBox, 3, 1);
		SelfLayout.addWidget(&TemperatureHBox, 4, 1);
		SelfLayout.addWidget(&TemperaturePBox, 5, 1);
		SelfLayout.addWidget(&PressureBox, 6, 1);
}

void SenseHat::deleteSelf(){
	this->ParentMainWindow->MainWindowRemoteButton.setDisabled(false);
	delete this;
}

json SenseHat::toJson(){
	json DataJSON;
	DataJSON["id"] = SENSE_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["humidity"] = this->HumidityBox.isChecked();
	DataJSON["temperatureH"] = this->TemperatureHBox.isChecked();
	DataJSON["temperatureP"] = this->TemperaturePBox.isChecked();
	DataJSON["pressure"] = this->PressureBox.isChecked();
	return DataJSON;
}

bool SenseHat::validateInput(){
	return true;
}

std::string SenseHat::remoteBuild(){
	if (this->HumidityBox.isChecked()){
		this->ParentDrawArea->LoopCodeVector.push_back("print(\"Relative Humidity recorded by Sense HAT : \" + str(int(__sense_hat.get_humidity())) + \"%\")\n");
	}
	if (this->TemperatureHBox.isChecked()){
		this->ParentDrawArea->LoopCodeVector.push_back("print(\"Temperature calculated from humidity by Sense HAT : \" + str(int(__sense_hat.get_temperature_from_humidity())) + \"deg C\")\n");
	}
	if (this->TemperaturePBox.isChecked()){
		this->ParentDrawArea->LoopCodeVector.push_back("print(\"Temperature calculated from presssure by Sense HAT : \" + str(int(__sense_hat.get_temperature_from_presssure())) + \"deg C\")\n");
	}
	if (this->PressureBox.isChecked()){
		this->ParentDrawArea->LoopCodeVector.push_back("print(\"Pressure recorded by Sense HAT (in Millibars) : \" + str(int(__sense_hat.get_pressure())))\n");
	}
	return  "__sense_hat = sense_hat.SenseHat()\n"
			"def __sense_stick_handler(sense_stick_event):\n"
			"\tprint(\"Direction : \" + sense_stick_event.direction + \"Action : \" + sense_stick_event.action)\n"
			"__sense_hat.stick.direction_any = __sense_stick_handler\n";
}

std::string SenseHat::simpleBuild(){
	return this->remoteBuild();
}