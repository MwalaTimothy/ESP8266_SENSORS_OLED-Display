#include <Arduino.h>
#include <U8x8lib.h>
#include <OneWire.h>
#include <DallasTemperature.h>

#define ONE_WIRE_BUS D2 // Pin for DS18B20 sensor
#define SCREEN_ADDRESS 0x3C // I2C address for the OLED

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);

#ifdef U8X8_HAVE_HW_SPI
#include <SPI.h>
#endif
#ifdef U8X8_HAVE_HW_I2C
#include <Wire.h>
#endif

  U8X8_SSD1306_128X64_NONAME_HW_I2C u8x8(/* reset=*/ U8X8_PIN_NONE);   

void setup(void) {
 u8x8.begin();// initialize the OLED display
}

void pre(void){
  u8x8.setFont(u8x8_font_amstrad_cpc_extended_f); //Set font
  u8x8.clear(); //clear the display buffer
  u8x8.inverse(); //inverse text color
  u8x8.print(" Carenuity "); // print company name
  u8x8.setFont(u8x8_font_chroma48medium8_r); //set font
  u8x8.noInverse();// disable inverse text color
  u8x8.setCursor(0,1); // set cursor position
}

void draw_bar(uint8_t c, uint8_t is_inverse)
{ 
  uint8_t r;
  u8x8.setInverseFont(is_inverse);
  for( r = 0; r < u8x8.getRows(); r++ )
  {
    u8x8.setCursor(c, r);
    u8x8.print(" ");
  }
}

void loop(void) {
  int i;
  uint8_t c, r, d;
  pre(); //call pre() function to initialize the display
  u8x8.setCursor(4,3); // set cursor position
  u8x8.print("Temp:"); // display Temperature label

sensors.requestTemperatures(); //request Temperature readings from sensors

  u8x8.setCursor(4,5);
  u8x8.print(sensors.getTempCByIndex(0));//Read and display Temperature in degree celcius
  u8x8.print(" C");

  u8x8.setCursor(4,7);
  u8x8.print(sensors.toFahrenheit(sensors.getTempCByIndex(0)));//Read and display Temperature in Fahrenheit
  u8x8.print(" F");

  delay(5000); //delay before next loop iteration
}
