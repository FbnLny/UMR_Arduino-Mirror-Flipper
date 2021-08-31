# UMR_Arduino-Mirror-Flipper
Arduino-Code to drive a stepper motor and act as a mouse to trigger actions on a PC, when requested through digital inputs.


The circuit is built to interface with Raspberry Pi GPIOs

It will start homing the motor, if that is unwanted, jump the homing-pin and GND

The homing switch has to be a normally open type, the homing pin is internally pulled high

!!! currently there is no homing-switch implemented !!!

!!! therefore homing should not be used at the moment, as the last motor position is stored in the EEPROM and the motor returns home automatically after a reset or power cycle !!!

!!! homing-pin shoud be shorted to GND !!!


A rising edge on the input-pin triggers one 90° movement of the stepper motor, a second rising edge will move the motor back into the "home" position, the cycle then repeats

after each cycle, the uC will perform a mouse press and release on the pc where it is connected to

While the motor is moving, the busy-pin will output a high value

The output-pin will indicate the current position of the motor, low being the home position, high being the other position, the output-pin value is only trustworty when busy-pin is low



the board provides 2 GPIOs they are connected to Arduino-pins D11 and D13 (see pinout), both are PWM capable, the pcb provides footprints for low pass filters, to enable analog outputs,
those are currently bridged

it also features 2 analog inputs, Arudino-pins A0 and A2, both having 10 bit ADCs. these pins can also be used as digital IO D14 and D16

as the board is flipped, the reset putton is not available, the pin is broken out as the RST jumper, the board will reset when both pins are shorted

the uC will act as a mouse on the connected pc, this can interfere with serial programming, in case there are problems, remove the PROG_jumper, reset the board and try again.
when the jumper is removed and the board reset, it will not pose as a mouse on the host computer and ignore all inputs, the busy-pin will be high constantly.

to put the board back into normal operating mode, reseat the jumper and reset the board

9-12V input volatge is required for the stepper motor driver.


comments on the code will be added



Digital voltages

All inputs are 3.3V and 5V tollerant

All outputs are 5V with a series 10k resistor to limit maximum current to 0.5mA, therefore protecting the Raspberry Pis 3.3V logic


