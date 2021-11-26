# Door Lock Security System

# Project Schematic
![Door Lock Security System-1](https://user-images.githubusercontent.com/74562586/143597711-b2d9b4e7-2f56-4928-9f96-526463459150.jpg)


# Layered Arcitecture
![layered](https://user-images.githubusercontent.com/74562586/143593176-2e2f3415-2738-40c9-912d-0c7226bf9602.png)


# Sequence of the Application
1)First time to the program you must set the Application password first, you should enter the desired password second, you should enter the same password again to confirm it:

    a)If the 2 passwords are matched, the password is saved, and you go the main options

    b)Else you should repeat this operation until complete setting password


2)Main Options of the program you can select from 2 options:

    a)Open The Door --> to select this, press '-' then press 'enter'

    b)Change Application Password --> to select this, press '+' then press 'enter'


3)Checking the password after selecting an option, you should enter the password to check if you are the owner or not:

    a)If you enter password correctly your option will execute:

      i)If the option open the door, the door will be opening then hold for some time then closing. during this operation the state of door will show in the LCD then you will go         to the main options

      ii)If the option change the password you should set password again as the first time you visit the program then you will go to the main options


    b)	if you enter password wrongly:

      i)	if the password is entered three times incorrect, the buzzer will turn on for 1 minute and showing ERROR state on the LCD and then you will go to the main options




# Main Hardware Components
•	2 ATmege16

•	32*2 LCD

•	4*4 Keypad

•	Buzzer

•	Dc Motor

•	M24C16 External EEPROM

•	Resistors

•	L293D


