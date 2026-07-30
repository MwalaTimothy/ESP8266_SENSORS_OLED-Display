#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#ifndef UNIT_TEST
#include <Arduino.h>
#endif
#include <IRremoteESP8266.h>
#include <IRrecv.h>
#include <IRutils.h>
#if DECODE_AC
#include <ir_Daikin.h>
#include <ir_Fujitsu.h>
#include <ir_Gree.h>
#include <ir_Haier.h>
#include <ir_Kelvinator.h>
#include <ir_Midea.h>
#include <ir_Toshiba.h>
#endif  // DECODE_AC
 
#define RECV_PIN D4
#define BAUD_RATE 115200
#define CAPTURE_BUFFER_SIZE 1024
 
 
#if DECODE_AC
#define TIMEOUT 50U  
#else  
#define TIMEOUT 15U  // Suits most messages, while not swallowing many repeats.
#endif  
 
#define MIN_UNKNOWN_SIZE 12
 
 // SCL GPIO5
// SDA GPIO4
#define OLED_RESET 0  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

// Use turn on the save buffer feature for more complete capture coverage.
IRrecv irrecv(RECV_PIN, CAPTURE_BUFFER_SIZE, TIMEOUT, true);
 
decode_results results;  // Somewhere to store the results
 
 
void setup() 
{
  Serial.begin(BAUD_RATE, SERIAL_8N1, SERIAL_TX_ONLY);
  while (!Serial)  // Wait for the serial connection to be establised.
    delay(50);

  display.println();
  display.print("IRrecvDumpV2 is now running and waiting for IR input on Pin ");
  display.println(RECV_PIN);
 
        // begin returns a boolean that can be used to detect setup problems.
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);  // initialize with the I2C addr 0x3C (for the 64x48)
  display.display();
 
#if DECODE_HASH
  // Ignore messages with less than minimum on or off pulses.
  irrecv.setUnknownThreshold(MIN_UNKNOWN_SIZE);
#endif  // DECODE_HASH
  irrecv.enableIRIn();  // Start the receiver
}
 
 
void loop() 
{
  delay(200);
  if (irrecv.decode(&results)) 
  {
    // print() & println() can't handle printing long longs. (uint64_t)
    display.clearDisplay();
   display.setTextSize(1);
   display.setTextColor(WHITE);
   display.setCursor(40,8);
   display.print("remote ");
    display.setCursor(40,18);
   display.setTextSize(1);
   display.print(results.value, HEX);
    display.println("");
    irrecv.resume();  // Receive the next value
    display.display();
  }
  delay(200);
}
