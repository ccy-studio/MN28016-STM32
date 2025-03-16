#include "sys.h"

// 24Mhz
static u8 sys_clk_us = 168;

/**
 * Hz代表每秒周期震动1次， 60Hz代表每秒周期震动60次。
 * 假如滴答时钟的频率是72MHZ，72MHz表示每秒钟有72,000,000个时钟周期。
 * 那让滴答时钟计1次，时间过去了1/72μs，也就是一个时钟周期为1/72000000 s =1/72
 us。 ●定时1us，就需要72个时钟周期。 ●定时1s，就需要72000个时钟周期。
 */
void delay_us(u32 us) {
    u32 ticks;
    u32 told, tnow, tcnt = 0;
    u32 reload = SysTick->LOAD;  // LOAD的值
    ticks = us * sys_clk_us;     // 需要的节拍数
    told = SysTick->VAL;         // 刚进入时的计数器值
    while (1) {
        tnow = SysTick->VAL;
        // SysTick是24位向下计数器
        if (tnow != told) {
            if (tnow < told) {
                tcnt += told - tnow;
            } else {
                tcnt += reload - tnow + told;
            }
            told = tnow;
            if (tcnt >= ticks) {
                break;  // 时间超过/等于要延迟的时间,则退出
            }
        }
    }
}
void delay_ms(u32 ms) {
    u32 i;
    for (i = 0; i < ms; i++) {
        delay_us(1000);
    }
}