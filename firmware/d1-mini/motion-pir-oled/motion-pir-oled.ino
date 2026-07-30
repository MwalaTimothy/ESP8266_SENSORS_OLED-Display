#include <Wire.h>
#include <U8g2lib.h>

#define SCREEN_WIDTH 128
#define SCREEN_HEIGHT 64
#define PIR_PIN D3 // Define the pin to which the PIR sensor is connected

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE); // Setup U8g2 library with D1 (SCL) and D2 (SDA) pins for SSD1306 OLED

void setup() {
  pinMode(PIR_PIN, INPUT); // Set PIR pin as input
  u8g2.begin();
}

void loop() {
  // Read the state of the PIR sensor
  int motionDetected = digitalRead(PIR_PIN);
  
  // Clear the OLED screen
  u8g2.clearBuffer();
  
  // Display motion detection status
  u8g2.setFont(u8g2_font_6x12_tf); // Choose a font
  u8g2.setCursor(0, 10);
  u8g2.print("PIR");
  

  if (motionDetected == HIGH) {
    u8g2.setCursor(0, 40);
    u8g2.print("Motion Detected!");
  } else {
    u8g2.setCursor(0, 40);
    u8g2.print("No Motion Detected");
  }
  
  // Send the clear buffer to the OLED
  u8g2.sendBuffer();
  
  // Add a delay to reduce sensor reading frequency
  delay(1000);
}
