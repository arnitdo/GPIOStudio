#include "main.hpp"
#include "config.cpp"

Function::Function(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) :
	GPIODevice(parent, parentMainWindow, X, Y, name),
	SelfLayout(this),
	DisplayLabel(convertToQString(name), this),
	NameLabel("Name : ", this),
	NameEdit(this),
	BodyLabel("Body : ", this),
	BodyButton("Edit Function Body", this){
		this->id = FUNC_ID;
		this->backgroundcolor = convertToQString(Config::FunctionColor.at("background").get<std::string>());
		this->foreground = convertToQString(Config::FunctionColor.at("foreground").get<std::string>());
		this->textcolor = convertToQString(Config::FunctionColor.at("text").get<std::string>());
		QWidget* BodyWindow = new QWidget;
		BodyWindow->setWindowIcon(QIcon("static/icon.png"));
		QTextEdit* FunctionBodyEdit = new QTextEdit(BodyWindow);
		QPushButton* CBody = new QPushButton("Close Window", BodyWindow);
		BodyWindow->setFixedSize(640, 480);
		FunctionBodyEdit->setGeometry(50, 20, 540, 380);
		CBody->setGeometry(270, 425, 100, 25);
		FunctionBodyEdit->setFontPointSize(12);
		FunctionBodyEdit->append("# Write your function code here\n# You do not need to indent the code body\nprint(\"Hello, World!\")\n");
		this->FunctionBodyWindow = BodyWindow;
		this->FunctionBody = FunctionBodyEdit;
		this->CloseBodyButton = CBody;
		this->ParentDrawArea = parent;
		this->ParentMainWindow = parentMainWindow;
		this->XCoord = X;
		this->YCoord = Y;
		this->GPIOName = name;
		DisplayLabel.setFixedSize(180, 20);
		NameLabel.setStyleSheet("border : 0px;");
		BodyLabel.setStyleSheet("border : 0px;");
		BodyLabel.setFixedHeight(20);
		BodyButton.setFixedHeight(20);
		NameEdit.setStyleSheet("background-color : " + this->foreground + ";");
		NameEdit.setText(convertToQString("MyFunction" + std::to_string(Counters::FUNCTIONCount)));
		BodyButton.setStyleSheet("background-color : " + this->foreground + ";");
		this->SelfLayout.addWidget(&DisplayLabel, 0, 1, 1, 2);
		this->SelfLayout.addWidget(&NameLabel, 1, 1);
		this->SelfLayout.addWidget(&NameEdit, 1, 2);
		this->SelfLayout.addWidget(&BodyLabel, 2, 1);
		this->SelfLayout.addWidget(&BodyButton, 2, 2);
		QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT(deleteSelf()));
		QObject::connect(&BodyButton, SIGNAL (clicked()), this, SLOT (showBodyWindow()));
		QObject::connect(CBody, SIGNAL (clicked()), this, SLOT (hideBodyWindow()));
		Counters::FUNCTIONCount++;
}

void Function::showBodyWindow(){
	this->FunctionBodyWindow->setWindowTitle("Edit Function " + this->NameEdit.text());
	this->FunctionBodyWindow->show();
};

void Function::hideBodyWindow(){
	this->FunctionBodyWindow->hide();
};

void Function::deleteSelf(){
	this->ParentMainWindow->log("Deleting " + this->GPIOName + " at - " + std::to_string(this->XCoord) + "," + std::to_string(this->YCoord));
	delete this;
}

json Function::toJson(){
	json DataJSON;
	DataJSON["id"] = FUNC_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	DataJSON["functionName"] = convertToStdString(this->NameEdit.text());
	DataJSON["functionBody"] = convertToStdString(this->FunctionBody->toPlainText());
	return DataJSON;
}

std::string Function::remoteBuild(){
	this->ParentMainWindow->log("Now Building " + this->GPIOName);
	std::string out, templine;
	this->FunctionText.clear();
	this->FunctionText << convertToStdString(this->FunctionBody->toPlainText());
	out = "def " + convertToStdString(this->NameEdit.text()) + "():\n";
	while (std::getline(this->FunctionText, templine)){
		out += "\t" + templine + "\n";
	}
	return out;
}

std::string Function::simpleBuild(){
	return this->remoteBuild();
}

bool Function::validateInput(){
	if (this->NameEdit.text().simplified().isEmpty()){
		this->ParentMainWindow->err("No function name provided for " + this->GPIOName);
		return false;
	}
	return true;
}