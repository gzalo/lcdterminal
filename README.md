# lcdterminal
Basic 8051 project to create a basic UART viewer using a 20x4 LCD 

# Hardware

![Schematic](schematic.png)

Proteus-editable Schematic is available in the `lcdterminal.pdsprj` file

To simplify the software, it uses the "external memory" feature of the 8052 to generate the `E` pulses for the `HD44780` LCD controller. That is achieved by defining two variables in `xdata`, at locations `0x0000` and `0x8000` (for `RS=0` and `RS=1` commands).

# Warning
This source code is at least 8 years old. It doesn't reflect modern coding skills, and shouldn't be taken as an example for anything you might be developing today. 
