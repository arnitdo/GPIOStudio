The config.json file is used to define behaviour and settings of the program

It is highly recommended to not change any value, unless you know what you are doing. An `example.config.json` file has been provided, you may rename it to `config.json` and use the program.

Do note, that the program will not function correctly if a `config.json` file is not provided

There are currently 5 properties that can be set in the file 

- `defaultPiIP` - `string` - The default Raspberry Pi IP to target when running the script remotely

- `defaultSleepTime` - `int` - The default number of seconds, set in Sleep Timers

- `keepaliveSleepTime` - `int` - The default number of seconds to sleep, when script is in keepalive mode

- `legacyMode` - `int` - The legacy mode setting changes the number of pins available in the program. Accepted values are `0`, `1` or `2`
	* `0` - Raspberry Pi 1 Model A, Model B (Rev 1.0) `(26 Pin Header)`
	* `1` - Raspberry Pi 1 Model A, Model B (Rev 2.0) `(26 Pin Header + 8 Pin Extra Header)`
	* `2` - Raspberry Pi 2 + (All Models) `(40 Pin Header)`

- `colors` - `json` - The colors setting is a collection of hexadecimal color codes for code blocks. Each type of block has 3 settings -
	- `background` - The background color of the code block
	- `foreground` - The background color of dropdowns, input fields, etc.
	- `text` - The color of the text for the entire code block
	
	Feel free to change any colors, if you have difficulty reading text.
 