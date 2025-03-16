#ifndef __SYSH
#define __SYSH

#include <string.h>
#include "main.h"

typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef unsigned char bool;

#define HIGH 1
#define LOW 0

#define false 0
#define true 1

void delay_us(u32 us);
void delay_ms(u32 ms);

#endif