

#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <WEMOS_SHT3X.h>

#define OLED_RESET 7  // GPIO0
Adafruit_SSD1306 display(OLED_RESET);

SHT3X sht30(0x45);

void setup() {
  Serial.begin(115200);
Wire.begin(8,10);
  display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
 display.display();
}

void loop() {
  
  // Clear the buffer.
  display.clearDisplay();
  display.setTextSize(1);
  display.setCursor(0, 0);
  display.setTextColor(WHITE);

  if(sht30.get()==0){
    display.println("T: ");
    display.setCursor(35,8);
    display.print("Temp:");
    display.setTextSize(1);
    display.println(sht30.cTemp);

    display.setTextSize(1);
    display.println("H:% ");
    display.setCursor(35,21);
    display.print("Hum:");
    display.setTextSize(1);
    display.println(sht30.humidity);
  }
  else
  {
    display.println("Error!");
  }
  display.display();
  delay(1000);

}
