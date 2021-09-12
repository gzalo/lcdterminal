all:
	sdcc lcd.c -o output/ --opt-code-size 
	packihx output/lcd.ihx > lcd.hex