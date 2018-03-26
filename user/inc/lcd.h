
#ifndef LCD_H 
#define	LCD_H 


#ifdef	THE_LCD
#define	LCD_TYPE_EXT
#else
#define	LCD_TYPE_EXT extern
#endif
#include"stm32f10x.h"

#define uchar unsigned char
#define uint unsigned int
#define ulong unsigned long
// LCD_TYPE_EXT uchar const ascii_table_5x8[95][5];
// LCD_TYPE_EXT uchar const ascii_table_8x16[95][16];
LCD_TYPE_EXT void iic_init(void);
LCD_TYPE_EXT void initial_lcd(void);
LCD_TYPE_EXT void lcd_address(uchar page,uchar column);
LCD_TYPE_EXT void clear_screen(void);
LCD_TYPE_EXT void display_graphic_192x64(const uchar *dp);
LCD_TYPE_EXT void full_display(uchar data_left,uchar data_right);
LCD_TYPE_EXT void display_graphic_32x32(uchar page,uchar column,uchar *dp);
LCD_TYPE_EXT void display_graphic_16x16(uchar page,uchar column,uchar *dp);
LCD_TYPE_EXT void display_graphic_8x16(uchar page,uchar column,uchar *dp);
LCD_TYPE_EXT void display_string_8x16(uint page,uint column,uchar *text);
LCD_TYPE_EXT void display_string_5x8(uint page,uint column,uchar reverse,uchar *text);
LCD_TYPE_EXT void display_string_5x8_1(uint page,uint column,uchar *text);
LCD_TYPE_EXT void display_graphic_33x64(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_32x50(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_speed_33x64(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_win_dir_39x56(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_win_dir_39x64(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_27x64_1(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_27x64_2(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_27x64_single(const uchar  *dp);
LCD_TYPE_EXT void display_graphic_54x64_single_clear(void);
LCD_TYPE_EXT void display_graphic_tree_clear(void);
LCD_TYPE_EXT void display_graphic_tree_32x64(const uchar  *dp);
#endif
