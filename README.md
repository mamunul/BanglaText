# BanglaText

This is a Bangla text rendering library for Arduino based esp supported displays. This library works with Bengali Unicode opentype fonts  which are ttf and otf files. This is tested with BanglaMN, SangamMN, Kohinur and Kalpurush fonts.

The font data is extracted from several libraries, including freetype, harfbuzz, and fonttools. The algorithm is greedy and was initially developed for personal use, so there may be issues. Use it at your own risk and report any issues you find.

[![ESP8266](https://img.shields.io/badge/ESP-8266-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp8266)

[![ESP32](https://img.shields.io/badge/ESP-32-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32)
[![ESP32](https://img.shields.io/badge/ESP-32S2-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32-s2)
[![ESP32](https://img.shields.io/badge/ESP-32C3-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32-c3)
[![ESP32](https://img.shields.io/badge/ESP-32S3-000000.svg?longCache=true&style=flat&colorA=CC101F)](https://www.espressif.com/en/products/socs/esp32-S3)

Works with the [ESP8266 Arduino](https://github.com/esp8266/Arduino) and [ESP32 Arduino](https://github.com/espressif/arduino-esp32) platforms.

## Known Issues

- not listed yet

## TODO List

- Write a Makefile to generate the font file without the help of Xcode.
- Create a compact header that renders only selected text and support AVR MCUs with 32k Flash.
- Reduce the number of steps in the font header generation process.

## Contents

- [Library API](#library-api)
- [Example codes to use the library](#example-codes-to-use-the-library)
  - [Example1](#example1)
  - [Example2](#example2)
- [Process of Font Header generation](#process-of-font-header-generation)
- [Generate Font Header File](#generate-font-header-file)
  - [Generate required xml from ttf file](#generate-required-xml-from-ttf-file)
  - [Generate Font Header File from xml file](#generate-font-header-file-from-xml-file)

## Library API

- include header

```cpp
#include <BanglaText.h> // library header
#include "BanglaMNRegular_25pt.h" // generated font header
```

- font variable

```cpp
BTfont font; // this is from Library Header
font = BanglaMNRegular_25pt; // this variable comes from font header
```

- class init

```cpp
BanglaTextRenderer *textRenderer = new BanglaTextRenderer(&font);
```

- text size measurement

```cpp
//get the required textsize
BTTextSize textSize = textRenderer->getTextSize(""জ্যৈষ্ঠ""); 
```

- text rendering method

```cpp
void draw_pixel(int16_t x, int16_t y)
{
    display->drawPixel(x, y, GxEPD_BLACK);
}

textRenderer->renderText("জ্যৈষ্ঠ", draw_pixel);
```

## Example codes to use the library

Below are two example codes that have been tested for ESP32 and ESP8266 boards in the Arduino IDE and VSCode IDE. You can find these projects in examples folder.

### Example1

In this example, it is shown to use the bangla text library usage in epaper.

```cpp

#include <Arduino.h>
#include <BanglaText.h> //banglaTextLibrary

// Epaper display library
#include <GxEPD.h>
#include <GxIO/GxIO.h>
#include <GxGDEW075T8/GxGDEW075T8.h> // 7.5" b/w
#include <GxIO/GxIO_SPI/GxIO_SPI.h>

#include "BanglaMNRegular_25pt.h"

// ESP32 epaper pins
//  BUSY -> 4, RST -> 16, DC -> 17, CS -> SS(5), CLK -> SCK(18), DIN -> MOSI(23), GND -> GND, 3.3V -> 3.3V
#define EPAPER_CS SS
#define EPAPER_DC GPIO_NUM_17
#define EPAPER_RST GPIO_NUM_16
#define EPAPER_BUSY GPIO_NUM_4

GxIO_Class *io;
GxEPD_Class *display;

void draw_pixel(int16_t x, int16_t y)
{
    display->drawPixel(x, y, GxEPD_BLACK);
}

void drawHelloWorld()
{
    display->setRotation(4);
    display->setTextColor(GxEPD_BLACK);
    int16_t tbx, tby;
    uint16_t tbw, tbh;
    display->getTextBounds(HelloWorld, 0, 0, &tbx, &tby, &tbw, &tbh);
    // center bounding box by transposition of origin:
    uint16_t x = ((display->width() - tbw) / 2) - tbx;
    uint16_t y = ((display->height() - tbh) / 2) - tby;
    display->fillScreen(GxEPD_WHITE);
    display->setCursor(x, y);

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
```

### Example2
In this example, it is shown to use the bangla text library usage in 128x32 I2C OLED Display.
```cpp
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
        display.clearDisplay();
        textRenderer->renderText(banglaMonths[i], draw_pixel);
        display.display();
        delay(2000);
    }
}
void setup()
{
    Serial.begin(19200);

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
```

## Process of Font Header generation

There are two steps to generate a font header file.

1. convert a ttf font file to a xml file
2. from that xml file generate font header file

## Generate Font Header File

Follow the steps thoroughly to generate font header files. This is tested only for the fonts used in this repository. If there is any issue with other fonts, please create an issue.

### Generate required xml from ttf file

you will require python packages ([fonttools](https://pypi.org/project/fonttools/)) installed to use this code. Go to the file _TTFToXMLExporter.py_ in the _extras/python_ folder and edit font information as shown below

```python
kohinur = FontFolder()
kohinur.outputFolderPath = "../FontsProcessed/kohinur/"
kohinur.outputFileName = "kohinur"
kohinur.fontFile = "../Fonts/KohinoorBanglaRegular.otf"
font = kohinur
```

then from the terminal execute that python file

```sh
python3 TTFToXMLExporter
```

This will generate the XML file required for the next step.

### Generate Font Header File from xml file

In this step, an Xcode based project is used to generate the font header file. To execute this code, you will need some libraries([cairo](https://formulae.brew.sh/formula/cairo), [freetype](https://formulae.brew.sh/formula/freetype#default), [harfbuzz](https://formulae.brew.sh/formula/harfbuzz#default), [icu4c](https://formulae.brew.sh/formula/icu4c#default), [jsoncpp](https://formulae.brew.sh/formula/jsoncpp#default)) which can be installed with homebrew. those libraries need to be added in the project configuration.

First open the Xcode project _BanglaFontConverter_ from _extras/BanglaFontConverter_ folder.

there is one Header file named _Header.h_ which contains the font files location you have created from the previous steps, you will need those in this step.

in the main file there are few methods

```cpp
renderTextWithFontHeaderLib(); //test generated header file 
renderAllConjunctWordsWithFontHeaderLib(); //test generated header file 

FontInfo fontInfo = getbanglaMNFont(); //get the font files from previous step

generateFontHeader // this method will generate the header file

```

Run the project, you will get your font header file in _extras/FontsHeader_ folder.

Use that generated header in Arduino project or you can test it in the Xcode project too.
