
#ifdef	DISPLAY_LOCAL
#define	DISPLAY_EXT
#else
#define	DISPLAY_EXT extern
#endif

#ifndef DISPLAY_H
#define DISPLAY_H

#include "stdio.h"
#include "lcd.h"

#include "stdint.h"

DISPLAY_EXT uchar const ascii_table_8x16[95][16];
DISPLAY_EXT uchar const ascii_table_5x8[95][5];
DISPLAY_EXT uchar const bmp1[];
DISPLAY_EXT uchar const display_S[];//33*64
DISPLAY_EXT uchar const display_A[];//33*64
DISPLAY_EXT uchar const display_M[];//32*50

DISPLAY_EXT uchar const display_no_speed[];
DISPLAY_EXT uchar const display_low_speed[];
DISPLAY_EXT uchar const display_mid_speed[];
DISPLAY_EXT uchar const display_high_speed[];
DISPLAY_EXT uchar const display_strong_speed[];
DISPLAY_EXT uchar const display_in_dir[];
DISPLAY_EXT uchar const display_out_dir[];
DISPLAY_EXT uchar const display_stright_dir[];
DISPLAY_EXT uchar const display_time_1[];
DISPLAY_EXT uchar const display_time_2[];
DISPLAY_EXT uchar const display_time_3[];
DISPLAY_EXT uchar const display_time_4[];
DISPLAY_EXT uchar const display_time_6[];
DISPLAY_EXT uchar const display_tree[];
#endif
