#include "mainwindow.cpp"

 GPIOToolBar::GPIOToolBar(QWidget* parent, MainWindow* parentMainWindow) : 
 	QWidget(parent),
	GPIOToolBarLayout(this){
	this->ParentMainWindow = parentMainWindow;
	this->MainWindowDrawArea = &this->ParentMainWindow->MainWindowDrawArea;
	GPIOToolBarLayout.setSpacing(0);
	GPIOToolBarLayout.setMargin(0);
	std::map<int, std::string> ButtonIconMap;
	ButtonIconMap.insert({1, "debug-start.svg"});
	ButtonIconMap.insert({2, "led-variant-on.svg"});
	ButtonIconMap.insert({3, "wave-square-solid.svg"});
	ButtonIconMap.insert({4, "rgbled.png"});
	ButtonIconMap.insert({5, "buzzer.png"});
	ButtonIconMap.insert({6, "function.svg"});
	ButtonIconMap.insert({7, "button.png"});
	for (int i = 1; i < (int)this->ParentMainWindow->MainWindowDrawArea.ButtonLabelMap.size() + 1; i++){
		GPIOButton* GPIOSelectButton = new GPIOButton(convertToQString("  " + this->ParentMainWindow->MainWindowDrawArea.ButtonLabelMap.at(i)), i,  this, this->ParentMainWindow);
		GPIOSelectButton->setFixedSize(232, 36);
		GPIOSelectButton->setIcon(QIcon(
			convertToQString("static/toolbar/" + ButtonIconMap[i])
		));
		GPIOToolBarLayout.addWidget(GPIOSelectButton);
		QObject::connect(GPIOSelectButton, SIGNAL (GPIOButtonPressed(int)), &this->ParentMainWindow->MainWindowDrawArea, SLOT(OnGPIODeviceSignal(int)));
	}
	GPIOToolBarLayout.setAlignment((Qt::AlignTop | Qt::AlignHCenter));
	this->setFixedSize(232, 720);
 };

GPIOButton::GPIOButton(QString label, int GPIOID, GPIOToolBar* mainWindowGPIOToolBar, MainWindow* parentMainWindow)
	: QPushButton(label, mainWindowGPIOToolBar){
	this->ParentGPIOToolBar = mainWindowGPIOToolBar;
	this->ParentMainWindow = parentMainWindow;
	this->GPIOID = GPIOID;
	QObject::connect(this, SIGNAL (clicked()), this, SLOT(SelfPressed()));
}

void GPIOButton::SelfPressed(){
	emit GPIOButtonPressed(this->GPIOID);
}