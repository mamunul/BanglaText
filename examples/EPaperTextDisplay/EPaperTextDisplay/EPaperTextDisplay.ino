#include <Arduino.h>
#include <BanglaText.h>

#include <GxEPD.h>
#include <GxIO/GxIO.h>
#include <GxGDEW075T8/GxGDEW075T8.h> // 7.5" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#include "BanglaMNRegular_25pt.h"

GxIO_Class *io;
GxEPD_Class *display;

// ESP32 epaper pins
//  BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V
#define EPAPER_CS SS
#define EPAPER_DC GPIO_NUM_17
#define EPAPER_RST GPIO_NUM_16
#define EPAPER_BUSY GPIO_NUM_4

void draw_pixel(int16_t x, int16_t y)
{
    display->drawPixel(x, y, GxEPD_BLACK);
}

void drawHelloWorld()
{
    const char HelloWorld[] = "Hello World!";
    // const char HelloWorld[] = "০১২৩৪৫৬৭৮৯ বৈশাখ জ্যৈষ্ঠ আষাঢ় শ্রাবণ ভাদ্র আশ্বিন কার্তিক অগ্রহায়ণ পৌষ মাঘ ফাল্গুন চৈত্র";
    display->setRotation(4);
    // display->setFont(&Charter_Bold12pt7b);
    display->setTextColor(GxEPD_BLACK);
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display->getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center bounding box by transposition of origin:
    uint16_t x = ((display->width() - tbw) / 2) - tbx;
    uint16_t y = ((display->height() - tbh) / 2) - tby;
    display->fillScreen(GxEPD_WHITE);
    display->setCursor(x, y);
    // display->print(HelloWorld);

    BTfont font;
    font = BanglaMNRegular_25pt;

    BanglaTextRenderer *textRenderer = new BanglaTextRenderer(&font);
    textRenderer->renderText("জ্যৈষ্ঠ", draw_pixel);
}
void setup()
{
    Serial.begin(19200);
    io = new GxIO_Class(SPI, /*CS=D8*/ EPAPER_CS, EPAPER_DC /*DC=D3*/, EPAPER_RST /*RST=D4*/);
    display = new GxEPD_Class(*io, /*RST=D4*/ EPAPER_RST, /*BUSY=D2*/ EPAPER_BUSY);

    display->init(115200);

    drawHelloWorld();
    display->update();

    display->powerDown();
}

void loop()
{
}