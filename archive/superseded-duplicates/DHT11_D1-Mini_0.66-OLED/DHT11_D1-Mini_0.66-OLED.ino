#include "DHT.h" //DHT library
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

const int OLED_RESET = 0;
Adafruit_SSD1306 display(OLED_RESET);

//Pin to which the DHT11 sensor is connected.
//In the DHT11 shield it is the digital pin D4.
const int DHTPIN = D4 ;

//Determine which type of DHT sensor is used.
#define DHTTYPE DHT11

//Initialise the sensor with the pin and type
DHT dht(DHTPIN, DHTTYPE);

void setup() {
  Serial.begin(9600); //Begin serial communication with 9600 baud.
  
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
  display.display();
  delay(2000);
  display.clearDisplay(); 
  dht.begin(); //DHT communication begin.
}

void loop() {
  //The DHT11 sensor returns a new value every 2 seconds.
  //Value every 2 seconds, so it is not worth running the loop constantly. 
  //to let it run.
  delay(2000);

  //read the humidity
  double humidity = dht.readHumidity();
  //read the temperature in degrees Celsius
  double temperatureC = dht.readTemperature();
  //read the temperature in degrees Fahrenheit
  //the Boolean parameter is used to "control" whether
  //the temperature is output in Fahrenheit or Celsius.
  double temperatureF = dht.readTemperature(true);

  //Check if the values were read successfully.
  if (isnan(humidity) || isnan(temperatureC) || isnan(temperatureF)) {
    Serial.println("Error reading data.");
    return;
  }

  display.clearDisplay();
  display.setTextSize(1);
  display.setTextColor(WHITE);

  display.setCursor(32,8);
  display.println("DHT11");

  display.setCursor(32,9);
  display.println("_____");

  display.setCursor(32,17);
  String tempValue = String(temperatureC);
  display.println("T: "+tempValue+"C");

  display.setCursor(32,25);
  String humValue = String(humidity);
  display.println("H: "+humValue+"%");

  display.display();
  delay(500);
}
