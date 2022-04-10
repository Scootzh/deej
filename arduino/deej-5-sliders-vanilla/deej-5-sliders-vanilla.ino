
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int NUM_SLIDERS = 4;
const int analogInputs[NUM_SLIDERS] = {A0, A1, A2, A6};

int displayVolume[NUM_SLIDERS];
int analogSliderValues[NUM_SLIDERS];

//Display Stuff
#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 64 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET     -1 // Reset pin # (or -1 if sharing Arduino reset pin)
Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

const unsigned char discord [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x07, 
  0xf8, 0x00, 0x0f, 0xfc, 0x00, 0x0f, 0xfc, 0x00, 0x1c, 0xce, 0x00, 0x1c, 0xce, 0x00, 0x1c, 0xce, 
  0x00, 0x1f, 0xfe, 0x00, 0x0e, 0x1c, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00
};
const unsigned char steam [] PROGMEM = {
  0x00, 0x00, 0x00, 0x03, 0xf0, 0x00, 0x0f, 0xfc, 0x00, 0x1f, 0xfe, 0x00, 0x3f, 0xc3, 0x00, 0x3f, 
  0x81, 0x80, 0x7f, 0x99, 0x80, 0x7f, 0x19, 0x80, 0x7f, 0x03, 0x80, 0x1e, 0x03, 0x80, 0x00, 0x1f, 
  0x80, 0x00, 0x3f, 0x80, 0x30, 0xff, 0x80, 0x38, 0xff, 0x00, 0x1f, 0xfe, 0x00, 0x0f, 0xfc, 0x00, 
  0x07, 0xf8, 0x00, 0x00, 0x00, 0x00
};

const unsigned char sys [] PROGMEM = {
  0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 
  0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0x80, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xff, 0x3f, 
  0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0, 
  0xff, 0x3f, 0xc0, 0xff, 0x3f, 0xc0
};

const unsigned char music [] PROGMEM = {
  0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3f, 0x00, 0x07, 0xff, 0x00, 0x07, 0xff, 0x00, 0x07, 
  0xff, 0x00, 0x07, 0xe3, 0x00, 0x06, 0x03, 0x00, 0x04, 0x03, 0x00, 0x04, 0x03, 0x00, 0x04, 0x03, 
  0x00, 0x04, 0x1f, 0x00, 0x0c, 0x3f, 0x00, 0x7c, 0x3e, 0x00, 0x7c, 0x3c, 0x00, 0x7c, 0x00, 0x00, 
  0x78, 0x00, 0x00, 0x00, 0x00, 0x00
};



void setup() { 
  for (int i = 0; i < NUM_SLIDERS; i++) {
    pinMode(analogInputs[i], INPUT);
  }
  Serial.begin(9600);
  
  if(!display.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // Address 0x3D for 128x64
  Serial.println(F("SSD1306 allocation failed"));
  for(;;); // Don't proceed, loop forever
  }
  display.clearDisplay();
  display.display();
}

void loop() {
  updateSliderValues();
  sendSliderValues(); // Actually send data (all the time)
  // printSliderValues(); // For debug
  delay(10);
}

void updateSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
     analogSliderValues[i] = analogRead(analogInputs[i]);
     
     if(analogSliderValues[i] > displayVolume[i]+30 || analogSliderValues[i] < displayVolume[i]-30){
        displayVolume[i] = analogSliderValues[i];
        displayVol();
     }
     
  }
}

void sendSliderValues() {
  String builtString = String("");

  for (int i = 0; i < NUM_SLIDERS; i++) {
    builtString += String((int)analogSliderValues[i]);

    if (i < NUM_SLIDERS - 1) {
      builtString += String("|");
    }
  }
  
  Serial.println(builtString);
}

void displayVol(){
  display.clearDisplay();
  
  //Draw "under"Lines and bitMap image
  display.drawBitmap(7, 2, sys, 18, 18, WHITE);
  display.drawLine(2,63,30,63, SSD1306_WHITE);
  display.drawBitmap(39, 2, steam, 18, 18, WHITE);
  display.drawLine(34,63,62,63, SSD1306_WHITE);
  display.drawBitmap(71, 2, music, 18, 18, WHITE);
  display.drawLine(66,63,94,63, SSD1306_WHITE);
  display.drawBitmap(103, 2, discord, 18, 18, WHITE);
  display.drawLine(98,63,127,63, SSD1306_WHITE);

  //Draw volume bar
  display.fillRect(7, map(displayVolume[0], 0, 1023, 62, 21), 19,62 ,  SSD1306_WHITE);
  display.fillRect(39, map(displayVolume[1], 0, 1023, 62, 21), 19, 62,  SSD1306_WHITE);
  display.fillRect(71, map(displayVolume[2], 0, 1023, 62, 21), 19, 62,  SSD1306_WHITE);
  display.fillRect(103, 21, 19, 62,  SSD1306_WHITE);
  display.display();
}

void printSliderValues() {
  for (int i = 0; i < NUM_SLIDERS; i++) {
    String printedString = String("Slider #") + String(i + 1) + String(": ") + String(analogSliderValues[i]) + String(" mV");
    Serial.write(printedString.c_str());

    if (i < NUM_SLIDERS - 1) {
      Serial.write(" | ");
    } else {
      Serial.write("\n");
    }
  }
}
