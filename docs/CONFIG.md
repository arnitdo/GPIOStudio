The config.json file is used to define behaviour and settings of the program

There are currently 3 properties that can be set in the file 

- `defaultPiIP` - The default Raspberry Pi IP to target when running the script remotely

- `defaultSleepTime` - The default number of seconds, set in Sleep Timers

- `legacyMode` - The legacy mode setting changes the number of pins available in the program. Accepted values are `0`, `1` or `2`
	* `0` - Raspberry Pi 1 Model A, Model B (Rev 1.0) `(26 Pin Header)`
	* `1` - Raspberry Pi 1 Model A, Model B (Rev 2.0) `(26 Pin Header + 8 Pin Extra Header)`
	* `2` - Raspberry Pi 2 + (All Models) `(40 Pin Header)`