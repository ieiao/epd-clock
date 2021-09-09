#ifndef _FONT_H_
#define _FONT_H_

#include <stdint.h>

#define MAIN_CLOCK_FONT_OFFSET    0x000000ULL
#define CHINESE_FONT_OFFSET       0x004000ULL
#define ASCII_FONT_OFFSET         0x008000ULL

uint32_t get_chinese_offset(int32_t unicode);
uint32_t get_ascii_offset(int32_t unicode);

#endif