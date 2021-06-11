#include "main.hpp"
#include "config.cpp"

void GPIODevice::paintEvent(QPaintEvent *)
{
    QStyleOption opt;
    opt.init(this);
    QPainter* p = new QPainter(this);
    this->style()->drawPrimitive(QStyle::PE_Widget, &opt, p, this);
	p->end();
	delete p;
}

GPIODevice::GPIODevice(DrawArea* parent, MainWindow* parentMainWindow, int X, int Y, std::string name) : QWidget(parent){
	this->ParentDrawArea = parent;
	this->ParentMainWindow = parentMainWindow;
	this->XCoord = X;
	this->YCoord = Y;
	this->GPIOName = name;
	QObject::connect(ParentMainWindow, SIGNAL (deleteGPIO()), this, SLOT( deleteSelf()));
}

void GPIODevice::deleteSelf(){
	delete this;
}

json GPIODevice::toJson(){
	json DataJSON;
	DataJSON["id"] = GPIOD_ID;
	DataJSON["x"] = this->XCoord;
	DataJSON["y"] = this->YCoord;
	return DataJSON;
	// Default JSON return
}

std::string GPIODevice::remoteBuild(){
	return "";
}
std::string GPIODevice::simpleBuild(){
	return "";
}
bool GPIODevice::validateInput(){
	return false;
}