#include <at89x52.h>

__xdata __at(0x0000) unsigned char lcdCommandIO; 
__xdata __at(0x8000) unsigned char lcdDataIO; 

void lcdWait(){
	unsigned char n=0;
	while(n != 64) n++;
}

void lcdCommand(unsigned char c){
	lcdCommandIO = c;
	lcdWait();
}

void lcdWrite(char n){
	lcdDataIO = n;	
	lcdWait();
}

void putchar(char n){
	lcdWrite(n);
}

void lcdPuts(char* n){
	while(*n){
		lcdWrite(*n);		
		n++;
	}
}

unsigned char getchar(){   
	while(!RI);
	RI = 0;
	return SBUF;
}   

void lcdInit(){
	unsigned char n;
	for(n=0;n<250;n++)
		lcdWait();

	lcdCommandIO = 0x0C;
	lcdWait();
	lcdCommandIO = 0x06;
	lcdWait();	
	lcdCommandIO = 0x38;
	lcdWait();	
	
}

#define BACKLIGHT P3_1

int main(){
	lcdInit();
	lcdPuts("LCD OK");
	
		
	RCAP2H = 0xFF; // 9600 bps when using 24 MHz
	RCAP2L = 0xB2;
	T2CON = 0x34;
	SCON = 0x52; 

	
	while(1){
		unsigned char r = getchar();
		unsigned char temp;
		if(r == 254){
			unsigned char c = getchar();
			switch(c){
				case 66: 
					break;
				case 70: //backlight off
					// not implemented				
									break;
				case 71:  //set cursor position
					temp = (getchar() - 1);  //get column byte
					switch (getchar())  //get row byte
					{
						//line 1 is already set up
						case 2:
							temp += 0x40;
							break;
						case 3:
							temp += 0x14;
							break;
						case 4:
							temp += 0x54;
							break;
						default:
							break;
					}
					lcdCommand(0b10000000 + temp);
					break;
				case 72:  //cursor home (reset display position)
					lcdCommand(2);
					break;
				case 74:  //show underline cursor
					lcdCommand(0b00001110);
					break;
				case 75:  //underline cursor off
				case 84:  //block cursor off
					lcdCommand(0b00001100);
					break;
				case 76:  //move cursor left
					lcdCommand(16);
					break;
				case 77:  //move cursor right
					lcdCommand(20);
					break;
				case 78:  //define custom char
					lcdCommand(64 + (getchar() * 8));  //get+set char address
					for (temp = 7; temp != 0; temp--)
					{
						putchar(getchar()); //get each pattern byte
					}
					break;
				case 83:  //show blinking block cursor
					lcdCommand(0b00001111);
					break;
				case 86:  //GPO OFF
					//implement later
					break;
				case 87:  //GPO ON
					/*temp = getchar();
					if (temp == 1)
					{
						GPO1 = GPO_ON;
					}*/
					break;
				case 88:  //clear display, cursor home
					lcdCommand(1);
					break;
				case 152: //set and remember (doesn't save value, though)
				case 153: //set backlight brightness
					temp = getchar();
					if(temp>128) BACKLIGHT = 0; else BACKLIGHT = 1;
					break;
				//these commands ignored (no parameters)
				case 35: //read serial number
				case 36: //read version number
				case 55: //read module type
				case 59: //exit flow-control mode
				case 65: //auto transmit keypresses
				case 96: //auto-repeat mode off (keypad)
				case 67: //auto line-wrap on
				case 68: //auto line-wrap off
				case 81: //auto scroll on
				case 82: //auto scroll off
				case 104: //init horiz bar graph
				case 109: //init med size digits
				case 115: //init narrow vert bar graph
				case 118: //init wide vert bar graph
					break;
				default:
					//all other commands ignored and parameter byte discarded
					temp = getchar();  //dump the command code
					break;
			}
		}else{
			putchar(r);
		}
	}
}
