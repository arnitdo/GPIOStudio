#include "main.hpp"
#include "config.cpp"

/* DOC
After program has finished executing, it will go into a keepalive state
Various print statements, such as distance sensor print statements will be executed in the keepalive loop
Thus, the user will actually be able to see the output
*/
ProgramStart::ProgramStart(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X,Y, name),
	DisplayLabel(this),
	SelfLayout(this){
		this->id = PSTART_ID;
		DisplayLabel.setText(convertToQString(name));
		DisplayLabel.setFixedWidth(200);
		this->ParentDrawArea = parent;
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		QObject::connect(&(this->ParentMainWindow->RemoteRunButton), SIGNAL (clicked()), this, SLOT (TriggerRemoteBuild()));
		QObject::connect(this, SIGNAL (buildCompleted()), this->ParentMainWindow, SLOT (runRemote()));
		QObject::connect(&(this->ParentMainWindow->MainWindowBuildButton), SIGNAL (clicked()), this, SLOT (TriggerSimpleBuild()));
}

void ProgramStart::deleteSelf(){
	delete this;
}

json ProgramStart::toJson(){
	json DataJSON;
	DataJSON["id"] = PSTART_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	return DataJSON;
}

bool ProgramStart::validateInput(){
	return true;
}

bool ProgramStart::validateCode(){
	for (GPIODevice* GPIOD : this->ParentDrawArea->GPIOCodeVector){
		if (GPIOD->validateInput() == false){
			return false;
			// If any GPIO Device reports invalid input, return false immediately
		}
	}
	return true;
}

std::string ProgramStart::remoteBuild(){
	return  "# script.py generated by " + convertToStdString("GPIO Studio v" + getVersionInfo()) + "\n"
			"import gpiozero\n"
			"import time\n"
			"__remote_pin_factory = gpiozero.pins.pigpio.PiGPIOFactory(host=\"" + convertToStdString(this->ParentMainWindow->RaspiIPEdit.text()) + "\")\n";
}

std::string ProgramStart::simpleBuild(){
	return  "# script.py generated by " + convertToStdString("GPIO Studio v" + getVersionInfo()) + "\n"
			"import gpiozero\n"
			"import time\n"
			"import sense_hat\n"
			"import picamera\n";
}

void ProgramStart::TriggerRemoteBuild(){
	this->ParentMainWindow->log("Validating Project!");
	if (this->validateCode()){
		this->ParentMainWindow->log("Program Validated Succesfully! No errors found!");
		this->ParentMainWindow->log("Building Project, Config : Remote!");
		// Main Code start
		this->ParentDrawArea->LoopCodeVector.clear();
		std::ofstream outfile ("script.py"); // Open File 
		for (GPIODevice* GPD : this->ParentDrawArea->GPIOCodeVector)
		{
			outfile << GPD->remoteBuild(); // Write each GPIO Device's build to file
		}
		outfile << "\n";
		// Main Code End
		// Keepalive code start
		outfile << "print(\"The program will now run in 'keepalive' mode.\")\n";
		outfile << "print(\"I.e, you can stop it at any time by pressing CTRL + C\")\n";
		outfile << "print(\"All actions such as Button Actions, Distance Measurements, etc will work.\")\n";
		outfile << "while True:\n";
		outfile << "\ttry:\n";
		for (std::string lcstring : this->ParentDrawArea->LoopCodeVector){
			outfile << "\t\t" << lcstring;
		}
		outfile << "\t\ttime.sleep(" + std::to_string(Config::keepaliveSleepTime) + ")\n";
		outfile << "\texcept KeyboardInterrupt:\n";
		outfile << "\t\timport sys\n";
		outfile << "\t\tsys.exit()\n";
		// Script no longer errs when interrupted by keyboard
		outfile.close();
		// Keepalive code end;
		this->ParentMainWindow->log("Finished Building!");
		// Emit buildcompleted signal
		// MainWindow will run program only after build is completed
		// Could have caused errors with large project files
		emit buildCompleted();
	} else {
		this->ParentMainWindow->warn("Invalid inputs found! Check console!");
	}
}

void ProgramStart::TriggerSimpleBuild(){
	this->ParentMainWindow->log("Validating Project!");
	if (this->validateCode()){
		this->ParentMainWindow->log("Program Validated Succesfully! No errors found!");
		this->ParentMainWindow->log("Building Project, Config : Local!");
		// Main Code start
		this->ParentDrawArea->LoopCodeVector.clear();
		std::ofstream outfile ("script.py"); // Open File 
		for (GPIODevice* GPD : this->ParentDrawArea->GPIOCodeVector)
		{
			outfile << GPD->simpleBuild(); // Write each GPIO Device's build to file
		}
		outfile << "\n";
		// Main Code End
		// Keepalive code start
		outfile << "print(\"The program will now run in 'keepalive' mode.\\nI.e, you can stop it at any time by pressing CTRL + C\\nAll actions such as Button Actions, Distance Measurements, etc will work.\")\n";
		outfile << "while True:\n";
		outfile << "\ttry:\n";
		for (std::string lcstring : this->ParentDrawArea->LoopCodeVector){
			outfile << "\t\t" << lcstring;
		}
		outfile << "\t\ttime.sleep(" + std::to_string(Config::keepaliveSleepTime) + ")\n";
		outfile << "\texcept KeyboardInterrupt:\n";
		outfile << "\t\timport sys\n";
		outfile << "\t\tsys.exit()\n";
		// Script no longer errs when interrupted by keyboard
		outfile.close();
		// Keepalive code end;
		this->ParentMainWindow->log("Finished Building!");
	} else {
		this->ParentMainWindow->warn("Invalid inputs found! Check console!");
	}
}