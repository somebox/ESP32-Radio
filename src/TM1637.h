// TM1637.h
// Includes and variables for 7-segment display TM1637

#include <TM1637Display.h>
// https://github.com/avishorp/TM1637

// Module connection pins (Digital Pins)
#define TMCLK 25
#define TMDIO 26

TM1637Display clockDisplay(TMCLK, TMDIO);
uint8_t clockstr[4]; // array for TM1637
int TMBRIGHT = 2; // default medium brightness (possible values: 0-7)
bool TMSTATE = true; // time will only be updated if this is set to true
bool TMCLOCK = true; // module could be active but we could use commands to update the display (not implemented)
bool TMDOT = true; // central colon is initially off, can be updated via command

// Color definitions for the TFT screen (if used)
#define BLACK   0
#define BLUE    1
#define RED     1
#define GREEN   1
#define CYAN    GREEN | BLUE
#define MAGENTA RED | BLUE
#define YELLOW  RED | GREEN
#define WHITE   RED | GREEN | BLUE


void showclock ( const char* str)
{
  if ( TMCLOCK )
  {
  clockstr[0] = clockDisplay.encodeDigit(str[0]);
  if ( TMDOT && ((int) str[7] % 2 == 0))
  {
  clockstr[1] = clockDisplay.encodeDigit(str[1]) | B10000000;
  } else {
  clockstr[1] = clockDisplay.encodeDigit(str[1]);  
  }
  
  clockstr[2] = clockDisplay.encodeDigit(str[3]);
  clockstr[3] = clockDisplay.encodeDigit(str[4]);
  clockDisplay.setSegments(clockstr);
  clockDisplay.setBrightness(TMBRIGHT);
  }
}
