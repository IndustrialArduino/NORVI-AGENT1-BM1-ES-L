#include "Wire.h"
#include <Adafruit_GFX.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ADS1015.h>



#define RXD 23
#define TXD 13
#define FC  32

#define BUTTON_PIN   35    // Digital IO pin connected to the button.  This will be
                          // driven with a pull-up resistor so the switch should
                          // pull the pin to ground momentarily.  On a high -> low
                          // transition the button press logic will execute.

#define PIXEL_PIN    25    // Digital IO pin connected to the NeoPixels.

#define PIXEL_COUNT 1

Adafruit_NeoPixel strip = Adafruit_NeoPixel(PIXEL_COUNT, PIXEL_PIN, NEO_GRB + NEO_KHZ800);

bool oldState = HIGH;
int showType = 0;

Adafruit_ADS1115 ads1(0x48);

int reading[8]={0};
String adcString[8];

void setup() {

  Serial.begin(115200);
  
  pinMode(FC, OUTPUT); 
 digitalWrite(FC, HIGH);

 Serial1.begin(9600, SERIAL_8N1,RXD,TXD);

 digitalWrite(FC, HIGH);   // RS-485 
 Serial1.println(" ");
 
  pinMode(27, OUTPUT);
  pinMode(26, OUTPUT);
  pinMode(18, OUTPUT);
  pinMode(19, OUTPUT);
 
  delay(100);
  input_led_test();
  delay(500);
  input_led_test();
  delay(500);
  
  pinMode(27, INPUT);
  pinMode(26, INPUT);
  pinMode(18, INPUT);
  pinMode(19, INPUT);

  
  ads1.begin();
  ads1.setGain(GAIN_ONE); 

  pinMode(BUTTON_PIN, INPUT_PULLUP);
  strip.begin();
  strip.show(); // Initialize all pixels to 'off'
}

void loop() {
 
  
      Serial1.println();
  
  // put your main code here, to run repeatedly:
  bool newState = digitalRead(BUTTON_PIN);

  // Check if state changed from high to low (button press).
  if (newState == LOW && oldState == HIGH) {
    // Short delay to debounce button.
    delay(20);
    // Check if button is still low after debounce.
    newState = digitalRead(BUTTON_PIN);
    if (newState == LOW) {
      showType++;
      if (showType > 9)
        showType=0;
      startShow(showType);
    }
  }

  // Set the last button state to the old state.
  oldState = newState;

 
  Serial1.println("  ");
  Serial.print("I1: ");Serial.println(digitalRead(27));
  Serial.print("I2: ");Serial.println(digitalRead(26));
  Serial.print("I3: ");Serial.println(digitalRead(18));
  Serial.print("I4: ");Serial.println(digitalRead(19));

  adcString[0] = String(ads1.readADC_SingleEnded(0));
  adcString[0] = String(ads1.readADC_SingleEnded(0));
  delay(10);
  adcString[1] = String(ads1.readADC_SingleEnded(1));
  adcString[1] = String(ads1.readADC_SingleEnded(1));
  delay(10);
  adcString[2] = String(ads1.readADC_SingleEnded(2));
  adcString[2] = String(ads1.readADC_SingleEnded(2));
  delay(10);
  adcString[3] = String(ads1.readADC_SingleEnded(3));
  adcString[3] = String(ads1.readADC_SingleEnded(3));
  delay(10);
  Serial.print("A1: ");Serial.print(adcString[0]);Serial.print("  ");
  Serial.print("A2: ");Serial.println(adcString[1]);
  Serial.print("A3: ");Serial.print(adcString[2]);Serial.print("  ");
  Serial.print("A4: ");Serial.println(adcString[3]);

  
  digitalWrite(FC, HIGH);                    // Make FLOW CONTROL pin HIGH
  delay(500);
  Serial1.println(F("RS485 01 SUCCESS"));    // Send RS485 SUCCESS serially
  delay(500);                                // Wait for transmission of data
  digitalWrite(FC, LOW) ;                    // Receiving mode ON

                                             // Serial1.flush() ;
  delay(1000);     
  
  while (Serial1.available()) {  // Check if data is available
    char c = Serial1.read();     // Read data from RS485
    
    Serial.write( c );// Print data on serial monitor
  }
 Serial.println(" ");
  Serial.println("____________________________________");  
}


void input_led_test(){
  digitalWrite(27, LOW); 
  digitalWrite(26, HIGH);
  digitalWrite(18, HIGH);   
 // digitalWrite(19, HIGH);

  delay(500);
  digitalWrite(27, HIGH); 
  digitalWrite(26, LOW);
  digitalWrite(18, HIGH);   
 // digitalWrite(19, HIGH);

  delay(500);
  digitalWrite(27, HIGH); 
  digitalWrite(26, HIGH);
  digitalWrite(18, LOW);   
  digitalWrite(19, HIGH);

   delay(500);
  digitalWrite(27, HIGH); 
  digitalWrite(26, HIGH);
  digitalWrite(18, HIGH);   
  digitalWrite(19, LOW);

  
   delay(500);
  digitalWrite(27, HIGH); 
  digitalWrite(26, HIGH);
  digitalWrite(18, HIGH);   
  digitalWrite(19, HIGH);

  
}


void startShow(int i) {
  switch(i){
    case 0: colorWipe(strip.Color(0, 0, 0), 50);    // Black/off
            break;
    case 1: colorWipe(strip.Color(255, 0, 0), 50);  // Red
            break;
    case 2: colorWipe(strip.Color(0, 255, 0), 50);  // Green
            break;
    case 3: colorWipe(strip.Color(0, 0, 255), 50);  // Blue
            break;
    case 4: theaterChase(strip.Color(127, 127, 127), 50); // White
            break;
    case 5: theaterChase(strip.Color(127,   0,   0), 50); // Red
            break;
    case 6: theaterChase(strip.Color(  0,   0, 127), 50); // Blue
            break;
    case 7: rainbow(20);
            break;
    case 8: rainbowCycle(20);
            break;
    case 9: theaterChaseRainbow(50);
            break;
  }
}

// Fill the dots one after the other with a color
void colorWipe(uint32_t c, uint8_t wait) {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, c);
    strip.show();
    delay(wait);
  }
}

void rainbow(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256; j++) {
    for(i=0; i<strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel((i+j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

// Slightly different, this makes the rainbow equally distributed throughout
void rainbowCycle(uint8_t wait) {
  uint16_t i, j;

  for(j=0; j<256*5; j++) { // 5 cycles of all colors on wheel
    for(i=0; i< strip.numPixels(); i++) {
      strip.setPixelColor(i, Wheel(((i * 256 / strip.numPixels()) + j) & 255));
    }
    strip.show();
    delay(wait);
  }
}

//Theatre-style crawling lights.
void theaterChase(uint32_t c, uint8_t wait) {
  for (int j=0; j<10; j++) {  //do 10 cycles of chasing
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, c);    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

//Theatre-style crawling lights with rainbow effect
void theaterChaseRainbow(uint8_t wait) {
  for (int j=0; j < 256; j++) {     // cycle all 256 colors in the wheel
    for (int q=0; q < 3; q++) {
      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, Wheel( (i+j) % 255));    //turn every third pixel on
      }
      strip.show();

      delay(wait);

      for (int i=0; i < strip.numPixels(); i=i+3) {
        strip.setPixelColor(i+q, 0);        //turn every third pixel off
      }
    }
  }
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
    return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  }
  if(WheelPos < 170) {
    WheelPos -= 85;
    return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  }
  WheelPos -= 170;
  return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
}
