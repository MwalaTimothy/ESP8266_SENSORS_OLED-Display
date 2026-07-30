#include <Wire.h> // Include the library for I2C Communication
#include "SSD1306Wire.h" //Include the library for OLED display
#include <BH1750.h> // Include the light sensor library

SSD1306Wire display(0x3C, SDA, SCL,GEOMETRY_128_64);

BH1750 light(0x23); // initialize BH1750

void setup() {
  Serial.begin(115200); //Initialize serial communication

  display.init(); //Initialize OLED display
  display.flipScreenVertically();
  display.setFont(ArialMT_Plain_10); // Reduce font size
  display.setTextAlignment(TEXT_ALIGN_LEFT); // Set text alignment to left
}

void loop() {
  display.clear(); // clear display buffer

  display.drawString(0,5, "Ambient Light");
  
  if (light.begin(BH1750::CONTINUOUS_HIGH_RES_MODE)) { //check if light sensor initialization is successful
    // Adjust the position of the text to fit the OLED screen
    display.drawString(0, 20, "Light Intensity:");
    display.drawString(0, 40, String(light.readLightLevel()) + " Lx"); // Display light intensity reading in lux
  } else {
    display.drawString(0, 40, "Error!"); // display error message if initialization fails
  }

  display.display(); // send the display buffer to the OLED
  delay(1000);
}
