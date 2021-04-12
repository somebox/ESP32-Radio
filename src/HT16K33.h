// HT16K33.h
// Includes and variables for 14-segment display HT16K33, alhpanumberic 8-digit

#include "Adafruit_GFX.h"
#include "Adafruit_LEDBackpack.h"

Adafruit_AlphaNum4 alphaDisplay1 = Adafruit_AlphaNum4();
Adafruit_AlphaNum4 alphaDisplay2 = Adafruit_AlphaNum4();
#define ALPHA_SIZE 16  // total number of 14-segment characters on the displays
#define ALPHA_BRIGHTNESS 15   // 0-15
#define ALPHA_I2C_ADDRESS_1 0x75
#define ALPHA_I2C_ADDRESS_2 0x73

uint8_t dispdata[ALPHA_SIZE+1];
String scroll_message;
int scroll_offset = 0;

// Color definitions for the TFT screen (if used)
#define BLACK   0
#define BLUE    1
#define RED     1
#define GREEN   1
#define CYAN    GREEN | BLUE
#define MAGENTA RED | BLUE
#define YELLOW  RED | GREEN
#define WHITE   RED | GREEN | BLUE

// Various macro's to mimic the ST7735 version of display functions
#define dsp_setRotation()                                          // Use standard landscape format
#define dsp_setTextSize(a)                                         // Set the text size
#define dsp_setTextColor(a)                              
#define dsp_setCursor(a,b)                                         // Position the cursor
#define dsp_erase()         alphaDisplay1.clear();                                      // Clear the screen
#define dsp_getwidth()      8                                    // Get width of screen
#define dsp_getheight()     1                                      // Get height of screen
#define dsp_usesSPI()       false                                  // Does not use SPI



void* tft = NULL ;


void alpha_write_buffer(){
  uint8_t c = ' ';
  int len = scroll_message.length();
  
  for (int x=ALPHA_SIZE-1; x >= 0; x--){
    if ((scroll_offset + x > len - 1) || (scroll_offset + x < 0)){
      c = ' ';
    } else {
      c = scroll_message.charAt(scroll_offset + x);
    }
    bool dot = (strchr(".?!;",c) != NULL);
    if (x<8){
      alphaDisplay1.writeDigitAscii(x, c, dot);
    } else {
      alphaDisplay2.writeDigitAscii(x%8, c, dot);
    }
  }

  if (len > 16) {
    scroll_offset++;
    if (scroll_offset > len - 1){
      scroll_offset = -16;
    }
  }

  //alphaDisplay2.writeDisplay();
  //delay(10);
  vTaskDelay ( 2 ) ;  
  alphaDisplay1.writeDisplay();
  //delay(10);
  vTaskDelay ( 2 ) ;  
  alphaDisplay2.writeDisplay();
  vTaskDelay ( 2 ) ;  
  alphaDisplay2.writeDisplay();
  delay ( 5 ) ;  
  alphaDisplay2.writeDisplay();
  delay ( 5 ) ;  
  //delay(10);
}

bool dsp_begin()
{
  dbgprint("Start HT16K33", NULL);
  Wire.setClock(400000);
  
  alphaDisplay1.begin(ALPHA_I2C_ADDRESS_1);
  alphaDisplay1.setBrightness(ALPHA_BRIGHTNESS);
  delay(10);

  alphaDisplay2.begin(ALPHA_I2C_ADDRESS_2);
  alphaDisplay2.setBrightness(ALPHA_BRIGHTNESS);

  return true ;
}

void dsp_fillRect ( int16_t x, int16_t y, int16_t w, int16_t h, uint16_t color )
{} ;                                                        // Fill a rectange


#define TFTSECS 1
scrseg_struct     tftdata[TFTSECS] =                        // Screen divided in 3 segments + 1 overlay
{                                                           // One text line is 8 pixels
  { false, WHITE,   0,  8, "" },                            // 1 top line
} ;

void dsp_update(){
  static int counter = 0;
  //dbgprint(" ...>");
  if (counter++ % 2 > 0){
  //  return;
  }
  alpha_write_buffer();
  //dbgprint(" <...");
}


//**************************************************************************************************
//                                      D I S P L A Y B A T T E R Y                                *
//**************************************************************************************************
// Dummy routine for this type of display.                                                         *
//**************************************************************************************************
void displaybattery()
{
}


//**************************************************************************************************
//                                      D I S P L A Y V O L U M E                                  *
//**************************************************************************************************
// Dummy routine for this type of display.                                                         *
//**************************************************************************************************
void displayvolume()
{
}



//**************************************************************************************************
//                                      D I S P L A Y T I M E                                      *
//**************************************************************************************************
// Dummy routine for this type of display.                                                         *
//**************************************************************************************************
void displaytime ( const char* str, uint16_t color )
{
  for (int x=0; x<strlen(str); x++){
    alphaDisplay1.writeDigitAscii(x, str[x]);
  }
}

void alpha_print(String str) {
  if (str.length() == 0) return;
  str.replace("\r","");
  str.replace("\n"," - ");
  dbgprint("  alpha_print(\"%s\")", str.c_str());
  scroll_message = str;
  scroll_offset = 0;
  alpha_write_buffer();
  //alphaDisplay1.setBrightness(ALPHA_BRIGHTNESS);
  //alphaDisplay2.setBrightness(ALPHA_BRIGHTNESS);
  //alphaDisplay1.writeDisplay();
  //alphaDisplay2.writeDisplay();
}

void dsp_print(String str){
  alpha_print(str);
}

void dsp_println(String str){
  alpha_print(str);
}