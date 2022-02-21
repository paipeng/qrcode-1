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
char qr_data[128];

char *generate_qrcode(const char *qr_data, int border, int scale, int *qr_size)
{
  char *data = NULL;
  scanner_t scanner;
  scanner.c = 0;
  scanner.verbosity = 0;

  qrc_encode(&scanner, qr_data);
  *qr_size = scanner.s * scale;
  data = (char*) malloc(sizeof(char) * (*qr_size) * (*qr_size));

  for (int i = 0; i < (*qr_size); i+=scale)
  {
    for (int j = 0; j < (*qr_size); j += scale)
    {
      char tmp = scanner.d[i/scale*scanner.s+j/scale];
      for (int k = 0; k < scale; k++)
      {
        for (int l = 0; l < scale; l++)
        {
          data[(i + k)*(*qr_size) + j + l] = tmp;
        }
      }
    }
  }
  
  free(scanner.d);
  return data;
}

void qrcode_update_buffer(const char *qr_data, uint8_t *data, int width, int height, int depth, int scale)
{
  int i, j;
  int k = 0;
  int ox, oy;
  int p;
  int qr_size = 0;

  char *output_data = NULL;

  if (qr_data == NULL)
  {
    return;
  }

  output_data = generate_qrcode(qr_data, 0, scale, &qr_size);

  ox = (width - qr_size) / 2;
  oy = (height - qr_size) / 2;
  for (i = 0; i < height; i++)
  {
    for (j = 0; j < width * depth; j += depth)
    {
      uint8_t pixel_r = 255; // random(255);
      uint8_t pixel_g = 255; // random(255);
      uint8_t pixel_b = 255; // random(255);
      if (j / 2 >= ox && j / 2 < (ox + qr_size) && i >= oy && i < (oy + qr_size))
      {
        p = j / 2 - ox + (i - oy) * qr_size;

        if ((int)(output_data[p]) == 1)
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
}

void genQrcode() {
  snprintf(qr_data, sizeof(char) * 128, "TEST%08d", count);
  count = count + 1;

  qrcode_update_buffer(qr_data, data, SCREEN_WIDTH, SCREEN_HEIGHT, depth, 10);
  //lcd.drawGrayscaleBitmap(0, 0, (uint8_t*)data, SCREEN_WIDTH, SCREEN_HEIGHT);
  lcd.drawImage(0, 0, SCREEN_WIDTH, SCREEN_HEIGHT, (uint16_t*)data);

  lcd.setTextSize(2);
  lcd.setTextColor(COLOR_RED);
  lcd.setCursor(10, 220);
  lcd.println(qr_data);
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
