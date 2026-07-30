#include <ESP8266WiFi.h>
#include <Wire.h>
#include<WiFiManager.h>
#include <Adafruit_GFX.h>
#include <Adafruit_ILI9341.h>
#include <NTPClient.h>
#include <WiFiUdp.h>
#include <TimeLib.h>

// Include additional libraries for weather and sensor readings
#include <Adafruit_SGP30.h>

Adafruit_SGP30 sgp30; 

// Replace with your network credentials
const char* ssid = "*****";
const char* password = "*****";

// Define the NTP client
WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "pool.ntp.org");

// Define TFT display pins
#define TFT_CS    D0  // CS pin for TFT
#define TFT_DC    D8  // DC pin for TFT
#define TFT_RST   D6  // RST pin for TFT

Adafruit_ILI9341 tft = Adafruit_ILI9341(TFT_CS, TFT_DC, TFT_RST);

// Buffers for the tree, countdown, and header
char treeBuffer[100];
char countdownBuffer[100];
char headerBuffer[50];

void setup() {
  Serial.begin(115200);

  // Connect to Wi-Fi
 // WiFi.begin(ssid, password);
   WiFiManager wm;
  wm.resetSettings();

  bool res;
      res = wm.autoConnect("ChristamsGift","password");
      if(!res) {
        Serial.println("Failed to connect");
        //ESP.restart();
      }
  while (WiFi.status() != WL_CONNECTED) {
    delay(1000);
    Serial.println("Connecting to WiFi...");
  }
  Serial.println("Connected to WiFi");
//SGP30 SENSOR
 
  if (sgp30.begin() == false)
  {

    while (1)
      ;
  }

  // Initialize TFT display
  tft.begin();
  tft.setRotation(0);  // Set rotation to 0 for a static display

  // Initialize NTP client
  timeClient.begin();
}

void loop() {
  // Update the NTP client
  timeClient.update();

  // Display the header, Christmas tree, and countdown
  displayHeader();
  drawChristmasTree();
  displayCountdown();

  // Fetch and display weather conditions and sensor readings here
 if (sgp30.IAQmeasure())
 { 
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLUE); 
   tft.setCursor(10, 230);
  tft.print("Air Quality Meter");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLUE); 
  tft.setCursor(10, 230);
  tft.println("Carbon(iv)Oxide: ");
  tft.println(sgp30.eCO2);
   
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLUE); 
  tft.setCursor(10, 240);
  tft.println("Organic Compounds: ");
  tft.println(sgp30.TVOC);
  delay(1000);  // Update every second
    }
  else
  {
    tft.println("Error!");
     tft.display();

    while (1)
    {
      ;
    }

}

void displayHeader() {
  // Set background color (black)
  tft.fillScreen(ILI9341_BLACK);

  // Display the header
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(20, 10);
  tft.print("CARENUITY.COM");
}

void drawChristmasTree() {
  // Draw the Christmas tree
  tft.fillTriangle(120, 30, 150, 80, 90, 80, ILI9341_GREEN);
  tft.fillTriangle(120, 60, 160, 110, 80, 110, ILI9341_GREEN);
  tft.fillTriangle(120, 90, 170, 140, 70, 140, ILI9341_GREEN);
  
  // Tree trunk
  tft.fillRect(110, 140, 20, 20, tft.color565(139, 69, 19));

  // Christmas decorations
  tft.fillCircle(150, 80, 5, ILI9341_RED);
  tft.fillCircle(92, 80, 5, ILI9341_RED);
  tft.fillCircle(160, 110, 5, ILI9341_PURPLE);
  tft.fillCircle(82, 110, 5, ILI9341_PURPLE);
  tft.fillCircle(170, 140, 5, ILI9341_YELLOW);
  tft.fillCircle(72, 140, 5, ILI9341_YELLOW);
}

void displayCountdown() {
  // Calculate the remaining time to Christmas
  unsigned long currentTime = timeClient.getEpochTime();
  int currentYear = year(currentTime);
  int targetYear = currentYear;  // Change this to the desired year
  tmElements_t targetTime;
  targetTime.Year = CalendarYrToTm(targetYear);
  targetTime.Month = 12;
  targetTime.Day = 25;
  targetTime.Hour = 0;
  targetTime.Minute = 0;
  targetTime.Second = 0;
  unsigned long targetTimestamp = makeTime(targetTime);

  unsigned long remainingSeconds;
  if (currentTime < targetTimestamp) {
    remainingSeconds = targetTimestamp - currentTime;
  } else {
    // If Christmas has passed, calculate the countdown for the next year
    targetTime.Year++;
    targetTimestamp = makeTime(targetTime);
    remainingSeconds = targetTimestamp - currentTime;
  }

  // Display the countdown
  int days = remainingSeconds / 86400;
  int hours = (remainingSeconds % 86400) / 3600;
  int minutes = ((remainingSeconds % 86400) % 3600) / 60;
  int seconds = ((remainingSeconds % 86400) % 3600) % 60;

  // Update the countdown buffer
  snprintf(countdownBuffer, sizeof(countdownBuffer), "Christmas Countdown:");

  // Draw the countdown on the TFT display with different text colors
  tft.setTextSize(2);
  tft.setTextColor(ILI9341_PINK); // "Christmas Countdown" in pink
  tft.setCursor(0, 180);
  tft.print(countdownBuffer);

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_YELLOW); // Days in yellow
  tft.setCursor(10, 210);
  tft.print(days);
  tft.print("d ");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_RED); // Hours in red
  tft.print(hours);
  tft.print("h ");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_BLUE); // Minutes in blue
  tft.print(minutes);
  tft.print("m ");

  tft.setTextSize(2);
  tft.setTextColor(ILI9341_GREEN); // Seconds in green
  tft.print(seconds);
  tft.print("s");
}


   

  
