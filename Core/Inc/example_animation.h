#ifndef __ANNO_DEMO
#define __ANNO_DEMO

#include "mn28016a.h"
#include "main.h"

/**
 * 基础示例：全屏点亮测试
 */
void example_1();

/**
 * 基础示例：水平移动线条
 */
void example_2();

/**
 * 基础示例：垂直移动线条
 */
void example_3();

/**
 * 基础图形：绘制矩形
 * @param x1 左上角x坐标
 * @param y1 左上角y坐标
 * @param x2 右下角x坐标
 * @param y2 右下角y坐标
 */
void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * 基础图形：绘制圆形
 * @param x0 圆心x坐标
 * @param y0 圆心y坐标
 * @param radius 半径
 */
void draw_circle(uint16_t x0, uint16_t y0, uint8_t radius);

/**
 * 基础图形：绘制直线
 * @param x1 起点x坐标
 * @param y1 起点y坐标
 * @param x2 终点x坐标
 * @param y2 终点y坐标
 */
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);

/**
 * 动画示例：波浪效果
 * 在屏幕上显示一个移动的正弦波
 */
void animation_wave();

/**
 * 动画示例：下雨效果
 * 模拟雨滴从屏幕顶部落下的效果
 */
void animation_rain();

/**
 * 动画示例：弹跳球
 * 显示一个在屏幕边界弹跳的小球
 */
void animation_bouncing_ball();

/**
 * 动画示例：星空效果
 * 显示闪烁的星星效果
 */
void animation_starfield();

/**
 * 动画示例：文字滚动
 * 在屏幕上水平滚动显示文字
 * @param text 要显示的文字
 */
void animation_text_scroll(const char* text);

/**
 * 动画示例：扫描线
 * 显示类似于老式CRT显示器的扫描线效果
 */
void animation_scan_line();

/**
 * 动画示例：爆炸效果
 * 从中心向外扩散的爆炸效果
 */
void animation_explosion();

/**
 * 动画示例：蛇形游戏
 * 简单的贪吃蛇游戏演示
 */
void animation_snake_game();

/**
 * 动画示例：矩阵效果
 * 模拟电影《黑客帝国》中的代码雨效果
 */
void animation_matrix();

/**
 * 动画示例：心跳效果
 * 显示一个跳动的心形图案
 */
void animation_heartbeat();

/**
 * 性能测试：帧率测试
 * 在屏幕上实时显示当前帧率
 * @param test_duration 测试持续时间(秒)
 */
void test_framerate(uint16_t test_duration);

#endif