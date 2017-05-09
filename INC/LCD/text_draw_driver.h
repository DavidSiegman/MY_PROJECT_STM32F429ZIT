#ifndef __TEXT_DRAW_DRIVER_H 
#define __TEXT_DRAW_DRIVER_H

#include "stm32f429xx.h"
#include "fonts.h"
#include <stdio.h>
#include <string.h>

void LCD_draw_symbol(unsigned char symbol, sFONT font,unsigned char layer,unsigned int start_pix,unsigned int background_color,unsigned int text_color);
void LCD_draw_text(const char* text, sFONT font,unsigned char layer,unsigned int start_pix,unsigned int background_color,unsigned int text_color);

#endif /* end __TEXT_DRAW_DRIVER_H */
