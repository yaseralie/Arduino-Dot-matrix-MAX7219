//Reference GPIO  https://randomnerdtutorials.com/esp8266-pinout-reference-gpios/
//Display Text to max7219: Static Text, Running Text, and Clock
//Hardware: Dot matrix Max7219, Real Time Clock DS3231
//Using ESP8266

#include <Wire.h>
#include <ds3231.h>

#include <MD_Parola.h>
#include <MD_MAX72xx.h>
#include <SPI.h>

struct ts t;
//Setup for LED Max7219==============================
// Uncomment according to your hardware type
#define HARDWARE_TYPE MD_MAX72XX::FC16_HW
//#define HARDWARE_TYPE MD_MAX72XX::GENERIC_HW

// Defining size, and output pins
#define MAX_DEVICES 4
#define CS_PIN 15
// Hardware SPI connection
MD_Parola Display = MD_Parola(HARDWARE_TYPE, CS_PIN, MAX_DEVICES);

//Variables=========================================
int BUTTON_PIN1 = 12; //button D6
int BUTTON_PIN2 = 0; //button D3
int BUTTON_PIN3 = 2; //button D4
bool status_button1 = false;
bool status_button2 = false;
bool status_button3 = false;
String status_text = "clock";
String time_now = "00:00";
String time_hour = "0";
String time_min = "0";

void setup() {
  Serial.begin(115200);
  Display.begin();
  Display.setIntensity(0);
  Display.displayClear();

  pinMode(BUTTON_PIN1, INPUT_PULLUP);
  pinMode(BUTTON_PIN2, INPUT_PULLUP);
  pinMode(BUTTON_PIN3, INPUT_PULLUP);

  Wire.begin();
  DS3231_init(DS3231_CONTROL_INTCN);
}

void loop() {
  //for RTC=======================================
  DS3231_get(&t);

  if (t.hour < 10)
  {
    time_hour = "0" + String(t.hour);    
  }
  else
  {
    time_hour = String(t.hour); 
  }

  if (t.min < 10)
  {
    time_min = "0" + String(t.min);
  }
  else
  {
    time_min = String(t.min);
  }

  if ( (t.sec % 2) == 0) {
    // Display the current time in 24 hour format with leading zeros enabled and a center colon:
    time_now = time_hour + ":" +  time_min ;
  }
  else {
    time_now = time_hour + " " +  time_min ;
  }


  //check button, if pressed=======================
  int buttonValue1 = digitalRead(BUTTON_PIN1);
  int buttonValue2 = digitalRead(BUTTON_PIN2);
  int buttonValue3 = digitalRead(BUTTON_PIN3);

  //button 1*******************************
  if (buttonValue1 == LOW )
  {
    if (status_button1 == false)
    {
      Serial.println("Show static");
      status_text = "static";
      status_button1 = true;
    }
  }
  else if (buttonValue1 == HIGH)
  {
    if (status_button1 == true)
    {
      status_button1 = false;
    }
  }

  //button 2*******************************
  if (buttonValue2 == LOW )
  {
    if (status_button2 == false)
    {
      Serial.println("Show running");
      status_text = "running";
      status_button2 = true;
    }
  }
  else if (buttonValue2 == HIGH)
  {
    if (status_button2 == true)
    {
      status_button2 = false;
    }
  }

  //button 3*******************************
  if (buttonValue3 == LOW )
  {
    if (status_button3 == false)
    {
      status_text = "clock";
      Serial.println("Show clock");
      status_button3 = true;
    }
  }
  else if (buttonValue3 == HIGH)
  {
    if (status_button3 == true)
    {
      status_button3 = false;
    }
  }


  //Display text to Dot matrix==========================
  if (status_text == "static")
  {
    //Set static text with left alignment
    Display.print("Hello...");
  }
  else if (status_text == "running")
  {
    //Set running text
    if (Display.displayAnimate())
      Display.displayText("Hello World!", PA_LEFT, 100, 100, PA_SCROLL_LEFT, PA_SCROLL_LEFT);
  }
  else if (status_text == "clock")
  {
    Display.setTextAlignment(PA_CENTER);
    Display.print(time_now.c_str());
    delay(500);
  }


}
