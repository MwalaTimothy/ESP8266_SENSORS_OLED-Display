#include <Wire.h>// Include wire library for I2C communication
#include <U8g2lib.h> //Include library for OLED display
#include <DHT.h>// include library for DHT sensor

#define SCREEN_WIDTH 128 //define width of OLED screen
#define SCREEN_HEIGHT 64 //define height of OLED screen

U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE); // Setup U8g2 library with D1 (SCL) and D2 (SDA) pins for SSD1306 OLED
#define DHTPIN D4    // Digital pin connected to the DHT sensor
#define DHTTYPE DHT22   // DHT 22 (AM2302)

DHT dht(DHTPIN, DHTTYPE); //create a DHT object

void setup() {
  Serial.begin(9600); //initialize serial communication
  dht.begin(); //Initialize DHT sensor
  u8g2.begin(); //Initialize OLED Display
}

void loop() {
  delay(2000); // Delay to stabilize sensor readings

  float temperature = dht.readTemperature(); //Read Temp from DHT
  float humidity = dht.readHumidity(); //Read Humidity from DHT

  u8g2.clearBuffer();

  u8g2.setFont(u8g2_font_6x12_tf); // Choose a font

   u8g2.setCursor(0, 10);
  u8g2.print("DHT22");

  u8g2.setCursor(0, 30);
  u8g2.print("Temp: ");
  u8g2.print(temperature);
  u8g2.print(" C");

  u8g2.setCursor(0, 40);
  u8g2.print("Humidity: ");
  u8g2.print(humidity);
  u8g2.print(" %");

  u8g2.sendBuffer();

  delay(2000); // Update every 2 seconds
}
