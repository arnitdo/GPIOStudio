#include "main.hpp"
#include "config.cpp"

DrawArea::DrawArea(MainWindow *parent) : 
	QWidget(parent){
	this->ParentMainWindow = parent;
	// Insert Values for GPIOToolBar
	this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
	ButtonLabelMap.insert({PSTART_ID, "Program Start"});
	ButtonLabelMap.insert({LED_ID, "Simple LED"});
	ButtonLabelMap.insert({PWMLED_ID, "PWM LED"});
	ButtonLabelMap.insert({RGBLED_ID, "RGB LED"});
	ButtonLabelMap.insert({BUZ_ID, "Simple Buzzer"});
	ButtonLabelMap.insert({FUNC_ID, "Custom Function"});
	ButtonLabelMap.insert({BTN_ID, "Simple Button"});
	ButtonLabelMap.insert({DISTS_ID, "Distance Sensor"});
	ButtonLabelMap.insert({LIGHTS_ID, "Light Sensor"});
	ButtonLabelMap.insert({MOTION_ID, "Motion Sensor"});
	ButtonLabelMap.insert({LINES_ID, "Line Sensor"});
	ButtonLabelMap.insert({SLEEP_ID, "Sleep Timer"});
	ButtonLabelMap.insert({LEDCTRL_ID, "LED Controls"});
	ButtonLabelMap.insert({PWMLEDCTRL_ID, "PWM LED Controls"});
	ButtonLabelMap.insert({RGBLEDCTRL_ID, "RGB LED Controls"});
	ButtonLabelMap.insert({BUZCTRL_ID, "Buzzer Controls"});
	ButtonLabelMap.insert({FCTRL_ID, "Function Controls"});
	ButtonLabelMap.insert({BTNCTRL_ID, "Button Controls"});
	ButtonLabelMap.insert({SENSE_ID, "Sense HAT"});
	ButtonLabelMap.insert({SENSE_TEXT_ID, "Sense HAT Text"});
}

void DrawArea::loadJson(QString fname){
	this->ParentMainWindow->log("Opening file " + convertToStdString(fname));
	std::ifstream JSONFileIn (convertToStdString(fname));
	json JSON;
	JSONFileIn >> JSON;
	bool versionMatch = false;
	try {
		versionMatch = JSON.at("version").get<std::string>() == convertToStdString(getVersionInfo());
	} catch (json::out_of_range& JSONExcept){
		if (JSONExcept.id == 403){
			this->ParentMainWindow->log("Invalid file provided! Project will be reset!");
			emit this->ParentMainWindow->deleteGPIO();
			this->resetSelf();
		}	
	}
	if (versionMatch || Config::overrideVersionWarnings){
		// Version Matches, Proceed!
		// Emulate button click, clears the entire board
		for (json GPIOJSON : JSON["json"]){
			try {
				this->createGPIODevice(GPIOJSON);
			} catch (json::out_of_range& JSONExcept){
				if (JSONExcept.id == 403){
					this->ParentMainWindow->log("Invalid file provided! Project will be reset!");
					emit this->ParentMainWindow->deleteGPIO();
					this->resetSelf();
				}
			}
		}
	} else {
		this->ParentMainWindow->err("Version Mismatch!");
		this->ParentMainWindow->err("The version of the file you have provided does not match the current version of GPIO Studio!");
	}
}

void DrawArea::saveToJson(){
	QFileDialog OpenFileDialog;
	OpenFileDialog.setDefaultSuffix("json");
	QString fname = OpenFileDialog.getSaveFileName(this, "Save GPIO Project", "", "JSON Files (*.json)");
	if (!fname.simplified().isEmpty()){
		this->ParentMainWindow->log("Saving to File " + convertToStdString(fname));
		json JsonWrite;
		JsonWrite["version"] = convertToStdString(getVersionInfo());
		json GPIOArray = json::array();
		for (GPIODevice* GPIOD : this->GPIOCodeVector){
			GPIOArray.push_back(GPIOD->toJson());
		}
		JsonWrite["json"] = GPIOArray;
		std::ofstream WriteFile;
		WriteFile.open(convertToStdString(fname), std::ios::trunc);
		WriteFile << JsonWrite.dump(4); // Indentation!
		WriteFile.close();
	}
}

void DrawArea::Undo(){
	if (this->GPIOCodeVector.size() > 0){
		GPIODevice* GPIOD = this->GPIOCodeVector.back();
		if (GPIOD->id == PSTART_ID){
			this->ParentMainWindow->warn("Deleting Program Start Block! Entire Project will be reset!");
			this->ActionJSONBuffer.push_back(GPIOD->toJson());
			GPIOD->deleteSelf();
			this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
		} else {
			switch(GPIOD->id){
				case LED_ID:{
					Counters::LEDCount--;
					this->LEDVec.pop_back();
					break;
				}
				case PWMLED_ID:{
					Counters::PWMLEDCount--;
					this->PWMLEDVec.pop_back();
					break;
				}
				case RGBLED_ID:{
					Counters::RGBLEDCount--;
					this->RGBLEDVec.pop_back();
					break;
				}
				case BUZ_ID:{
					Counters::BUZZERCount--;
					this->BUZVec.pop_back();
					break;
				}
				case FUNC_ID:{
					Counters::FUNCTIONCount--;
					this->FUNCVec.pop_back();
					break;
				}
				case BTN_ID:{
					Counters::BUTTONCount--;
					this->BTNVec.pop_back();
					break;
				}
				case DISTS_ID:{
					Counters::DISTSCount--;
					break;
				}
				case LIGHTS_ID:{
					Counters::LIGHTSCount--;
					break;
				}
				case MOTION_ID:{
					Counters::MOTIONCount--;
					break;
				}
				case LINES_ID:{
					Counters::LINESCount--;
					break;
				}
				case SLEEP_ID:{
					Counters::SLEEPCount--;
					break;
				}
				case LEDCTRL_ID:{
					Counters::LEDCTRLCount--;
					this->LEDCTRLVec.pop_back();
					break;
				}
				case PWMLEDCTRL_ID:{
					Counters::PWMLEDCTRLCount--;
					this->PWMLEDCTRLVec.pop_back();
					break;
				}
				case RGBLEDCTRL_ID:{
					Counters::RGBLEDCTRLCount--;
					this->RGBLEDCTRLVec.pop_back();
					break;
				}
				case BUZCTRL_ID:{
					Counters::BUZZERCTRLCount--;
					this->BUZCTRLVec.pop_back();
					break;
				}
				case FCTRL_ID:{
					Counters::FUNCTRLCount--;
					this->FUNCTRLVec.pop_back();
					break;
				}
				case BTNCTRL_ID:{
					Counters::BTNCTRLCount--;
					this->BTNCTRLVec.pop_back();
					break;
				}
				default:{
					break;
				}
			}
			this->ActionJSONBuffer.push_back(GPIOD->toJson());
			GPIOD->deleteSelf(); // Delete GPIOD
			this->GPIOCodeVector.pop_back(); // Delete Reference to GPIOD

			// Sense Hat Check
			this->ParentMainWindow->MainWindowRemoteButton.setDisabled(this->checkForSenseHat());

			this->Lines.pop_back(); // Delete last line
			int YOffset = 75; // Default Y offset for 200 * 100 blocks
			// Increases for 200 * 200 blocks by 75 (Y = 150)
			GPIODevice* LastGPIO = this->GPIOCodeVector.back();
			if (LastGPIO->id == RGBLED_ID || LastGPIO->id == RGBLEDCTRL_ID || LastGPIO->id == BTNCTRL_ID || LastGPIO->id == SENSE_ID){
				YOffset += 75;
			}
			this->LastPoint = QPoint(
				LastGPIO->x() + 200, /*Get last GPIODevice, get X coord, add 200 to X*/
				LastGPIO->y() + YOffset /*Get last GPIODevice, get Y coord, add Offset to Y*/
			); 
			// Line will be redrawn on next paintEvent.
			this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
			// Save JSON
		}
	} else {
		this->ParentMainWindow->log("Nothing to undo!");
	}
}

void DrawArea::Redo(){
	if (this->ActionJSONBuffer.size() > 0){
		createGPIODevice(this->ActionJSONBuffer.back());
		this->ActionJSONBuffer.pop_back();
		this->ActionJSONBuffer.shrink_to_fit();
		this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
		// Use vector as stack
	} else {
		this->ParentMainWindow->log("Nothing to redo!");
	}
}

bool DrawArea::checkForPStart(){
	for (GPIODevice* GPIOD : this->GPIOCodeVector){
		if (GPIOD->id == PSTART_ID){
			return true; // ProgramStart exists, somewhere in the area
		}
	}
	return false;
}

bool DrawArea::checkForSenseHat(){
	for (GPIODevice* GPIOD : this->GPIOCodeVector){
		if (GPIOD->id == SENSE_ID){
			return true; // ProgramStart exists, somewhere in the area
		}
	}
	return false;
}

void DrawArea::mousePressEvent(QMouseEvent *event){
	// Set of actions to do on mouse press
	if (this->NWMode && event->button() == Qt::LeftButton){
		// Check if New Widget Mode is active
		// I.e if NWMode == false
		// No click action
		// Prevents unnecessary clicks and GPIO Constructions
		json ClickJSON;
		ClickJSON["id"] = this->activeGPIO;
		ClickJSON["x"] = event->x();
		ClickJSON["y"] = event->y();
		// Pack mouse event into JSON, and pass as reference, not copy
		this->ActionJSONBuffer.clear();
		this->createGPIODevice(ClickJSON);
		this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
		this->NWMode = false;
	}
}

void DrawArea::createGPIODevice(json& GPIOData){
	// Constructs a GPIO Device at the given coordinates
	// This function has been specifically created to allow loading of GPIO Devices from a JSON File
	// CSS Doc - Qt likes to inherit background images on its own, despite having 'background-image : none;' in css.
	// Solution - GPIODevice gets a completely blank PNG file
	// Horrible fix, no other option. :-(
	int id = GPIOD_ID;
	int X = 0;
	int Y = 0;
	try {
		id = GPIOData.at("id").get<int64_t>();
		X = GPIOData.at("x").get<int64_t>();
		Y = GPIOData.at("y").get<int64_t>();
		this->activeGPIO = id;
	} catch (json::out_of_range& JSONExcept){
		if (JSONExcept.id == 403){
			this->ParentMainWindow->err("Invalid JSON Data Provided!");
		}
		return;
	}	
	switch(this->activeGPIO){
		case PSTART_ID:{
			QMessageBox NewPStartDialog (this->ParentMainWindow);
			NewPStartDialog.setIcon(QMessageBox::Warning);
			NewPStartDialog.setText("You are creating a new Program Start Block.\nAll previous blocks will be removed.\nDo you want to continue?");
			NewPStartDialog.setStandardButtons(QMessageBox::Yes | QMessageBox::No);
			switch(NewPStartDialog.exec()){
				case QMessageBox::Yes:{
					emit this->ParentMainWindow->deleteGPIO();
					this->resetSelf();
					// Reset self, so that any programstart created will be at top of code
					QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
					ProgramStart* GPIOD = new ProgramStart(this, ParentMainWindow, X, Y, ("Program Start"));
					GPIOD->setGeometry(GPIOBoundBox);
					GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
					GPIOD->show();
					this->LastPoint = QPoint(X + 200, Y + 75);
					this->Lines.push_back(std::make_pair(this->LastPoint, this->LastPoint));
					this->GPIOCodeVector.push_back(GPIOD);
					break;
				}
				case QMessageBox::No:{
					this->ParentMainWindow->log("Cancelled creation of Program Start Block");
				}
			}
			break;
		}
		case LED_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				LED* GPIOD = new LED(this, ParentMainWindow, X, Y, ("LED " + std::to_string(Counters::LEDCount)));
				if (GPIOData.contains("LEDPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("LEDPin").get<int64_t>()))
					);
				}
				// Get LED Pin and LED Name property, if exists, and set text
				if (GPIOData.contains("LEDName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("LEDName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->LEDVec.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case PWMLED_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				PWMLED* GPIOD = new PWMLED(this, ParentMainWindow, X, Y, ("PWM LED " + std::to_string(Counters::PWMLEDCount)));
				if (GPIOData.contains("PWMLEDPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("PWMLEDPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("PWMLEDName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("PWMLEDName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->PWMLEDVec.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case RGBLED_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				RGBLED* GPIOD = new RGBLED(this, ParentMainWindow, X, Y, ("RGB LED " + std::to_string(Counters::RGBLEDCount)));
				if (GPIOData.contains("RGBLEDPinR")){
					GPIOD->RPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("RGBLEDPinR").get<int64_t>()))
					);
				}
				if (GPIOData.contains("RGBLEDPinG")){
					GPIOD->GPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("RGBLEDPinG").get<int64_t>()))
					);
				}
				if (GPIOData.contains("RGBLEDPinB")){
					GPIOD->BPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("RGBLEDPinB").get<int64_t>()))
					);
				}
				if (GPIOData.contains("RGBLEDName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("RGBLEDName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 100);
				this->GPIOCodeVector.push_back(GPIOD);
				this->RGBLEDVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BUZ_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Buzzer* GPIOD = new Buzzer(this, ParentMainWindow, X, Y, ("Buzzer " + std::to_string(Counters::BUZZERCount)));
				if (GPIOData.contains("buzzerPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("buzzerPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("buzzerName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("buzzerName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->BUZVec.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}			
		}
		case FUNC_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Function* GPIOD = new Function(this, ParentMainWindow, X, Y, ("Function " + std::to_string(Counters::FUNCTIONCount)));
				if (GPIOData.contains("functionName")){
					GPIOD->NameEdit.setText(
						convertToQString(GPIOData.at("functionName").get<std::string>())
					);
				}
				if (GPIOData.contains("functionBody")){
					GPIOD->FunctionBody->setText(
						convertToQString(GPIOData.at("functionBody").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->FUNCVec.push_back(GPIOD);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BTN_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Button* GPIOD = new Button(this, ParentMainWindow, X, Y, ("Button " + std::to_string(Counters::BUTTONCount)));
				if (GPIOData.contains("buttonPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("buttonPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("buttonName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("buttonName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->BTNVec.push_back(GPIOD);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case DISTS_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				DistanceSensor* GPIOD = new DistanceSensor(this, ParentMainWindow, X, Y, ("Distance Sensor " + std::to_string(Counters::DISTSCount)));
				if (GPIOData.contains("distanceEchoPin")){
					GPIOD->EchoPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("distanceEchoPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("distanceTrigPin")){
					GPIOD->TrigPinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("distanceTrigPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("distanceName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("distanceName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case LIGHTS_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				LightSensor* GPIOD = new LightSensor(this, ParentMainWindow, X, Y, ("Light Sensor " + std::to_string(Counters::LIGHTSCount)));
				if (GPIOData.contains("lightPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("lightPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("lightName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("lightName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case MOTION_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				MotionSensor* GPIOD = new MotionSensor(this, ParentMainWindow, X, Y, ("Motion Sensor " + std::to_string(Counters::MOTIONCount)));
				if (GPIOData.contains("motionPin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("motionPin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("motionName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("motionName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case LINES_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 110));
				LineSensor* GPIOD = new LineSensor(this, ParentMainWindow, X, Y, ("Line Sensor " + std::to_string(Counters::LINESCount)));
				if (GPIOData.contains("linePin")){
					GPIOD->PinSelect.setCurrentText(
						convertToQString(std::to_string(GPIOData.at("linePin").get<int64_t>()))
					);
				}
				if (GPIOData.contains("lineName")){
					GPIOD->VarnameEdit.setText(
						convertToQString(GPIOData.at("lineName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case SLEEP_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				Sleep* GPIOD = new Sleep(this, ParentMainWindow, X, Y, ("Sleep Timer " + std::to_string(Counters::SLEEPCount)));
				if (GPIOData.contains("sleepTime")){
					GPIOD->DurationEdit.setText(
						convertToQString(std::to_string(GPIOData.at("sleepTime").get<int64_t>()))
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}		
		case LEDCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				LEDCtrl* GPIOD = new LEDCtrl(this, ParentMainWindow, X, Y, ("LED Controls " + std::to_string(Counters::LEDCTRLCount)));
				if (GPIOData.contains("targetLEDName")){
					GPIOD->LEDSelect.setCurrentText(
						convertToQString(GPIOData.at("targetLEDName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetLEDState")){
					GPIOD->StateSelect.setCurrentText(
						convertToQString(GPIOData.at("targetLEDState").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->LEDCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case PWMLEDCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				PWMLEDCtrl* GPIOD = new PWMLEDCtrl(this, ParentMainWindow, X, Y, ("PWM LED Controls " + std::to_string(Counters::PWMLEDCTRLCount)));
				if (GPIOData.contains("targetPWMLEDName")){
					GPIOD->PWMLEDSelect.setCurrentText(
						convertToQString(GPIOData.at("targetPWMLEDName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetPWMLEDvalue")){
					GPIOD->ValueEdit.setText(
						convertToQString(std::to_string(GPIOData.at("targetPWMLEDvalue").get<int64_t>()))
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->PWMLEDCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case RGBLEDCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				RGBLEDCtrl* GPIOD = new RGBLEDCtrl(this, ParentMainWindow, X, Y, ("RGB LED Controls " + std::to_string(Counters::RGBLEDCTRLCount)));
				if (GPIOData.contains("targetRGBLEDName")){
					GPIOD->RGBLEDSelect.setCurrentText(
						convertToQString(GPIOData.at("targetRGBLEDName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetRGBLEDRed")){
					GPIOD->RPinSlider.setValue(
						GPIOData.at("targetRGBLEDRed").get<int64_t>()
					);
				}
				if (GPIOData.contains("targetRGBLEDGreen")){
					GPIOD->GPinSlider.setValue(
						GPIOData.at("targetRGBLEDGreen").get<int64_t>()
					);
				}
				if (GPIOData.contains("targetRGBLEDBlue")){
					GPIOD->BPinSlider.setValue(
						GPIOData.at("targetRGBLEDBlue").get<int64_t>()
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->RGBLEDCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BUZCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				BuzzerCtrl* GPIOD = new BuzzerCtrl(this, ParentMainWindow, X, Y, ("Buzzer Controls " + std::to_string(Counters::BUZZERCTRLCount)));
				if (GPIOData.contains("targetBuzzerName")){
					GPIOD->BuzzerSelect.setCurrentText(
						convertToQString(GPIOData.at("targetBuzzerName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetBuzzerState")){
					GPIOD->StateSelect.setCurrentText(
						convertToQString(GPIOData.at("targetBuzzerState").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->BUZCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}

		case FCTRL_ID:{
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				FunctionControl* GPIOD = new FunctionControl(this, ParentMainWindow, X, Y, ("Function Controls " + std::to_string(Counters::FUNCTRLCount)));
				if (GPIOData.contains("targetFunctionName")){
					GPIOD->FunctionSelect.setCurrentText(
						convertToQString(GPIOData.at("targetFunctionName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetFunctionLoopState")){
					GPIOD->LoopCheckBox.setCheckState(
						GPIOData.at("targetFunctionLoopState").get<bool>() ? Qt::Checked : Qt::Unchecked
						// Quick ternary operator
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				this->FUNCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case BTNCTRL_ID: {
			if (this->checkForPStart()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				ButtonControl* GPIOD = new ButtonControl(this, ParentMainWindow, X, Y, ("Button Controls " + std::to_string(Counters::BTNCTRLCount)));
				if (GPIOData.contains("targetButtonName")){
					GPIOD->ButtonSelect.setCurrentText(
						convertToQString(GPIOData.at("targetButtonName").get<std::string>())
					);
				}
				if (GPIOData.contains("targetButtonState")){
					GPIOD->StateSelect.setCurrentText(
						convertToQString(GPIOData.at("targetButtonState").get<std::string>())
					);
				}
				if (GPIOData.contains("targetFunctionName")){
					GPIOD->FunctionSelect.setCurrentText(
						convertToQString(GPIOData.at("targetFunctionName").get<std::string>())
					);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 100);
				this->GPIOCodeVector.push_back(GPIOD);
				this->BTNCTRLVec.push_back(GPIOD);
				this->RefreshSelects();
				break;
			} else {
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;		
			}
		}
		case SENSE_ID:{
			if (this->checkForPStart() && !this->checkForSenseHat()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 200));
				SenseHat* GPIOD = new SenseHat(this, ParentMainWindow, X, Y, "Sense HAT");
				if (GPIOData.contains("humidity")){
					GPIOD->HumidityBox.setCheckState(GPIOData.at("humidity").get<bool>() ? Qt::Checked : Qt::Unchecked);
				}
				if (GPIOData.contains("temperatureH")){
					GPIOD->TemperatureHBox.setCheckState(GPIOData.at("temperatureH").get<bool>() ? Qt::Checked : Qt::Unchecked);
				}
				if (GPIOData.contains("temperatureP")){
					GPIOD->TemperaturePBox.setCheckState(GPIOData.at("temperatureP").get<bool>() ? Qt::Checked : Qt::Unchecked);
				}
				if (GPIOData.contains("pressure")){
					GPIOD->PressureBox.setCheckState(GPIOData.at("pressure").get<bool>() ? Qt::Checked : Qt::Unchecked);
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 50);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 100);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else if (!this->checkForPStart()){
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;
			} else {
				this->ParentMainWindow->err("A Sense HAT Device already exists!");
				this->ParentMainWindow->err("Multiple Instances of Sense HATs are not supported!");
				break;
			}
		}
		case SENSE_TEXT_ID:{
			if (this->checkForPStart() && this->checkForSenseHat()){
				QRect GPIOBoundBox = QRect(QPoint(X, Y), QPoint(X + 200, Y + 100));
				SenseText* GPIOD = new SenseText(this, ParentMainWindow, X, Y, "Sense HAT Text");
				if (GPIOData.contains("displayText")){
					GPIOD->TextEdit.setPlainText(convertToQString(GPIOData.at("displayText").get<std::string>()));
				}
				GPIOD->setGeometry(GPIOBoundBox);
				GPIOD->setStyleSheet("border : 1px solid black; color : " + GPIOD->textcolor + "; background-color : " + GPIOD->backgroundcolor + "; background-image : url('static/blank.png');");
				GPIOD->show();
				this->CurrentPoint = QPoint(X, Y + 25);
				this->Lines.push_back(std::make_pair(this->LastPoint, this->CurrentPoint));
				this->LastPoint = QPoint(this->CurrentPoint.x() + 200, this->CurrentPoint.y() + 50);
				this->GPIOCodeVector.push_back(GPIOD);
				break;
			} else if (!this->checkForPStart()){
				this->ParentMainWindow->err("No Program Start block exists! Please create one!");
				break;
			} else {
				this->ParentMainWindow->err("No Sense HAT Block exists! Please create one!");
				break;
			}	
		}
		default:{
			this->ParentMainWindow->err("Invalid parameter passed!");
			this->ParentMainWindow->warn("The file you have loaded has been incorrectly modified");
			}
	}
}

void DrawArea::paintEvent(QPaintEvent* event){
	QStyleOption* opt = new QStyleOption;
	QPainter* p = new QPainter(this);
	p->setCompositionMode(QPainter::CompositionMode_Source);
    opt->init(this);
	this->style()->drawPrimitive(QStyle::PE_Widget, opt, p, this);
	p->setRenderHint(QPainter::Antialiasing);
	// For each std::pair of points, draw line from p1 to p2;
	for (std::pair<QPoint, QPoint> PointPair : this->Lines){
		p->setPen(QPen(Qt::black, 2));
		QPoint Point1 = std::get<0>(PointPair),
			   Point2 = std::get<1>(PointPair);
		// Upper Mid Point
		QPoint UMidPoint = QPoint(
			(Point1.x() + ((Point2.x() - Point1.x()) / 2)),
			Point1.y()
		);
		QPoint LMidPoint = QPoint(
			(Point1.x() + ((Point2.x() - Point1.x()) / 2)),
			Point2.y()
		);
		p->drawLine(Point1, UMidPoint);
		p->drawLine(UMidPoint, LMidPoint);
		p->drawLine(LMidPoint, Point2);
	};
	p->end();
	event->ignore();
	delete p;
	delete opt;
}

void DrawArea::OnGPIODeviceSignal(int GPIOID){
	this->activeGPIO = GPIOID;
	this->NWMode = true;
}

void DrawArea::resetSelf(){
	this->Lines.clear();
	this->LastPoint = QPoint(0, 0);
	this->setStyleSheet("background-color : #ffffff; background-image : url('static/grid.png');");
	this->isNew = true;
	this->GPIOCodeVector.clear();
	this->BUZVec.clear();
	this->LEDVec.clear();
	this->BTNVec.clear();
	this->LEDCTRLVec.clear();
	this->BUZCTRLVec.clear();
	this->FUNCVec.clear();
	this->FUNCTRLVec.clear();
	this->BTNCTRLVec.clear();
	this->RGBLEDVec.clear();
	this->RGBLEDCTRLVec.clear();
	this->PWMLEDVec.clear();
	this->PWMLEDCTRLVec.clear();
	Counters::reset();
}

void DrawArea::RefreshSelects(){
	QStringList LEDNames,
		BuzzerNames,
		FuncNames,
		FunctionNames,
		ButtonNames,
		RGBLEDNames,
		PWMLEDNames;
	// LEDCtrl Refresh

	for (LED* Led : this->LEDVec){
		LEDNames << Led->VarnameEdit.text();
	}
	for (LEDCtrl* LedControl : this->LEDCTRLVec){
		LedControl->LEDSelect.clear();
		LedControl->LEDSelect.insertItems(0, LEDNames);
		LedControl->LEDSelect.setMaxCount(LedControl->LEDSelect.count());
	}

	// BuzzerCtrl Refresh
	for (Buzzer* Buzzer : this->BUZVec){
		BuzzerNames << Buzzer->VarnameEdit.text();
	}
	for (BuzzerCtrl* BuzControl : this->BUZCTRLVec){
		BuzControl->BuzzerSelect.clear();
		BuzControl->BuzzerSelect.insertItems(0, BuzzerNames);
		BuzControl->BuzzerSelect.setMaxCount(BuzControl->BuzzerSelect.count());
	}

	// Function Refresh
	for (Function* F : this->FUNCVec){
		FuncNames << F->NameEdit.text();
	}

	for (FunctionControl* FCTRL : this->FUNCTRLVec){
		FCTRL->FunctionSelect.clear();
		FCTRL->FunctionSelect.insertItems(0, FuncNames);
		FCTRL->FunctionSelect.setMaxCount(FCTRL->FunctionSelect.count());
	}

	// ButtonControl Refresh
	for (Button* BTN : this->BTNVec){
		ButtonNames << BTN->VarnameEdit.text();
	}

	for (Function* F : this->FUNCVec){
		FunctionNames << F->NameEdit.text();
	}

	for (ButtonControl* BCTRL : this->BTNCTRLVec){
		BCTRL->FunctionSelect.clear();
		BCTRL->ButtonSelect.clear();
		BCTRL->ButtonSelect.insertItems(0, ButtonNames);
		BCTRL->ButtonSelect.setMaxCount(BCTRL->ButtonSelect.count());
		BCTRL->FunctionSelect.insertItems(0, FunctionNames);
		BCTRL->FunctionSelect.setMaxCount(BCTRL->FunctionSelect.count());
	}

	// RGBLEDCTRL Refresh
	for (RGBLED* RGB : this->RGBLEDVec){
		RGBLEDNames << RGB->VarnameEdit.text();
	}

	for (RGBLEDCtrl* RGBC : this->RGBLEDCTRLVec){
		RGBC->RGBLEDSelect.clear();
		RGBC->RGBLEDSelect.insertItems(0, RGBLEDNames);
		RGBC->RGBLEDSelect.setMaxCount(RGBC->RGBLEDSelect.count());
	}

	// PWMLED Refresh
	for (PWMLED* PWM : this->PWMLEDVec){
		PWMLEDNames << PWM->VarnameEdit.text();
	}

	for (PWMLEDCtrl* PWMC : this->PWMLEDCTRLVec){
		PWMC->PWMLEDSelect.clear();
		PWMC->PWMLEDSelect.insertItems(0, PWMLEDNames);
		PWMC->PWMLEDSelect.setMaxCount(PWMC->PWMLEDSelect.count());
	}

	FunctionNames.clear();
	FuncNames.clear();
	ButtonNames.clear();
	LEDNames.clear();
	BuzzerNames.clear();
	RGBLEDNames.clear();
	PWMLEDNames.clear();
}