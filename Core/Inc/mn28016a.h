/**
 * 算法定义：
 * 1. 缓冲区
 */

#ifndef __MN28016A
#define __MN28016A

#include "main.h"

#define VFD_DISPLAY_WIDTH 280  // 屏幕实际像素长度 X
#define VFD_DISPLAY_HIGHT 16   // 屏幕实际像素宽度 Y

/**
 * 初始化VFD
 */
void init_vfd();

/**
 * 清空全部缓冲区
 */
void clear_buffers();

/**
 * 发送缓冲区数据到屏幕
 */
void send_buffer();

/**
 * 画一个点,起点从0,0开始
 */
void draw_pixel(uint16_t x, uint16_t y);


/**
 * 亮度调整0~255级
 */
void set_brightness(uint8_t level);

/**
 * 内部测试
 */
void test_vfd();

#endif