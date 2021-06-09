#include "main.hpp"

#ifndef GPIOSTUDIO_CONFIG_CPP
#define GPIOSTUDIO_CONFIG_CPP

#define MAJOR_VERSION 0
#define MINOR_VERSION 0
#define REVISION 7

#define GPIOD_ID 0
#define PSTART_ID 1
#define LED_ID 2
#define PWMLED_ID 3
#define RGBLED_ID 4
#define BUZ_ID 5
#define FUNC_ID 6
#define BTN_ID 7
#define LINES_ID 8
#define MOTION_ID 9
#define LIGHTS_ID 10
#define DISTS_ID 11
#define SLEEP_ID 12
#define LEDCTRL_ID 13
#define PWMLEDCTRL_ID 14
#define RGBLEDCTRL_ID 15
#define BUZCTRL_ID 16
#define FCTRL_ID 17
#define BTNCTRL_ID 18

// Function Prototypes
QString getVersionInfo();
QString convertToQString(std::string in);
std::string convertToStdString(QString in);

namespace Counters{
	int BUZZERCount = 1;
	int LEDCount = 1;
	int LEDCTRLCount = 1;
	int BUZZERCTRLCount = 1;
	int SLEEPCount = 1;
	int BUTTONCount = 1;
	int DISTSCount = 1;
	int LIGHTSCount = 1;
	int MOTIONCount = 1;
	int LINESCount = 1;
	int FUNCTIONCount = 1;
	int FUNCTRLCount = 1;
	int BTNCTRLCount = 1;
	int RGBLEDCount = 1;
	int RGBLEDCTRLCount = 1;
	int PWMLEDCount = 1;
	int PWMLEDCTRLCount = 1;
	void reset(){
		BUZZERCount = 1;
		LEDCount = 1;
		LEDCTRLCount = 1;
		BUZZERCTRLCount = 1;
		SLEEPCount = 1;
		BUTTONCount = 1;
		DISTSCount = 1;
		LIGHTSCount = 1;
		MOTIONCount = 1;
		LINESCount = 1;
		FUNCTIONCount = 1;
		FUNCTRLCount = 1;
		BTNCTRLCount = 1;
		RGBLEDCount = 1;
		RGBLEDCTRLCount = 1;
		PWMLEDCount = 1;
		PWMLEDCTRLCount = 1;
	}
}

namespace Config{
	int defaultSleepTime; // Default time in SleepTimer
	int keepaliveSleepTime; // Default time in keepalive
	std::string defaultPiIP; // Default pi IP in Remote
	// Default Colors (Stored as JSON{
	//	"background" : "backgroundcolor",
	// 	"foreground" : "foregroundcolor",
	// "text" : "textcolor"
	// })
	bool overrideVersionWarnings = false;
	json LEDColor,
		PWMLEDColor,
		BuzzerColor,
		LEDCtrlColor,
		PWMLEDCtrlColor,
		BuzzerCtrlColor,
		SleepColor,
		ButtonColor,
		FunctionColor,
		DistanceSensorColor,
		LightSensorColor,
		MotionSensorColor,
		LineSensorColor,
		FunctionCtrlColor,
		ButtonCtrlColor,
		RGBLEDColor,
		RGBLEDCtrlColor,
		ProgramStartColor;
	// End Colors
	int legacyMode;
	/* 
	LEGACY MODE DOC
	Reference - https://elinux.org/RPi_Low-level_peripherals
	legacy mode is defined by an integer - 
	0 - Raspberry Pi Model A, B -  revision 1.0 (26 pin header)
		[0, 1, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 22, 23, 24, 25, 27]
	1 - Raspberry Pi Model A, B - revision 2.0 (26 pin + 8 pin extra (4 GPIO Extra))
		[2, 3, 4, 7, 8, 9, 10, 11, 14, 15, 17, 18, 21, 22, 23, 24, 25, 28 ,29, 30, 31]
	2 - Raspberry Pi Model 2, 2B+ .... (Full 40 Pins)
		[2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 , 21, 22, 23, 24, 25, 26, 27]
	 */
	void resetConfig(){
		defaultSleepTime = 5;
		keepaliveSleepTime = 1; 
		defaultPiIP = ""; 
		LEDColor = {
			{"background", "#00ffff"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		PWMLEDColor = {
			{"background", "#69b5e2"},
			{"foreground", "#4ae4f4"},
			{"text", "#000000"}
		},
		BuzzerColor = {
			{"background", "#98fb98"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		LEDCtrlColor = {
			{"background", "#ffb473"},
			{"foreground", "#fffa73"},
			{"text", "#000000"}
		},
		PWMLEDCtrlColor = {
			{"background", "#abe587"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		BuzzerCtrlColor = {
			{"background", "#cacdff"},
			{"foreground", "#cae8ff"},
			{"text", "#000000"}
		},
		SleepColor = {
			{"background", "#fedf00"},
			{"foreground", "#defe00"},
			{"text", "#000000"}
		},
		ButtonColor = {
			{"background", "#aadf0a"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		} ,
		FunctionColor = {
			{"background", "#00cc99"},
			{"foreground", "#ccffd3"},
			{"text", "#000000"}
		},
		DistanceSensorColor = {
			{"background", "#bccc38"},
			{"foreground", "#cceecc"},
			{"text", "#000000"}
		},
		LightSensorColor = {
			{"background", "#00cc99"},
			{"foreground", "#00CC33"},
			{"text", "#000000"}
		},
		MotionSensorColor = {
			{"background" , "#a2d0d7"},
			{"foreground" , "#a1d6c3"},
			{"text", "#000000"}
		},
		LineSensorColor = {
			{"background", "#98a20b"},
			{"foreground", "#4da30b"},
			{"text", "#000000"}
		},
		FunctionCtrlColor =  {
			{"background", "#fedbf8"},
			{"foreground", "#fedbe7"},
			{"text", "#000000"}
		},
		ButtonCtrlColor = {
			{"background", "#8da3f3"},
			{"foreground", "#8dd6f3"},
			{"text", "#000000"}
		},
		RGBLEDColor = {
			{"background", "#f7957a"},
			{"foreground", "#dbf779"},
			{"text", "#000000"}
		},
		RGBLEDCtrlColor = {
			{"background", "#9acd32"},
			{"foreground", "#dbf779"},
			{"text", "#000000"}
		},
		ProgramStartColor = {
			{"background", "#aaaaaa"},
			{"foreground", "#aaaaaa"},
			{"text", "#000000"}
		};
	// End Colors
	legacyMode = 2;		
	}
	void LoadConfig(MainWindow* MainWin){
		resetConfig();
		std::ifstream configFile;
		configFile.open("config.json", std::ios::in);
		if (!configFile.is_open()){
			// Revert to defaults
			MainWin->err("Error opening config.json file!");
			MainWin->err("Falling back to default configuration");
			resetConfig();
			configFile.close();
		} else {
			// Load Config from JSON File
			json JsonConfigData;
			configFile >> JsonConfigData;
			try {
				defaultPiIP = JsonConfigData.at("defaultPiIP").get<std::string>();
				defaultSleepTime = JsonConfigData.at("defaultSleepTime").get<int64_t>();
				keepaliveSleepTime = JsonConfigData.at("keepaliveSleepTime").get<int64_t>();
				legacyMode = JsonConfigData.at("legacyMode").get<int64_t>();
				overrideVersionWarnings = JsonConfigData.at("overrideVersionWarnings").get<bool>();
				LEDColor = JsonConfigData.at("colors").at("LED");
				PWMLEDColor = JsonConfigData.at("colors").at("PWMLED");
				BuzzerColor = JsonConfigData.at("colors").at("Buzzer");
				LEDCtrlColor = JsonConfigData.at("colors").at("LEDCtrl");
				PWMLEDCtrlColor = JsonConfigData.at("colors").at("PWMLEDCtrl");
				BuzzerCtrlColor = JsonConfigData.at("colors").at("BuzzerCtrl");
				SleepColor = JsonConfigData.at("colors").at("Sleep");
				ButtonColor = JsonConfigData.at("colors").at("Button");
				FunctionColor = JsonConfigData.at("colors").at("Function");
				DistanceSensorColor = JsonConfigData.at("colors").at("DistanceSensor");
				LineSensorColor = JsonConfigData.at("colors").at("LineSensor");
				MotionSensorColor = JsonConfigData.at("colors").at("MotionSensor");
				LightSensorColor = JsonConfigData.at("colors").at("LightSensor");
				FunctionCtrlColor = JsonConfigData.at("colors").at("FunctionControl");
				ButtonCtrlColor = JsonConfigData.at("colors").at("ButtonControl");
				RGBLEDColor = JsonConfigData.at("colors").at("RGBLED");
				RGBLEDCtrlColor = JsonConfigData.at("colors").at("RGBLEDCtrl");
				ProgramStartColor = JsonConfigData.at("colors").at("ProgramStart");
			} catch (json::out_of_range& JSONExcept){
				if (JSONExcept.id == 403){
					MainWin->err("Unable to set configuration. config.json file is invalid");
					MainWin->err("Falling back to default configuration");
					resetConfig();
				}
			}
			if (overrideVersionWarnings) MainWin->warn("Overriding version warnings. This may cause undefined behaviour!");
			MainWin->RaspiIPEdit.setText(convertToQString(defaultPiIP));
			configFile.close();
		}
	}
}

QString getVersionInfo(){
	// Returns "MAJOR_VERSION.MINOR_VERSION.REVISION"
	std::string VInfo = std::to_string(MAJOR_VERSION) + "." + std::to_string(MINOR_VERSION) + "." + std::to_string(REVISION);
	return QString(VInfo.c_str());
}

QString convertToQString(std::string in){
	// Converts std::string to QString
	return QString(in.c_str());
}

std::string convertToStdString(QString in){
	// Vice Versa, converts QString to std::string
	return in.toUtf8().constData();
}

#endif