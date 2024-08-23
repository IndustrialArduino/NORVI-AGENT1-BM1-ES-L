/*
 * NORVI-AGENT-BM1-ES-L
 */
#include <Wire.h>
#include <Adafruit_NeoPixel.h>
#include <Adafruit_ADS1X15.h>

Adafruit_ADS1115 ads1;
#define VOLTAGE_DIVIDER_RATIO 0.4005

#define BUTTON_PIN   35   // Digital IO pin connected to the button.  

#define LED_PIN 25     // Pin connected to the data input of the LED
#define NUM_LEDS 1     // Number of LEDs (1 in this case)

#define RXD 33 // 485 DIFINE
#define TXD 13
#define FC  32

Adafruit_NeoPixel strip = Adafruit_NeoPixel(NUM_LEDS, LED_PIN, NEO_GRB + NEO_KHZ800);


void setup() {
  Serial.begin(115200);

  pinMode(FC, OUTPUT); 
  digitalWrite(FC, HIGH);
  Serial1.begin(9600, SERIAL_8N1,RXD,TXD);

  rgb_test();
  delay(1000);
  
  pinMode(27, INPUT);
  pinMode(36, INPUT);
  pinMode(34, INPUT);
  
  pinMode(BUTTON_PIN, INPUT);
  
   Wire.begin(21,22);
   
  if (!ads1.begin(0x48)) {
    Serial.println("Failed to initialize ADS 1 .");
    while (1);
  }
  ads1.setGain(GAIN_ONE);  // 1x gain +/- 4.096V  (1 bit = 0.125mV)

 digitalWrite(FC, HIGH);   // RS-485 
}

void loop() {
  int16_t adc0, adc1, adc2, adc3;
  Serial.println("");  

  Serial.print("BUTTON: ");
  Serial.println(digitalRead(BUTTON_PIN));

  Serial.print("I1: ");Serial.println(digitalRead(27));
  Serial.print("I2: ");Serial.println(digitalRead(36));
  Serial.print("I3: ");Serial.println(digitalRead(34));
  delay(200);
  
  adc0 = ads1.readADC_SingleEnded(0);
  adc1 = ads1.readADC_SingleEnded(1);
  adc2 = ads1.readADC_SingleEnded(2);
  adc3 = ads1.readADC_SingleEnded(3);
 
  float voltage0 = adc0 * 0.125 / 1000.0 / VOLTAGE_DIVIDER_RATIO;  
  float voltage1 = adc1 * 0.125 / 1000.0 / VOLTAGE_DIVIDER_RATIO; 
  float voltage2 = adc2 * 0.125 / 1000.0 / VOLTAGE_DIVIDER_RATIO;  
  float voltage3 = adc3 * 0.125 / 1000.0 / VOLTAGE_DIVIDER_RATIO; 
  
  Serial.print("Input Voltage 0: "); Serial.print(voltage1); Serial.println(" V");
  Serial.print("Input Voltage 1: "); Serial.print(voltage2); Serial.println(" V");
  Serial.print("Input Voltage 2: "); Serial.print(voltage0); Serial.println(" V");
  
  digitalWrite(FC, HIGH);                    // Make FLOW CONTROL pin HIGH
  delay(300);
  Serial1.println(F("RS485 01 SUCCESS"));    // Send RS485 SUCCESS serially
  delay(300);                                // Wait for transmission of data
  digitalWrite(FC, LOW) ;                    // Receiving mode ON
  delay(300);     
  
  while (Serial1.available()) {  // Check if data is available
    char c = Serial1.read();     // Read data from RS485
    Serial.write(c);             // Print data on serial monitor
  }
 delay(300); 
 Serial.println("____________________________________");  
}

void rgb_test(){
  strip.setPixelColor(0, strip.Color(255, 0, 0)); // Red
  strip.show();
  delay(1000); // Wait for 1 second

  strip.setPixelColor(0, strip.Color(0, 255, 0)); // Green
  strip.show();
  delay(1000); // Wait for 1 second

  strip.setPixelColor(0, strip.Color(0, 0, 255)); // Blue
  strip.show();
  delay(1000); // Wait for 1 second

  strip.setPixelColor(0, strip.Color(255, 255, 0)); // Yellow
  strip.show();
  delay(1000); // Wait for 1 second

  strip.setPixelColor(0, strip.Color(0, 255, 255)); // Cyan
  strip.show();
  delay(1000); // Wait for 1 second

  strip.setPixelColor(0, strip.Color(255, 0, 255)); // Magenta
  strip.show();
  delay(1000); // Wait for 1 second

  strip.setPixelColor(0, strip.Color(255, 255, 255)); // White
  strip.show();
  delay(1000); // Wait for 1 second

  strip.setPixelColor(0, strip.Color(0, 0, 0)); // Off
  strip.show();
  delay(1000); // Wait for 1 second
}
