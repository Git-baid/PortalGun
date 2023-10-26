#include <FastLED.h>
#include <SoftwareSerial.h>
#include "Adafruit_Soundboard.h"

#define LED_PIN          7
#define NUM_LEDS         16
#define BLUE_PIN         2
#define ORANGE_PIN       3

//#define SOUNDBRD_PWR     4
#define LED_GND     5

#define AMP_PWR          8
#define AMP_GND          9

#define TX         10
#define RX       11
#define RST       12


SoftwareSerial ss = SoftwareSerial(TX, RX);
Adafruit_Soundboard sfx = Adafruit_Soundboard(&ss, NULL, RST);

int orangePin;
int bluePin;

int colorState = 0;
unsigned long sfxPlayTime = 0;
bool soundPlaying = false;
const unsigned long sfxLength = 888; //milliseconds length of sfx
int brightness; // 0-255
const int targetBrightness = 50;
const float interpSpeed = 1.9; // higher = slower
CRGB leds[NUM_LEDS];

void setup() {

  Serial.begin(115200);
  ss.begin(9600);

  if (!sfx.reset()) {
    Serial.println("Not found");
    while (1);
  }
  Serial.println("SFX board found");

  pinMode(BLUE_PIN, INPUT_PULLUP);
  pinMode(ORANGE_PIN, INPUT_PULLUP);

  //5V power source for adafruit soundboard
  //pinMode(SOUNDBRD_PWR, OUTPUT);
  //digitalWrite(SOUNDBRD_PWR, HIGH);
  //Ground for adafruit soundboard
  pinMode(LED_GND, OUTPUT);
  digitalWrite(LED_GND, LOW);

  //5V power source for adafruit amplifier
  pinMode(AMP_PWR, OUTPUT);
  digitalWrite(AMP_PWR, HIGH);
  //Ground for adafruit amplifier
  pinMode(AMP_GND, OUTPUT);
  digitalWrite(AMP_GND, LOW);

  FastLED.addLeds<WS2812, LED_PIN, GRB>(leds, NUM_LEDS);

}

void loop() {
  unsigned long currentMillis = millis();
  bluePin = digitalRead(BLUE_PIN);
  orangePin = digitalRead(ORANGE_PIN);
  if (bluePin == 1 && orangePin == 1){
    colorState = 0;
  }
  else if(bluePin == 0 && colorState == 0){
    bluePortal();
  }
  else if(orangePin == 0 && colorState == 0){
    orangePortal();
  }

  if(sfxPlayTime == 0 && soundPlaying){
    sfxPlayTime = currentMillis;
  } else if(currentMillis - sfxPlayTime >= 888 && soundPlaying){
    Serial.println("sfx over"); Serial.print(currentMillis-sfxPlayTime);
    soundPlaying = false;
    sfxPlayTime = 0;
  }
  FastLED.show(brightness);
  if(brightness > targetBrightness){
    brightness -= (brightness-targetBrightness)/(targetBrightness*interpSpeed); // interpolate brightness towards target brightness, smooth "animation" :)
  }
}


void orangePortal(){
  for(int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(255, 60, 0);
  colorState = 2;
  Serial.println("playing orange");
  if(soundPlaying)
    sfx.stop();
  soundPlaying = true;
  sfxPlayTime=0;
  brightness = 255;
  FastLED.show(brightness);
  if (!sfx.playTrack((uint8_t)2)) {
    Serial.println("Failed to play track?");
  }
  
}
void bluePortal(){
  for(int i = 0; i < NUM_LEDS; i++)
    leds[i] = CRGB(0, 0, 255);
  colorState = 1;
  Serial.println("playing blue");
  if(soundPlaying)
    sfx.stop();
  soundPlaying = true;
  sfxPlayTime=0;
  brightness = 255;
  FastLED.show(brightness);
  if (!sfx.playTrack((uint8_t)1)) {
    Serial.println("Failed to play track?");
  }
  
}
