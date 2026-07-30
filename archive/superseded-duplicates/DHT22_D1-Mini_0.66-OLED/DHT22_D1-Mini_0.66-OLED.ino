#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include "Adafruit_SSD1306.h"
#include <DHT.h>

#define OLED_RESET 0 // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

#define DHTPIN D4
#define DHTTYPE DHT22
DHT dht(DHTPIN, DHTTYPE);

void setup()
{
  Serial.begin(115200);
  dht.begin();
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C); // initialize with the I2C addr 0x3C (for the 64x48)
  display.display();
}

void loop()
{
  delay(1000);

  // Read sensor values
  float h = dht.readHumidity();
  float t = dht.readTemperature();
  float f = dht.readTemperature(true);
  float bat = ESP.getVcc();

  // Check if any reads failed and exit early (to try again).
  if (isnan(h) || isnan(t) || isnan(f))
  {
    display.println("Failed to read from DHT sensor!");
    return;
  }

  // Clear the display and set the cursor to the top-left corner
  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);
  

  // clear buffer and create a 1st page to Display temperature in Celsius
  display.clearDisplay();
  display.setCursor(32,10);
  display.println("Temperature.");
  display.setCursor(32,19);
  display.print(t);
  display.println(" C");

  display.display();
  delay(3000);
  

  // clear buffer and create a 2nd page to Display humidity
  display.clearDisplay();
  display.setCursor(32,10);
  display.println("Humidity");
  display.setCursor(32,19);
  display.print(h);
  display.println(" %");

  display.display();

  delay(3000);
}
