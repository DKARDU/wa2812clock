#include <Adafruit_NeoPixel.h>
#include <Wire.h>
#include "DHT.h"
#include "RTClib.h"
RTC_DS3231 RTC;
#define DHTPIN A0
#define DHTTYPE DHT11 
DHT dht(DHTPIN, DHTTYPE);
#define PIN  7
#define POT_PIN  8
#define BTN  3
#define button 2
#define dataPin 8 
uint8_t NUMPIXELS = 4; //led number of pot_pin 
uint8_t NUM_DIGIT = 4;

const uint8_t LEDS_PER_DIGIT = 35;
uint16_t NUM_LEDS = LEDS_PER_DIGIT * NUM_DIGIT;//35*4=140

uint8_t time_h = 0;
uint8_t time_m = 0;
uint8_t time_s = 0;
int num = 0;
int check = 0;
boolean oldState = HIGH;
int Temperature = 0;
float ambiant_light = 1.0;
bool mask_digit[13][35] =
{
  { //0
    1, 1, 1, 1, 1,   //E
    1, 1, 1, 1, 1,   //F
    1, 1, 1, 1, 1,   //A
    1, 1, 1, 1, 1,   //B
    0, 0, 0, 0, 0,   //G
    1, 1, 1, 1, 1,   //D
    1, 1, 1, 1, 1    //C
  },
  { //1 
    0, 0, 0, 0, 0,   //E
    0, 0, 0, 0, 0,   //F
    0, 0, 0, 0, 0,   //A
    1, 1, 1, 1, 1,   //B
    0, 0, 0, 0, 0,   //G
    0, 0, 0, 0, 0,   //D
    1, 1, 1, 1, 1    //C
  },
  { //2
    1, 1, 1, 1, 1,  //E
    0, 0, 0, 0, 0,  //F
    1, 1, 1, 1, 1,  //A
    1, 1, 1, 1, 1,  //B
    1, 1, 1, 1, 1,  //G
    1, 1, 1, 1, 1,  //D
    0, 0, 0, 0, 0   //C
  },
  { //3
    0, 0, 0, 0, 0,  //E
    0, 0, 0, 0, 0,  //F
    1, 1, 1, 1, 1,  //A
    1, 1, 1, 1, 1,  //B
    1, 1, 1, 1, 1,  //G
    1, 1, 1, 1, 1,  //D
    1, 1, 1, 1, 1   //C
  },
  { //4
    0, 0, 0, 0, 0,   //E
    1, 1, 1, 1, 1,   //F
    0, 0, 0, 0, 0,   //A
    1, 1, 1, 1, 1,  //B
    1, 1, 1, 1, 1,   //G
    0, 0, 0, 0, 0,  //D
    1, 1, 1, 1, 1    //C
  },
  { //5
    0, 0, 0, 0, 0,   //E
    1, 1, 1, 1, 1,   //F
    1, 1, 1, 1, 1,   //A
    0, 0, 0, 0, 0,   //B
    1, 1, 1, 1, 1,   //G
    1, 1, 1, 1, 1,   //D
    1, 1, 1, 1, 1    //C
  },
  { //6
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
  },
  { //7
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,  //A
    1, 1, 1, 1, 1,  //B
    0, 0, 0, 0, 0,
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1   //C
  },
  { //8
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
  },
  {  //9
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1
  },

  {  //o
    0, 0, 0, 0, 0,  //E
    1, 1, 1, 1, 1,  //F
    1, 1, 1, 1, 1,  //A
    1, 1, 1, 1, 1,  //B
    1, 1, 1, 1, 1,  //G
    0, 0, 0, 0, 0, // D
    0, 0, 0, 0, 0   //C
  },

   {  //C
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 0,      
    0, 0, 0, 0, 0,
    1, 1, 1, 1, 1,
    0, 0, 0, 0, 0
  },

   {  //F
    1, 1, 1, 1, 1,  //E
    1, 1, 1, 1, 1,  //F
    1, 1, 1, 1, 1,  //A
    0, 0, 0, 0, 0,  //B
    1, 1, 1, 1, 1,  //G
    0, 0, 0, 0, 0,  //D
    0, 0, 0, 0, 0   //C
  },
};
Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, PIN, NEO_GRB + NEO_KHZ800);
Adafruit_NeoPixel pixels(NUMPIXELS, POT_PIN, NEO_GRB + NEO_KHZ800);

byte colors[3][3] = { 
  {0xff, 0, 0},
  {0xff, 0xff, 0xff},
  {0   , 0   , 0xff}
};

void setup()
{
  Serial.begin(9600);
  Wire.begin();
  RTC.begin();
  if (RTC.lostPower())
  {
    Serial.println("RTC is NOT running!");
    // Following line sets the RTC to the date & time this sketch was compiled
    // RTC.adjust(DateTime(__DATE__, __TIME__));
   //  RTC.adjust(DateTime(2022, 1, 5, 11, 25, 0));
  }
  strip.begin(); 
  pixels.begin();
  strip.show(); // Initialize all pixels to 'off'
  pinMode(BTN, INPUT);
  pinMode(button,INPUT_PULLUP);
}

void loop()
{
 Temperature = dht.readTemperature();
  Serial.println(Temperature);
  DateTime now = RTC.now();
  time_h = now.hour();
  time_m = now.minute();
  time_s = now.second();
  int digit_4 = time_h / 10; //10 states, 0-9
  int digit_3 = time_h % 10; //10 states, 0-9
  int digit_2 = time_m / 10; //10 states, 0-9
  int digit_1 = time_m % 10; //10 states, 0-9
//    Serial.print(digit_4);
//    Serial.print(digit_3);
//    Serial.print(digit_3); 
//    Serial.print(" : ");
//    Serial.print(digit_2);
//    Serial.println(digit_1);
    Serial.print(time_s);
    Serial.println(" s");
  
  int digit_5 = Temperature / 10;
  int digit_6 = Temperature % 10;

  int Fahrenheit = Temperature * 1.8 + 32;
  int digit_7 = Fahrenheit / 10;
  int digit_8 = Fahrenheit % 10;


  byte *c;
  bool map_num[NUM_LEDS] = {};//NUM_LEDS = 140;
   while(time_s >= 0 && time_s < 40){

  for (int i = 0; i < LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_4][i];
  }
  for (int i = LEDS_PER_DIGIT; i < 2 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_3][i - LEDS_PER_DIGIT];
  }
  for (int i = 2 * LEDS_PER_DIGIT; i < 3 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_2][i - 2 * LEDS_PER_DIGIT];
  }
  for (int i = 3 * LEDS_PER_DIGIT; i < 4 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_1][i - 3 * LEDS_PER_DIGIT];
  }
  
  for(int i=0; i<NUMPIXELS;i++) { // For each pixel...

  pot_led_on(0,3);
  }
 
  break;

   }

  //========================================
   while(time_s < 50 && time_s >= 40)
 {
  for (int i = 0; i < LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_5][i];
  }
  for (int i = LEDS_PER_DIGIT; i < 2 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_6][i - LEDS_PER_DIGIT];
  }
  for (int i = 2 * LEDS_PER_DIGIT; i < 3 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[10][i - 2 * LEDS_PER_DIGIT];
  }
  for (int i = 3 * LEDS_PER_DIGIT; i < 4 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[11][i - 3 * LEDS_PER_DIGIT];
  }
  for(int i=0; i<NUMPIXELS;i++) { // For each pixel...

  pot_led_off(0,3);
  }
  
  break;
 }
  //===========================================
  while(time_s < 60 && time_s >= 50){
  for (int i = 0; i < LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_7][i];
  }
  for (int i = LEDS_PER_DIGIT; i < 2 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[digit_8][i - LEDS_PER_DIGIT];
  }
  for (int i = 2 * LEDS_PER_DIGIT; i < 3 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[10][i - 2 * LEDS_PER_DIGIT];
  }
  for (int i = 3 * LEDS_PER_DIGIT; i < 4 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[12][i - 3 * LEDS_PER_DIGIT];
  }
  for(int i=0; i<NUMPIXELS;i++) { // For each pixel...

  pot_led_off(0,3);
  }
  break;
  } 
    for (uint16_t j = 256; j > 0; j--) { // 5 cycles of all colors on wheel
    for (uint16_t i = 0; i < NUM_LEDS; i++) {
     c = Wheel(((i * 256 / (NUM_LEDS/4)) + j) & 255); //each digit is rainbow
    // c = Wheel(((i * 256 / (NUM_LEDS*8)) + j) & 255); //slow transition缓慢的过渡
      if (map_num[i] == 1)
      {
        uint8_t R = *c * ambiant_light;
        uint8_t G = *(c + 1) * ambiant_light;
        uint8_t B = *(c + 2) * ambiant_light;
        setPixel(i, R, G, B);
      }
      else
        setPixel(i, 0, 0, 0);
    }
    showStrip();
    delay(20);
  }

}

void pot_led_on(int x, int y){
  
    pixels.setPixelColor(x, pixels.Color(255, 255, 255));
    pixels.setPixelColor(y, pixels.Color(255, 255, 255));
    pixels.show();   // Send the updated pixel colors to the hardware.
}


void pot_led_off(int x, int y){
  
    pixels.setPixelColor(x, pixels.Color(0, 0, 0));
    pixels.setPixelColor(y, pixels.Color(0, 0, 0));
    pixels.show();   // Send the updated pixel colors to the hardware.
}
 
void rainbowCycle(uint8_t SpeedDelay, int d4, int d3, int d2, int d1, float ambiant_light)
{
  
  byte *c;
  uint16_t i, j;
  bool map_num[NUM_LEDS] = {};
  for (int i = 0; i < LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[d4][i];
  }
  for (int i = LEDS_PER_DIGIT; i < 2 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[d3][i - LEDS_PER_DIGIT];
  }
  for (int i = 2 * LEDS_PER_DIGIT; i < 3 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[d2][i - 2 * LEDS_PER_DIGIT];
  }
  for (int i = 3 * LEDS_PER_DIGIT; i < 4 * LEDS_PER_DIGIT; i++)
  {
    map_num[i] = mask_digit[d1][i - 3 * LEDS_PER_DIGIT];
  }
 
  for (j = 0; j < 256; j++) { // 5 cycles of all colors on wheel车轮上所有颜色的5个循环
    for (i = 0; i < NUM_LEDS; i++) {
      c = Wheel(((i * 256 / (NUM_LEDS)) + j) & 255);
      if (map_num[i] == 1)
      {
        uint8_t R = *c * ambiant_light;
        uint8_t G = *(c + 1) * ambiant_light;
        uint8_t B = *(c + 2) * ambiant_light;
        setPixel(i, R, G, B);
      }
      else
        setPixel(i, 0, 0, 0);
    }
    showStrip();
    delay(1);
  }
}

// *** REPLACE TO HERE ***

void showStrip() {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.show();
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  FastLED.show();
#endif
}

void setPixel(int Pixel, byte red, byte green, byte blue) {
#ifdef ADAFRUIT_NEOPIXEL_H
  // NeoPixel
  strip.setPixelColor(Pixel, strip.Color(red, green, blue));
#endif
#ifndef ADAFRUIT_NEOPIXEL_H
  // FastLED
  leds[Pixel].r = red;
  leds[Pixel].g = green;
  leds[Pixel].b = blue;
#endif
}

void setAll(byte red, byte green, byte blue) {
  for (int i = 0; i < NUM_LEDS; i++ ) {
    setPixel(i, red, green, blue);
    delay(1);
  }
  showStrip();
}

byte * Wheel(byte WheelPos) {
  static byte c[3];

  if (WheelPos < 85) {
    c[0] = WheelPos * 3;
    c[1] = 255 - WheelPos * 3;
    c[2] = 0;
  } else if (WheelPos < 170) {
    WheelPos -= 85;
    c[0] = 255 - WheelPos * 3;
    c[1] = 0;
    c[2] = WheelPos * 3;
  } else {
    WheelPos -= 170;
    c[0] = 0;
    c[1] = WheelPos * 3;
    c[2] = 255 - WheelPos * 3;
  }

  return c;
}
