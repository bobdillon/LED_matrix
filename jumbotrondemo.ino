///////////////////////////////////////////////////////
//
//          >>>>  JUMBOTRON demo  <<<<
//
//
// Exercise the hardware from HackerBox #0036
//         which is avaiable at www.HackerBoxes.com
//
// Based on the PxMatrix library found here:
// https://github.com/2dom/PxMatrix
// The PxMatrix library is required for this dmeo.
//
///////////////////////////////////////////////////////
#include <PxMatrix.h>

// ESP32 Pins for JoyStick
#define JOY_X 35
#define JOY_Y 34
#define JOY_Z 26
#define KEY_1 27
#define KEY_2 32
#define KEY_3 33
#define KEY_4 25

// ESP32 Pins for LED MATRIX
#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15  // NOT USED for 1/16 scan
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

uint8_t display_draw_time=0;
 int numberofdots= random(0,64);
 bool firsttime=true;
 int counter=0;

struct Piece{
  int size[8][8];
  int x;
  int y;
};


 
PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);

int key_state=4;

// Some standard colors
uint16_t myRED = display.color565(255, 0, 0);
uint16_t myGREEN = display.color565(0, 255, 0);
uint16_t myBLUE = display.color565(0, 0, 255);
uint16_t myWHITE = display.color565(255, 255, 255);
uint16_t myYELLOW = display.color565(255, 255, 0);
uint16_t myCYAN = display.color565(0, 255, 255);
uint16_t myMAGENTA = display.color565(255, 0, 255);
uint16_t myBLACK = display.color565(0, 0, 0);

uint16_t colorlist[] = {myRED,myGREEN,myBLUE,myWHITE,myCYAN,myMAGENTA};


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

void setup() 
{
  pinMode(JOY_Z, INPUT_PULLUP);
  pinMode(KEY_1, INPUT_PULLUP);
  pinMode(KEY_2, INPUT_PULLUP);
  pinMode(KEY_3, INPUT_PULLUP);
  pinMode(KEY_4, INPUT_PULLUP);      
  display.begin(16); // 1/16 scan
  display.setFastUpdate(true);
  display_update_enable(true);
}

void loop() 
{
  read_keybuttons();
  switch(key_state)
  {
    case 1:
      snow();
      break;
    case 2:
      matrix();
      //color_lines();
      break;
     case 3:
      stick_demo();
      break;
     case 4:
      HB_Jumbo_Text();
      break;
     //case 5:
      //Tetris();
      //break;
      
  }
}

void read_keybuttons()
{
  if (digitalRead(KEY_2) == LOW)
    key_state = 2;
  else 
  if (digitalRead(KEY_1) == LOW)
    key_state = 1;
  else
  if (digitalRead(KEY_3) == LOW)
    key_state = 3;
  else
  if (digitalRead(KEY_4) == LOW)
    key_state = 4;
  else
  if (digitalRead(JOY_Z) == LOW)
    key_state = 5;
    
}

void stick_demo()
{
  display.clearDisplay();
  int x, y;
  uint16_t joy_color;
  x=(63*analogRead(JOY_X))/4096;
  y=30-((31*analogRead(JOY_Y))/4096);
  if (digitalRead(JOY_Z) == LOW)
    joy_color = myRED;
  else
    joy_color = myGREEN;
  display.drawPixel(x,   y,   joy_color);
  display.drawPixel(x,   y+1, joy_color);
  display.drawPixel(x+1, y,   joy_color);
  display.drawPixel(x+1, y+1, joy_color);
  delay(50);
}

void color_lines()
{
  display.clearDisplay();
  int x1,x2,y1,y2;
  for (int n=0; n < 2; n++) 
  {
    x1=random(0,64);
    y1=random(0,32);
    x2=random(0,64);
    y2=random(0,32);
    display.drawLine(x1,y1,x2,y2,myRED);
    delay(30);
    x1=random(0,64);
    y1=random(0,32);
    x2=random(0,64);
    y2=random(0,32);
    display.drawLine(x1,y1,x2,y2,myGREEN);
    delay(30);
    x1=random(0,64);
    y1=random(0,32);
    x2=random(0,64);
    y2=random(0,32);
    display.drawLine(x1,y1,x2,y2,myBLUE);
  }
  
  delay(30);
}
void matrix()
{
   display.clearDisplay();
 if(firsttime==true)
 {
  numberofdots= random(0,64);
  counter=0;
  firsttime=false;
 }
  for(int i=0;i<numberofdots;i++)
  {
   
     display.drawPixel( random(0,64),counter,myGREEN );
  }
  counter++;
  if(counter==32)
  {
    firsttime=true;
  }
   delay(45);
}


void color_blocks()
{
  display.clearDisplay();
  int x,y,r,g,b;
  for (int n=0; n < 25; n++) // 25 2x2 squares at a time
  {
    x=random(0,63);
    y=random(0,31);
    r=random(0,256);
    g=random(0,256);
    b=random(0,256);
    display.drawPixel(x,   y,   display.color565(r,g,b));
    display.drawPixel(x,   y+1, display.color565(r,g,b));
    display.drawPixel(x+1, y,   display.color565(r,g,b));
    display.drawPixel(x+1, y+1, display.color565(r,g,b));
  }
  delay(50);
}

void snow() 
{
  display.clearDisplay();
  for (int n=0; n < 100; n++) // 100 LEDs on at a time
  {
    display.drawPixel(random(0,64), random(0,32), colorlist[random(0,5)]);
  }
  delay(10);
}

void HB_Jumbo_Text()
{
  display.clearDisplay();
  display.drawLine(0,0,63,0,myYELLOW);
  display.drawLine(0,31,63,31,myYELLOW);
  display.drawLine(0,1,0,30,myBLUE);
  display.drawLine(63,1,63,30,myBLUE);
  display.setTextColor(myWHITE);
  display.setCursor(5,7);
  display.print("Let's get");
  display.setTextColor(myRED);
  display.setCursor(5,18);
  display.print("dinner");
  display.drawPixel(15, 18, myBLUE);
  display.drawPixel(15, 18, myRED);
  delay(50);
}
void Tetris()
{
 display.clearDisplay();
}
