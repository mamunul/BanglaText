#include <Arduino.h>
#include <BanglaText.h>
#include "BanglaMNRegular_25pt.h"

#include <SPI.h>
#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 // OLED display width, in pixels
#define SCREEN_HEIGHT 32 // OLED display height, in pixels

// Declaration for an SSD1306 display connected to I2C (SDA, SCL pins)
#define OLED_RESET -1       // Reset pin # (or -1 if sharing Arduino reset pin)
#define SCREEN_ADDRESS 0x3C ///< See datasheet for Address; 0x3D for 128x64, 0x3C for 128x32

Adafruit_SSD1306 display(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, OLED_RESET);

void draw_pixel(int16_t x, int16_t y)
{
    display.drawPixel(x, y, SSD1306_WHITE);
}

void drawHelloWorld()
{

    BTfont font;
    font = BanglaMNRegular_25pt;
    std::string banglaMonths[] = {"বৈশাখ", "জ্যৈষ্ঠ", "আষাঢ়", "শ্রাবণ", "ভাদ্র", "আশ্বিন", "কার্তিক", "অগ্রহায়ণ", "পৌষ", "মাঘ", "ফাল্গুন", "চৈত্র"};
    BanglaTextRenderer *textRenderer = new BanglaTextRenderer(&font);
    // textRenderer->renderText("জ্যৈষ্ঠ", draw_pixel);
    for (int i = 0; i < 12; i++)
    {
        // int i = 0;
        display.clearDisplay();
        textRenderer->renderText(banglaMonths[i], draw_pixel);
        display.display();
        delay(2000);
    }
}
void setup()
{
    Serial.begin(19200);

    // SSD1306_SWITCHCAPVCC = generate display voltage from 3.3V internally
    if (!display.begin(SSD1306_SWITCHCAPVCC, SCREEN_ADDRESS))
    {
        Serial.println(F("SSD1306 allocation failed"));
    }
    delay(2000);
    display.clearDisplay();

    display.setTextSize(1);
    display.setTextColor(WHITE);
    display.setCursor(0, 10);
    display.setRotation(1);
    for (;;)
        drawHelloWorld();
}

void loop()
{
}