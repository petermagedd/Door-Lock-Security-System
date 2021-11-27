# Door Lock Security System

# Project Schematic
![Door Lock Security System-1](https://user-images.githubusercontent.com/74562586/143597711-b2d9b4e7-2f56-4928-9f96-526463459150.jpg)


# Layered Arcitecture
![layered](https://user-images.githubusercontent.com/74562586/143593176-2e2f3415-2738-40c9-912d-0c7226bf9602.png)


# Sequence of the Application
At first, the user should enter the desired password 2 times to create his password. The UART of the 1st microcontroller send the password to the UART of 2nd microcontroller and then compare them if they are matched then the EEPROM will save it which this EEPROM is attached with the 2nd microcontroller. If the entered passwords aren’t matched, the user should repeat this operation again until he enters 2 matching passwords. (This is the 1st operation).

After the desired password is saved successfully, the user has to choose between opening the door or changing his current password with new one.

If the user choose to open the door, the user will enter his password again and then the door will open by the DC Motor is rotating for 15 sec clockwise and hold the door open for 3 sec after that the door will begin closing automatically by rotating the DC Motor for 15 sec counterclockwise. If the user entered the password wrongly for 3 times, the buzzer will turn on for 1 min and ERROR state will appear on the LCD.

If the user chooses to change his password, the user will enter his password again and then the user will repeat all the 1st operation again.



# Main Hardware Components
•	2 ATmege16

•	32*2 LCD

•	4*4 Keypad

•	Buzzer

•	Dc Motor

•	M24C16 External EEPROM

•	Resistors

•	L293D

# Implemented Drivers

•	GPIO

•	UART

•	TIMER

•	I2C

•	LCD

•	Keypad

•	Buzzer

•	Dc Motor

•	EEPROM

# Tools

•	Eclipse

•	Proteus




