#include <Wire.h> // Include the Wire library for I2C communication
#include <U8g2lib.h> // Include the U8g2 library for OLED display
#include <DHT.h> // Include the DHT library for DHT sensor

#define SCREEN_WIDTH 128 // Define the width of the OLED screen
#define SCREEN_HEIGHT 64 // Define the height of the OLED screen

// Setup U8g2 library with D1 (SCL) and D2 (SDA) pins for SSD1306 OLED
U8G2_SSD1306_128X64_NONAME_F_SW_I2C u8g2(U8G2_R0, /* clock=*/ D1, /* data=*/ D2, /* reset=*/ U8X8_PIN_NONE);

#define DHTPIN D4     // Digital pin connected to the DHT sensor
#define DHTTYPE DHT11   // DHT 11

DHT dht(DHTPIN, DHTTYPE); // Create a DHT object

void setup() {
  Serial.begin(9600); // Initialize serial communication
  dht.begin(); // Initialize DHT sensor
  u8g2.begin(); // Initialize OLED display
}

void loop() {
  delay(2000); // Delay to stabilize sensor readings

  float temperature = dht.readTemperature(); // Read temperature from DHT sensor
  float humidity = dht.readHumidity(); // Read humidity from DHT sensor

  u8g2.clearBuffer(); // Clear the display buffer

  u8g2.setFont(u8g2_font_6x12_tf); // Choose a font

  u8g2.setCursor(0, 10); // Set cursor position
  u8g2.print("DHT11"); // Display sensor type

  u8g2.setCursor(0, 30); // Set cursor position
  u8g2.print("Temp: "); 
  u8g2.print(temperature); // Display temperature value
  u8g2.print(" C"); 

  u8g2.setCursor(0, 40); // Set cursor position
  u8g2.print("Humidity: "); 
  u8g2.print(humidity); // Display humidity value
  u8g2.print(" %"); 

  u8g2.sendBuffer(); // Send the display buffer to the OLED

  delay(2000); // Update every 2 seconds
}
