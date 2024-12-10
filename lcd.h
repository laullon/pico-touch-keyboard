#ifndef _LCD_H_
#define _LCD_H_

#include <stdlib.h>
#include <stdio.h>

#include "lib/lcd/LCD_Driver.h"
#include "LCD_Touch.h"
#include "LCD_BMP.h"

int lcd_test(void);
void draw_keyboard(void);
void read_keyboard(uint32_t blink_interval_ms);

#endif