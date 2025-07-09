#ifndef __DRAW_H__
#define __DRAW_H__

#include <stdint.h>

void drawRectangle(int x, int y, int width, int height, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void drawCircle(int centerX, int centerY, int radius, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void drawLine(int x1, int y1, int x2, int y2, uint8_t r, uint8_t g, uint8_t b, uint8_t a);
void drawPixel(int x, int y, uint8_t r, uint8_t g, uint8_t b, uint8_t a)
;
#endif