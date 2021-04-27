#Instructions on how to run a generated program -

1)  If you are using Remote Scripts, you do not need to make any modifications.
	Simply enter the raspberry pi's IP address, and press Run Remote

2)	If you are simply building the project, you will need to remove a few lines of unnecessary code. Remove the following lines of code
	i. `remote_pin_factory = gpiozero.pins.pigpio.PiGPIOFactory(host="<Raspberry Pi IP>")`

	ii. This step applies to all GPIO Devices created. Simply remove anything that is similar to -
	`, pin_factory=remote_pin_factory`.

	So, for example, 
	`MyLED1 = gpiozero.LED(21, pin_factory=remote_pin_factory)`
	should be changed to
	`MyLED1 = gpiozero.LED(21)`

##### Note that this is a temporary fix, and a permanent solution is in the working