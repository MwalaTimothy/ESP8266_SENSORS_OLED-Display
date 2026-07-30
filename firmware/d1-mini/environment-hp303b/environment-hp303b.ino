#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <LOLIN_HP303B.h>

LOLIN_HP303B HP303B;

#define OLED_RESET -1
Adafruit_SSD1306 display(OLED_RESET);

void setup()
{

    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.setTextColor(WHITE);

    //Address of the HP303B (0x77 or 0x76)
    HP303B.begin(); // I2C address = 0x77
    // HP303B.begin(0x76); //I2C address = 0x76
}

void loop()
{

    HP303B.begin();
    // Clear the buffer.
    display.clearDisplay();
    display.setCursor(0, 0);
    

    int32_t temperature;
    int32_t pressure;
    int16_t oversampling = 7;
    int16_t ret;

    ret = HP303B.measureTempOnce(temperature, oversampling);

    if (ret != 0)
    {
        display.setTextSize(1);
        display.println("T: error");
    }
    else
    {
        display.setCursor(35,10);
        display.setTextSize(1);
        display.println("Temperature:");
        display.setCursor(45,21);
        display.setTextSize(1);
        display.println(temperature);
         display.display();
            delay(3000);
    }

    //create anew display page to display pressure readings
    ret = HP303B.measurePressureOnce(pressure, oversampling);

    if (ret != 0)
    {
        display.clearDisplay();
        display.setTextSize(1);
        display.println("P: error");
    }
    else
    {
        display.clearDisplay();
        display.setCursor(35,10);
        display.setTextSize(1);
        display.println("Pressure:");
        display.setCursor(40,21);
        display.setTextSize(1);
        display.println(pressure);
         display.display();
    }

    display.display();
    delay(3000);
}
