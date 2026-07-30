#include <Wire.h> //include wire library for I2C Communication
#include <U8g2lib.h> //include library for OLED display
#include <Adafruit_BMP085.h> // Use Adafruit_BMP180 if your sensor is BMP180

#define SCREEN_WIDTH 128 //define width of OLED Screen
#define SCREEN_HEIGHT 64 //Define the height of the OLED Screen

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE); // Setup U8g2 library with D1 (SCL) and D2 (SDA) pins for SSD1306 OLED

Adafruit_BMP085 bmp; 

void setup() {
  Serial.begin(9600);
  if (!bmp.begin()) {// Check if BMP180 sensor is found
    Serial.println("Could not find BMP180 or BMP085 sensor!");
    while (1) {}
  }

  u8g2.begin();
}
void loop() {
  float temperature = bmp.readTemperature();//Read Temperature from BMP180 sensor
  float pressure = bmp.readPressure() / 100.0; // Read Pressure from BMP180 sensor
  float altitude = bmp.readAltitude(); //Read Altitude from BMP180 sensor
  float seaLevelPressure = bmp.readSealevelPressure() / 100.0; // Read Sea Level from BMP180 sensor

  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_6x12_tf); // Choose a slightly larger font (6x12 pixels)

  u8g2.setCursor(0, 10);
  u8g2.print("BMP180");

  u8g2.setCursor(0, 25);
  u8g2.print("Temp: ");
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.setCursor(0, 35);
  u8g2.print("Pressure: ");
  u8g2.print(pressure);
  u8g2.print(" hPa");

  u8g2.setCursor(0, 45);
  u8g2.print("Altitude: ");
  u8g2.print(altitude);
  u8g2.print(" m");

  u8g2.setCursor(0, 55);
  u8g2.print("Sea Level: ");
  u8g2.print(seaLevelPressure);
  u8g2.print(" hPa");

  u8g2.sendBuffer();// send the display buffer to the OLED

  delay(2000); // Update every 2 seconds
}
