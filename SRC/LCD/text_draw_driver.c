#include "text_draw_driver.h"
#include "lcd.h"

void LCD_draw_symbol(unsigned char symbol, sFONT font,unsigned char layer,unsigned int start_pix,unsigned int background_color,unsigned int text_color)
{
	unsigned int*  start_symbol_memory_adress;
	unsigned int   font_symbol_index = 0;
	unsigned int   width_pix_index, width_counter;
	unsigned char  bytes_for_width = (font.Width/8)+1;
	unsigned char* symbol_adress;
	if (symbol >= ' ')
	{
		font_symbol_index = (symbol-32)*font.Height*bytes_for_width;
		symbol_adress = (unsigned char*)&(font.table[font_symbol_index]);
	}
	if (layer == 1)
	{
		start_symbol_memory_adress = (unsigned int*)(LAYER1_MEMORY_START_ADRESS + start_pix*4);
	}
	else if (layer == 2)
	{
		start_symbol_memory_adress = (unsigned int*)(LAYER2_MEMORY_START_ADRESS + start_pix*4);
	}
	
	for(unsigned char height_counter = 0; height_counter < font.Height; height_counter++)
	{
		width_pix_index = font.Width;
		width_counter = 8;

		while (width_pix_index > 0)
		{
			if ((*symbol_adress & (1 << (width_counter-1))))
			{
				*(start_symbol_memory_adress) = text_color;
				start_symbol_memory_adress++;
			}
			else if(!(*symbol_adress & (1 << (width_counter-1))))
			{
				*(start_symbol_memory_adress) = background_color;
				start_symbol_memory_adress++;
			}
			width_pix_index--;
			width_counter--;
			if (width_counter == 0)
			{
				width_counter = 8;
				symbol_adress++;
			}
		}
		symbol_adress++;
		start_symbol_memory_adress += (ILI9341_LCD_PIXEL_WIDTH-font.Width);
	}
}
void LCD_draw_text(const char* text, sFONT font,unsigned char layer,unsigned int start_pix,unsigned int background_color,unsigned int text_color)
{
	int text_len = strlen(text);
	for(unsigned int i = 0; i < text_len; i++)
	{
		LCD_draw_symbol(*(text++),font,layer,start_pix+i*font.Width,background_color,text_color);
	}
}
