#include <PxMatrix.h>

// ESP32 Pins for LED MATRIX
#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15  // NOT USED for 1/16 scan
#define P_OE 2

//set up hardware timing 
hw_timer_t * timer = NULL;

//set board multiplexing 
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

//initialize library for wiring 
PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);

// set draw time var
uint8_t display_draw_time=0;
bool firsttime=true;
int counter=0;

// Set some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);
uint16_t colorlist[] = {myRED,myGREEN,myBLUE,myWHITE,myCYAN,myMAGENTA};

// set up multiplexing interrupt 
void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(display_draw_time);
  portEXIT_CRITICAL_ISR(&timerMux);
}

void display_update_enable(bool is_enable)
{
if (is_enable)
  {
    timer = timerBegin(0, 80, true);
    timerAttachInterrupt(timer, &display_updater, true);
    timerAlarmWrite(timer, 2000, true);
    timerAlarmEnable(timer);
  }
else
  {
    timerDetachInterrupt(timer);
    timerAlarmDisable(timer);
  }
}

void setup() {
  // put your setup code here, to run once:
  display.begin(16); // 1/16 scan
  display.setFastUpdate(true);
  display_update_enable(true);
  display.clearDisplay();
}

//void Text_Write() {
//  display.clearDisplay();
//  // turn off wrap around to allow scrolling
//  display.setTextWrap(false)
//  display.drawLine(0,0,63,0,myYELLOW);
//  display.drawLine(0,31,63,31,myYELLOW);
//  display.setTextColor(myWHITE);
//  display.setCursor(5,7);
//  display.print("Hello");
//  display.setTextColor(myRED);
//  display.setCursor(5,18);
//  display.print("world");
//  display.drawPixel(15, 18, myBLUE);
//  display.drawPixel(15, 18, myRED);
//  delay(50);
//}

void ScrollText() {
  String text = "I am out of office for August 5th-23rd - David Schaub        "; // sample text
  const int width = 18; // width of the marquee display (in characters)
  //display.setTextSize(2); 
  display.setTextColor(myYELLOW);
  // turn off wrap around to allow scrolling
  display.setTextWrap(false);
  // draw border lines
  display.drawLine(0,0,63,0,myRED);
  display.drawLine(0,31,63,31,myRED);

  // Loop once through the string
  for (int offset = 0; offset < text.length(); offset++)  {
    // Construct the string to display for this iteration
    String t = "";
  
    for (int i = 0; i < width; i++) {

      t += text.charAt((offset + i) % text.length());
  
      // Print the string for the current iteration
      display.setCursor(0, display.height()/2-8); // display will be halfway down screen
      display.print(t);
  
      // Short delay so the text doesn't move too fast
      delay(10);
      display.clearDisplay();
      // draw border lines
      display.drawLine(0,0,63,0,myRED);
      display.drawLine(0,31,63,31,myRED);
      }
   }
 
}

void loop() {
  ScrollText();
}
