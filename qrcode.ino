#pragma GCC diagnostic ignored "-Werror=unused-but-set-parameter"
#pragma GCC diagnostic ignored "-Wunused-variable"

#include <Sipeed_ST7789.h>
#include <lcd.h>
#include "encoder.h"

#define SCREEN_WIDTH 320
#define SCREEN_HEIGHT 240
#define GPIO_OUTPUT 4

#define RGB2COLOR(r, g, b) ((((r >> 3) << 11) | ((g >> 2) << 5) | (b >> 3)))

SPIClass spi_(SPI0); // MUST be SPI0 for Maix series on board LCD
Sipeed_ST7789 lcd(SCREEN_WIDTH, SCREEN_HEIGHT, spi_);


uint8_t* data;
int depth = 2;

static int count = 0;
char dm_data[128];

scanner_t *generate_qrcode(const char *data, int border, int scale)
{
  scanner_t *scanner;
  scanner = (scanner_t*)malloc(sizeof(scanner_t));
  scanner->c = 0;
  scanner->verbosity = 0;

  qrc_encode(scanner, data);
  return scanner;
}

void qrcode_update_buffer(const char *dm_data, uint8_t *data, int width, int height, int depth)
{
  int i, j;
  int k = 0;
  int ox, oy;
  int p;

  char *output_data = NULL;

  if (dm_data == NULL)
  {
    return;
  }

  scanner_t * scanner = generate_qrcode(dm_data, 0, 12);

  ox = (width - scanner->s) / 2;
  oy = (height - scanner->s) / 2;
  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width * depth; j += depth)
    {
      uint8_t pixel_r = 255; // random(255);
      uint8_t pixel_g = 255; // random(255);
      uint8_t pixel_b = 255; // random(255);
      if (j / 2 >= ox && j / 2 < (ox + scanner->s) && i >= oy && i < (oy + scanner->s))
      {
        p = j / 2 - ox + (i - oy) * scanner->s;

        if ((int)(scanner->d[p]) == 0)
        {
          pixel_r = 0;
          pixel_g = 0;
          pixel_b = 0;
        }
        else
        {
          pixel_r = 255;
          pixel_g = 255;
          pixel_b = 255;
        }
      }
      unsigned int pixel = (pixel_r & 0xFF << 16) | (pixel_g & 0xFF << 8) | pixel_b & 0xFF;
      unsigned short rgb565 = RGB2COLOR(pixel_r, pixel_g, pixel_b); // RGB888ToRGB565(pixel);
      *(data + i * width * depth + j) = (rgb565 & 0xFF00) >> 8;
      *(data + i * width * depth + j + 1) = rgb565 & 0xFF;
    }
  }
  free(scanner->d);
  free(scanner);
}

void genQrcode() {
  snprintf(dm_data, sizeof(char) * 128, "TEST%08d", count);
  count = count + 1;

  qrcode_update_buffer(dm_data, data, SCREEN_WIDTH, SCREEN_HEIGHT, depth);
  //lcd.drawGrayscaleBitmap(0, 0, (uint8_t*)data, SCREEN_WIDTH, SCREEN_HEIGHT);
  lcd.drawImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t*)data);

  lcd.setTextSize(2);
  lcd.setTextColor(COLOR_RED);
  lcd.setCursor(10, 220);
  lcd.println(dm_data);
}

void setup() {
  // put your setup code here, to run once:
  lcd.begin(15000000, COLOR_RED);
  // put your setup code here, to run once:
  randomSeed(analogRead(0));
  data = (uint8_t*) malloc(SCREEN_WIDTH * SCREEN_HEIGHT * depth * sizeof(uint8_t));
  delay(30);
}

void loop() {
  // put your main code here, to run repeatedly:
  genQrcode();
  delay(1000);
}
