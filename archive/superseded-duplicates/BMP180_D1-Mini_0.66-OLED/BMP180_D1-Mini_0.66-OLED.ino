#include <Wire.h>
#include <Adafruit_BMP085.h>
#include <SFE_MicroOLED.h> 

#define PIN_RESET 255 
#define DC_JUMPER 0 

MicroOLED oled(PIN_RESET, DC_JUMPER); // Example I2C declaration
Adafruit_BMP085 bmp;

void setup(){
Serial.begin(9600);
  
if (!bmp.begin()) {
Serial.println("Could not find a valid BMP180 sensor, check wiring!");
while (1) {}
}
  
oled.begin();
oled.clear(ALL); // Clear the display's memory (gets rid of artifacts)
oled.display();
}

void loop()
{
// Wait a few seconds between measurements.
delay(2000);
  
oled.clear(PAGE);
oled.setFontType(0); 
  
oled.setCursor(1, 3);
oled.print("Pressure = ");
oled.setCursor(1, 12);
oled.print(bmp.readPressure());
oled.print(" Pa");
  
oled.setCursor(1, 21);
oled.print("Temp =");
oled.setCursor(1, 30);
oled.print(bmp.readTemperature());
oled.print(" *C ");
  
oled.display();
  
delay(2000);
  
//2nd page of readings
oled.clear(PAGE);
oled.setFontType(0); 
  
oled.setCursor(1, 3);
oled.print("Altitude = ");
oled.setCursor(1, 12);
oled.print(bmp.readAltitude());
oled.print(" m");
  
oled.setCursor(1, 21);
oled.print("Sea level=");
oled.setCursor(1, 30);
oled.print(bmp.readSealevelPressure());
oled.print(" Pa");
  
oled.display();
}
