#include "example_animation.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>
#include "sys.h"

/**
 * 基础示例：全屏点亮测试
 */
void example_1() {
    printf("开始全屏点亮测试\n");

    // 清空缓冲区
    clear_buffers();

    // 设置所有像素点
    for (int y = 0; y < VFD_DISPLAY_HIGHT; y++) {
        for (int x = 0; x < VFD_DISPLAY_WIDTH; x++) {
            draw_pixel(x, y);
        }
    }

    // 发送到屏幕
    send_buffer();

    printf("全屏点亮测试完成\n");
}

/**
 * 基础示例：水平移动线条
 */
void example_2() {
    printf("开始水平移动线条测试\n");

    for (int x = 0; x < VFD_DISPLAY_WIDTH; x++) {
        // 清空缓冲区
        clear_buffers();

        // 绘制垂直线
        for (int y = 0; y < VFD_DISPLAY_HIGHT; y++) {
            draw_pixel(x, y);
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(15);
    }

    printf("水平移动线条测试完成\n");
}

/**
 * 基础示例：垂直移动线条
 */
void example_3() {
    printf("开始垂直移动线条测试\n");

    for (int y = 0; y < VFD_DISPLAY_HIGHT; y++) {
        // 清空缓冲区
        clear_buffers();

        // 绘制水平线
        for (int x = 0; x < VFD_DISPLAY_WIDTH; x++) {
            draw_pixel(x, y);
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(100);
    }

    printf("垂直移动线条测试完成\n");
}

/**
 * 基础图形：绘制矩形
 */
void draw_rectangle(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    // 确保坐标在有效范围内
    if (x1 >= VFD_DISPLAY_WIDTH || y1 >= VFD_DISPLAY_HIGHT ||
        x2 >= VFD_DISPLAY_WIDTH || y2 >= VFD_DISPLAY_HIGHT) {
        return;
    }

    // 绘制水平线
    for (int x = x1; x <= x2; x++) {
        draw_pixel(x, y1);
        draw_pixel(x, y2);
    }

    // 绘制垂直线
    for (int y = y1; y <= y2; y++) {
        draw_pixel(x1, y);
        draw_pixel(x2, y);
    }
}

/**
 * 基础图形：绘制圆形
 * 使用Bresenham算法
 */
void draw_circle(uint16_t x0, uint16_t y0, uint8_t radius) {
    int x = radius;
    int y = 0;
    int err = 0;

    while (x >= y) {
        draw_pixel(x0 + x, y0 + y);
        draw_pixel(x0 + y, y0 + x);
        draw_pixel(x0 - y, y0 + x);
        draw_pixel(x0 - x, y0 + y);
        draw_pixel(x0 - x, y0 - y);
        draw_pixel(x0 - y, y0 - x);
        draw_pixel(x0 + y, y0 - x);
        draw_pixel(x0 + x, y0 - y);

        if (err <= 0) {
            y += 1;
            err += 2 * y + 1;
        }

        if (err > 0) {
            x -= 1;
            err -= 2 * x + 1;
        }
    }
}

/**
 * 基础图形：绘制直线
 * 使用Bresenham算法
 */
void draw_line(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;
    int err = dx - dy;
    int e2;

    while (1) {
        draw_pixel(x1, y1);

        if (x1 == x2 && y1 == y2) {
            break;
        }

        e2 = 2 * err;

        if (e2 > -dy) {
            err -= dy;
            x1 += sx;
        }

        if (e2 < dx) {
            err += dx;
            y1 += sy;
        }
    }
}

/**
 * 动画示例：波浪效果
 */
void animation_wave() {
    printf("开始波浪效果动画\n");

    // 波浪参数
    float amplitude = 4.0f;  // 振幅
    float frequency = 0.1f;  // 频率
    float phase = 0.0f;      // 相位

    // 动画循环
    for (int frame = 0; frame < 100; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 计算波浪
        for (int x = 0; x < VFD_DISPLAY_WIDTH; x++) {
            // 计算y坐标 (正弦波)
            int y = (VFD_DISPLAY_HIGHT / 2) +
                    (int)(amplitude * sin(frequency * x + phase));

            // 确保y在有效范围内
            if (y >= 0 && y < VFD_DISPLAY_HIGHT) {
                draw_pixel(x, y);
            }
        }

        // 更新相位
        phase += 0.2f;

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(50);
    }

    printf("波浪效果动画完成\n");
}

/**
 * 动画示例：下雨效果
 */
void animation_rain() {
    printf("开始下雨效果动画\n");

// 雨滴数量
#define MAX_RAINDROPS 50

    // 雨滴结构
    typedef struct {
        int x;
        int y;
        int speed;
        int active;
    } Raindrop;

    // 初始化雨滴
    Raindrop raindrops[MAX_RAINDROPS];
    for (int i = 0; i < MAX_RAINDROPS; i++) {
        raindrops[i].active = 0;
    }

    // 动画循环
    for (int frame = 0; frame < 200; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 随机生成新雨滴
        if (rand() % 5 == 0) {
            for (int i = 0; i < MAX_RAINDROPS; i++) {
                if (!raindrops[i].active) {
                    raindrops[i].x = rand() % VFD_DISPLAY_WIDTH;
                    raindrops[i].y = 0;
                    raindrops[i].speed = 1 + (rand() % 2);
                    raindrops[i].active = 1;
                    break;
                }
            }
        }

        // 更新和绘制雨滴
        for (int i = 0; i < MAX_RAINDROPS; i++) {
            if (raindrops[i].active) {
                // 绘制雨滴
                draw_pixel(raindrops[i].x, raindrops[i].y);

                // 更新位置
                raindrops[i].y += raindrops[i].speed;

                // 检查是否超出屏幕
                if (raindrops[i].y >= VFD_DISPLAY_HIGHT) {
                    raindrops[i].active = 0;
                }
            }
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(50);
    }

    printf("下雨效果动画完成\n");
}

/**
 * 动画示例：弹跳球
 */
void animation_bouncing_ball() {
    printf("开始弹跳球动画\n");

    // 球的参数
    int x = VFD_DISPLAY_WIDTH / 4;
    int y = VFD_DISPLAY_HIGHT / 2;
    int dx = 1;
    int dy = 1;
    int radius = 3;

    // 动画循环
    for (int frame = 0; frame < 300; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 绘制球
        draw_circle(x, y, radius);

        // 更新位置
        x += dx;
        y += dy;

        // 检查边界碰撞
        if (x <= radius || x >= VFD_DISPLAY_WIDTH - radius) {
            dx = -dx;
        }
        if (y <= radius || y >= VFD_DISPLAY_HIGHT - radius) {
            dy = -dy;
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        delay_ms(20);
    }

    printf("弹跳球动画完成\n");
}

/**
 * 动画示例：星空效果
 */
void animation_starfield() {
    printf("开始星空效果动画\n");

// 星星数量
#define MAX_STARS 50

    // 星星结构
    typedef struct {
        int x;
        int y;
        int brightness;
        int direction;
    } Star;

    // 初始化星星
    Star stars[MAX_STARS];
    for (int i = 0; i < MAX_STARS; i++) {
        stars[i].x = rand() % VFD_DISPLAY_WIDTH;
        stars[i].y = rand() % VFD_DISPLAY_HIGHT;
        stars[i].brightness = rand() % 2;
        stars[i].direction = (rand() % 2) ? 1 : -1;
    }

    // 动画循环
    for (int frame = 0; frame < 60; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 更新和绘制星星
        for (int i = 0; i < MAX_STARS; i++) {
            // 只绘制亮的星星
            if (stars[i].brightness > 0) {
                draw_pixel(stars[i].x, stars[i].y);
            }

            // 更新亮度
            stars[i].brightness += stars[i].direction;

            // 改变方向
            if (stars[i].brightness <= 0 || stars[i].brightness >= 2) {
                stars[i].direction = -stars[i].direction;
            }
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        delay_ms(100);
    }

    printf("星空效果动画完成\n");
}

/**
 * 动画示例：文字滚动
 * 简单实现，仅支持基本字符
 */
void animation_text_scroll(const char* text) {
    printf("开始文字滚动动画\n");

// 字体宽度和高度
#define FONT_WIDTH 5
#define FONT_HEIGHT 7

// 字符间距
#define CHAR_SPACING 1

    // 计算文本总宽度
    int text_length = strlen(text);
    int total_width = text_length * (FONT_WIDTH + CHAR_SPACING);

    // 简单的5x7点阵字体 (仅包含部分ASCII字符)
    // 每个字符由5个字节表示，每个字节的7个位表示一列
    const uint8_t font[128][5] = {// 空格 (32)
                                  {0x00, 0x00, 0x00, 0x00, 0x00},
                                  // ! (33)
                                  {0x00, 0x00, 0x5F, 0x00, 0x00},
                                  // " (34)
                                  {0x00, 0x07, 0x00, 0x07, 0x00},
                                  // # (35)
                                  {0x14, 0x7F, 0x14, 0x7F, 0x14},
                                  // $ (36)
                                  {0x24, 0x2A, 0x7F, 0x2A, 0x12},
                                  // % (37)
                                  {0x23, 0x13, 0x08, 0x64, 0x62},
                                  // & (38)
                                  {0x36, 0x49, 0x55, 0x22, 0x50},
                                  // ' (39)
                                  {0x00, 0x05, 0x03, 0x00, 0x00},
                                  // ( (40)
                                  {0x00, 0x1C, 0x22, 0x41, 0x00},
                                  // ) (41)
                                  {0x00, 0x41, 0x22, 0x1C, 0x00},
                                  // * (42)
                                  {0x14, 0x08, 0x3E, 0x08, 0x14},
                                  // + (43)
                                  {0x08, 0x08, 0x3E, 0x08, 0x08},
                                  // , (44)
                                  {0x00, 0x50, 0x30, 0x00, 0x00},
                                  // - (45)
                                  {0x08, 0x08, 0x08, 0x08, 0x08},
                                  // . (46)
                                  {0x00, 0x60, 0x60, 0x00, 0x00},
                                  // / (47)
                                  {0x20, 0x10, 0x08, 0x04, 0x02},
                                  // 0-9 (48-57)
                                  {0x3E, 0x51, 0x49, 0x45, 0x3E},
                                  {0x00, 0x42, 0x7F, 0x40, 0x00},
                                  {0x42, 0x61, 0x51, 0x49, 0x46},
                                  {0x21, 0x41, 0x45, 0x4B, 0x31},
                                  {0x18, 0x14, 0x12, 0x7F, 0x10},
                                  {0x27, 0x45, 0x45, 0x45, 0x39},
                                  {0x3C, 0x4A, 0x49, 0x49, 0x30},
                                  {0x01, 0x71, 0x09, 0x05, 0x03},
                                  {0x36, 0x49, 0x49, 0x49, 0x36},
                                  {0x06, 0x49, 0x49, 0x29, 0x1E},
                                  // : (58)
                                  {0x00, 0x36, 0x36, 0x00, 0x00},
                                  // ; (59)
                                  {0x00, 0x56, 0x36, 0x00, 0x00},
                                  // < (60)
                                  {0x08, 0x14, 0x22, 0x41, 0x00},
                                  // = (61)
                                  {0x14, 0x14, 0x14, 0x14, 0x14},
                                  // > (62)
                                  {0x00, 0x41, 0x22, 0x14, 0x08},
                                  // ? (63)
                                  {0x02, 0x01, 0x51, 0x09, 0x06},
                                  // @ (64)
                                  {0x32, 0x49, 0x79, 0x41, 0x3E},
                                  // A-Z (65-90)
                                  {0x7E, 0x11, 0x11, 0x11, 0x7E},
                                  {0x7F, 0x49, 0x49, 0x49, 0x36},
                                  {0x3E, 0x41, 0x41, 0x41, 0x22},
                                  {0x7F, 0x41, 0x41, 0x22, 0x1C},
                                  {0x7F, 0x49, 0x49, 0x49, 0x41},
                                  {0x7F, 0x09, 0x09, 0x09, 0x01},
                                  {0x3E, 0x41, 0x49, 0x49, 0x7A},
                                  {0x7F, 0x08, 0x08, 0x08, 0x7F},
                                  {0x00, 0x41, 0x7F, 0x41, 0x00},
                                  {0x20, 0x40, 0x41, 0x3F, 0x01},
                                  {0x7F, 0x08, 0x14, 0x22, 0x41},
                                  {0x7F, 0x40, 0x40, 0x40, 0x40},
                                  {0x7F, 0x02, 0x0C, 0x02, 0x7F},
                                  {0x7F, 0x04, 0x08, 0x10, 0x7F},
                                  {0x3E, 0x41, 0x41, 0x41, 0x3E},
                                  {0x7F, 0x09, 0x09, 0x09, 0x06},
                                  {0x3E, 0x41, 0x51, 0x21, 0x5E},
                                  {0x7F, 0x09, 0x19, 0x29, 0x46},
                                  {0x46, 0x49, 0x49, 0x49, 0x31},
                                  {0x01, 0x01, 0x7F, 0x01, 0x01},
                                  {0x3F, 0x40, 0x40, 0x40, 0x3F},
                                  {0x1F, 0x20, 0x40, 0x20, 0x1F},
                                  {0x3F, 0x40, 0x38, 0x40, 0x3F},
                                  {0x63, 0x14, 0x08, 0x14, 0x63},
                                  {0x07, 0x08, 0x70, 0x08, 0x07},
                                  {0x61, 0x51, 0x49, 0x45, 0x43}};

    // 动画循环
    for (int offset = VFD_DISPLAY_WIDTH; offset > -total_width; offset--) {
        // 清空缓冲区
        clear_buffers();

        // 绘制文本
        for (int i = 0; i < text_length; i++) {
            // 计算字符位置
            int char_x = offset + i * (FONT_WIDTH + CHAR_SPACING);

            // 如果字符在屏幕范围内
            if (char_x > -FONT_WIDTH && char_x < VFD_DISPLAY_WIDTH) {
                // 获取字符ASCII码
                char c = text[i];

                // 确保字符在有效范围内
                if (c >= 32 && c < 128) {
                    // 绘制字符
                    for (int col = 0; col < FONT_WIDTH; col++) {
                        for (int row = 0; row < FONT_HEIGHT; row++) {
                            if (font[c - 32][col] & (1 << row)) {
                                int x = char_x + col;
                                int y =
                                    (VFD_DISPLAY_HIGHT - FONT_HEIGHT) / 2 + row;

                                // 确保像素在屏幕范围内
                                if (x >= 0 && x < VFD_DISPLAY_WIDTH && y >= 0 &&
                                    y < VFD_DISPLAY_HIGHT) {
                                    draw_pixel(x, y);
                                }
                            }
                        }
                    }
                }
            }
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(15);
    }

    printf("文字滚动动画完成\n");
}

/**
 * 动画示例：扫描线
 */
void animation_scan_line() {
    printf("开始扫描线动画\n");

    // 扫描线参数
    int scan_line = 0;
    int direction = 1;

    // 动画循环
    for (int frame = 0; frame < 100; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 绘制扫描线
        for (int x = 0; x < VFD_DISPLAY_WIDTH; x++) {
            draw_pixel(x, scan_line);
        }

        // 绘制一些静态内容
        draw_rectangle(10, 2, 50, 10);
        draw_circle(100, 8, 5);
        draw_line(150, 2, 200, 14);

        // 更新扫描线位置
        scan_line += direction;

        // 检查边界
        if (scan_line >= VFD_DISPLAY_HIGHT - 1 || scan_line <= 0) {
            direction = -direction;
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(100);
    }

    printf("扫描线动画完成\n");
}

/**
 * 动画示例：爆炸效果
 */
void animation_explosion() {
    printf("开始爆炸效果动画\n");

    // 爆炸中心
    int center_x = VFD_DISPLAY_WIDTH / 2;
    int center_y = VFD_DISPLAY_HIGHT / 2;

// 粒子数量
#define MAX_PARTICLES 50

    // 粒子结构
    typedef struct {
        float x;
        float y;
        float dx;
        float dy;
        int life;
    } Particle;

    // 初始化粒子
    Particle particles[MAX_PARTICLES];
    for (int i = 0; i < MAX_PARTICLES; i++) {
        // 随机角度
        float angle = (float)(rand() % 360) * 3.14159f / 180.0f;

        // 随机速度
        float speed = 0.5f + (float)(rand() % 20) / 10.0f;

        particles[i].x = center_x;
        particles[i].y = center_y;
        particles[i].dx = cos(angle) * speed;
        particles[i].dy = sin(angle) * speed;
        particles[i].life = 20 + (rand() % 30);
    }

    // 动画循环
    for (int frame = 0; frame < 50; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 更新和绘制粒子
        for (int i = 0; i < MAX_PARTICLES; i++) {
            if (particles[i].life > 0) {
                // 绘制粒子
                int x = (int)particles[i].x;
                int y = (int)particles[i].y;

                if (x >= 0 && x < VFD_DISPLAY_WIDTH && y >= 0 &&
                    y < VFD_DISPLAY_HIGHT) {
                    draw_pixel(x, y);
                }

                // 更新位置
                particles[i].x += particles[i].dx;
                particles[i].y += particles[i].dy;

                // 减少生命值
                particles[i].life--;
            }
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(50);
    }

    printf("爆炸效果动画完成\n");
}

/**
 * 动画示例：蛇形游戏
 */
void animation_snake_game() {
    printf("开始蛇形游戏动画\n");

// 蛇的最大长度
#define MAX_SNAKE_LENGTH 100

    // 蛇的结构
    typedef struct {
        int x[MAX_SNAKE_LENGTH];
        int y[MAX_SNAKE_LENGTH];
        int length;
        int dx;
        int dy;
    } Snake;

    // 食物结构
    typedef struct {
        int x;
        int y;
    } Food;

    // 初始化蛇
    Snake snake;
    snake.length = 3;
    snake.dx = 1;
    snake.dy = 0;

    // 初始蛇的位置
    for (int i = 0; i < snake.length; i++) {
        snake.x[i] = 10 - i;
        snake.y[i] = 8;
    }

    // 初始化食物
    Food food;
    food.x = rand() % (VFD_DISPLAY_WIDTH - 10) + 5;
    food.y = rand() % (VFD_DISPLAY_HIGHT - 4) + 2;

    // 游戏状态
    int game_over = 0;
    int score = 0;

    // 游戏循环
    for (int frame = 0; frame < 2000 && !game_over; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 每5帧更新一次蛇的位置
        if (frame % 5 == 0) {
            // 移动蛇身体
            for (int i = snake.length - 1; i > 0; i--) {
                snake.x[i] = snake.x[i - 1];
                snake.y[i] = snake.y[i - 1];
            }

            // 移动蛇头
            snake.x[0] += snake.dx;
            snake.y[0] += snake.dy;

            // 随机改变方向
            if (rand() % 10 == 0) {
                // 保存当前方向
                int old_dx = snake.dx;
                int old_dy = snake.dy;

                // 随机选择新方向
                int dir = rand() % 4;
                switch (dir) {
                    case 0:  // 上
                        snake.dx = 0;
                        snake.dy = -1;
                        break;
                    case 1:  // 右
                        snake.dx = 1;
                        snake.dy = 0;
                        break;
                    case 2:  // 下
                        snake.dx = 0;
                        snake.dy = 1;
                        break;
                    case 3:  // 左
                        snake.dx = -1;
                        snake.dy = 0;
                        break;
                }

                // 避免180度转弯
                if (snake.dx == -old_dx && snake.dy == -old_dy) {
                    snake.dx = old_dx;
                    snake.dy = old_dy;
                }
            }

            // 检查是否吃到食物
            if (snake.x[0] == food.x && snake.y[0] == food.y) {
                // 增加蛇的长度
                if (snake.length < MAX_SNAKE_LENGTH) {
                    snake.length++;
                    score++;
                }

                // 生成新食物
                food.x = rand() % (VFD_DISPLAY_WIDTH - 10) + 5;
                food.y = rand() % (VFD_DISPLAY_HIGHT - 4) + 2;
            }

            // 检查是否撞墙
            if (snake.x[0] < 0 || snake.x[0] >= VFD_DISPLAY_WIDTH ||
                snake.y[0] < 0 || snake.y[0] >= VFD_DISPLAY_HIGHT) {
                game_over = 1;
            }

            // 检查是否撞到自己
            for (int i = 1; i < snake.length; i++) {
                if (snake.x[0] == snake.x[i] && snake.y[0] == snake.y[i]) {
                    game_over = 1;
                    break;
                }
            }
        }

        // 绘制蛇
        for (int i = 0; i < snake.length; i++) {
            if (i == 0) {
                // 蛇头
                draw_pixel(snake.x[i], snake.y[i]);
                // 绘制眼睛
                if (snake.dx == 1) {
                    draw_pixel(snake.x[i] + 1, snake.y[i] - 1);
                } else if (snake.dx == -1) {
                    draw_pixel(snake.x[i] - 1, snake.y[i] - 1);
                } else if (snake.dy == 1) {
                    draw_pixel(snake.x[i] + 1, snake.y[i] + 1);
                } else {
                    draw_pixel(snake.x[i] + 1, snake.y[i] - 1);
                }
            } else {
                // 蛇身
                draw_pixel(snake.x[i], snake.y[i]);
            }
        }

        // 绘制食物
        draw_pixel(food.x, food.y);
        draw_pixel(food.x + 1, food.y);
        draw_pixel(food.x, food.y + 1);

        // 绘制边界
        for (int x = 0; x < VFD_DISPLAY_WIDTH; x++) {
            draw_pixel(x, 0);
            draw_pixel(x, VFD_DISPLAY_HIGHT - 1);
        }
        for (int y = 0; y < VFD_DISPLAY_HIGHT; y++) {
            draw_pixel(0, y);
            draw_pixel(VFD_DISPLAY_WIDTH - 1, y);
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        delay_ms(10);
    }

    // 游戏结束显示
    if (game_over) {
        // 清空缓冲区
        clear_buffers();

        // 绘制"GAME OVER"
        animation_text_scroll("GAME OVER");
    }

    printf("蛇形游戏动画完成，得分：%d\n", score);
}

/**
 * 动画示例：矩阵效果
 * 模拟电影《黑客帝国》中的代码雨效果
 */
void animation_matrix() {
    printf("开始矩阵效果动画\n");

// 定义列数
#define NUM_COLUMNS 40

    // 定义每列的状态
    typedef struct {
        int y;       // 当前位置
        int speed;   // 下落速度
        int length;  // 尾巴长度
        int active;  // 是否激活
        int wait;    // 等待时间
    } Column;

    // 初始化列
    Column columns[NUM_COLUMNS];
    for (int i = 0; i < NUM_COLUMNS; i++) {
        columns[i].active = 0;
        columns[i].wait = rand() % 50;
    }

    // 动画循环
    for (int frame = 0; frame < 300; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 更新和绘制每列
        for (int i = 0; i < NUM_COLUMNS; i++) {
            // 计算x坐标
            int x = i * (VFD_DISPLAY_WIDTH / NUM_COLUMNS);

            // 如果列未激活
            if (!columns[i].active) {
                // 减少等待时间
                if (columns[i].wait > 0) {
                    columns[i].wait--;
                } else {
                    // 激活列
                    columns[i].active = 1;
                    columns[i].y = 0;
                    columns[i].speed = 1 + (rand() % 2);
                    columns[i].length = 3 + (rand() % 5);
                }
            } else {
                // 绘制列
                for (int j = 0; j < columns[i].length; j++) {
                    int y = columns[i].y - j;
                    if (y >= 0 && y < VFD_DISPLAY_HIGHT) {
                        // 头部亮一些
                        if (j == 0) {
                            draw_pixel(x, y);
                            if (x + 1 < VFD_DISPLAY_WIDTH) {
                                draw_pixel(x + 1, y);
                            }
                        } else {
                            draw_pixel(x, y);
                        }
                    }
                }

                // 更新位置
                columns[i].y += columns[i].speed;

                // 检查是否超出屏幕
                if (columns[i].y - columns[i].length > VFD_DISPLAY_HIGHT) {
                    columns[i].active = 0;
                    columns[i].wait = rand() % 50;
                }
            }
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        delay_ms(50);
    }

    printf("矩阵效果动画完成\n");
}

/**
 * 动画示例：心跳效果
 */
void animation_heartbeat() {
    printf("开始心跳效果动画\n");

    // 心形坐标点
    const uint8_t heart_x[] = {
        8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 7,  8,  9,  10, 11, 12, 13, 14,
        15, 16, 17, 18, 6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19,
        6,  7,  8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 6,  7,  8,  9,
        10, 11, 12, 13, 14, 15, 16, 17, 18, 19, 7,  8,  9,  10, 11, 12, 13, 14,
        15, 16, 17, 18, 8,  9,  10, 11, 12, 13, 14, 15, 16, 17, 9,  10, 11, 12,
        13, 14, 15, 16, 10, 11, 12, 13, 14, 15, 11, 12, 13, 14};

    const uint8_t heart_y[] = {
        2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 3,  3,  3,  3,  3,  3,  3,  3,  3,  3, 3,
        3, 4, 4, 4, 4, 4, 4, 4, 4, 4, 4,  4,  4,  4,  4,  5,  5,  5,  5,  5, 5,
        5, 5, 5, 5, 5, 5, 5, 5, 6, 6, 6,  6,  6,  6,  6,  6,  6,  6,  6,  6, 6,
        6, 7, 7, 7, 7, 7, 7, 7, 7, 7, 7,  7,  7,  8,  8,  8,  8,  8,  8,  8, 8,
        8, 8, 9, 9, 9, 9, 9, 9, 9, 9, 10, 10, 10, 10, 10, 10, 11, 11, 11, 11};

    const int heart_points = sizeof(heart_x) / sizeof(heart_x[0]);

    // 心跳参数
    float scale = 1.0f;
    float scale_change = 0.05f;
    int center_x = VFD_DISPLAY_WIDTH / 2;
    int center_y = VFD_DISPLAY_HIGHT / 2;

    // 动画循环
    for (int frame = 0; frame < 100; frame++) {
        // 清空缓冲区
        clear_buffers();

        // 绘制心形
        for (int i = 0; i < heart_points; i++) {
            // 计算缩放后的坐标
            int x = center_x + (int)((heart_x[i] - 13) * scale);
            int y = center_y + (int)((heart_y[i] - 7) * scale);

            // 确保坐标在屏幕范围内
            if (x >= 0 && x < VFD_DISPLAY_WIDTH && y >= 0 &&
                y < VFD_DISPLAY_HIGHT) {
                draw_pixel(x, y);
            }
        }

        // 更新缩放
        scale += scale_change;

        // 改变缩放方向
        if (scale >= 1.3f || scale <= 0.8f) {
            scale_change = -scale_change;
        }

        // 发送到屏幕
        send_buffer();

        // 延时
        HAL_Delay(50);
    }

    printf("心跳效果动画完成\n");
}

// 更新帧率显示
void update_fps(uint32_t* frame_count,
                uint32_t* last_time,
                float* fps,
                float* min_fps,
                float* max_fps,
                float* avg_fps,
                char* fps_text) {
    (*frame_count)++;
    uint32_t current_time = HAL_GetTick();
    uint32_t elapsed_time = current_time - *last_time;

    // 每秒更新一次帧率显示
    if (elapsed_time >= 1000) {
        *fps = (float)(*frame_count) * 1000.0f / (float)elapsed_time;

        // 更新最大/最小帧率
        if (*fps > *max_fps)
            *max_fps = *fps;
        if (*fps < *min_fps)
            *min_fps = *fps;

        // 累加平均帧率计算
        *avg_fps = (*avg_fps * 0.7f) + (*fps * 0.3f);

        *frame_count = 0;
        *last_time = current_time;

        // 格式化帧率文本
        sprintf(fps_text, "FPS: %.1f", *fps);
        printf("当前帧率: %.1f FPS\n", *fps);
    }
}

// 绘制文本函数
void draw_text(const char* text, int x, int y, const uint8_t font[][5]) {
    for (int i = 0; i < strlen(text); i++) {
        char c = text[i];

        // 确保字符在有效范围内
        if (c >= 32 && c < 128) {
            // 绘制字符
            for (int col = 0; col < FONT_WIDTH; col++) {
                for (int row = 0; row < FONT_HEIGHT; row++) {
                    if (font[c - 32][col] & (1 << row)) {
                        int px = x + i * (FONT_WIDTH + CHAR_SPACING) + col;
                        int py = y + row;

                        // 确保像素在屏幕范围内
                        if (px >= 0 && px < VFD_DISPLAY_WIDTH && py >= 0 &&
                            py < VFD_DISPLAY_HIGHT) {
                            draw_pixel(px, py);
                        }
                    }
                }
            }
        }
    }
}

/**
 * 性能测试：帧率测试
 * 在屏幕上实时显示当前帧率，包含多个测试场景
 */
void test_framerate(uint16_t test_duration) {
    printf("开始帧率测试，持续%d秒/场景\n", test_duration);

    // 帧率计算变量
    uint32_t frame_count = 0;
    uint32_t last_time = HAL_GetTick();
    float fps = 0;
    float min_fps = 999.0f;
    float max_fps = 0.0f;
    float avg_fps = 0.0f;
    char fps_text[20] = "FPS: 0.0";

// 字体参数
#define FONT_WIDTH 5
#define FONT_HEIGHT 7
#define CHAR_SPACING 1

    // 简单的5x7点阵字体 (仅包含部分ASCII字符)
    const uint8_t font[128][5] = {// 空格 (32)
                                  {0x00, 0x00, 0x00, 0x00, 0x00},
                                  // ! (33)
                                  {0x00, 0x00, 0x5F, 0x00, 0x00},
                                  // " (34)
                                  {0x00, 0x07, 0x00, 0x07, 0x00},
                                  // # (35)
                                  {0x14, 0x7F, 0x14, 0x7F, 0x14},
                                  // $ (36)
                                  {0x24, 0x2A, 0x7F, 0x2A, 0x12},
                                  // % (37)
                                  {0x23, 0x13, 0x08, 0x64, 0x62},
                                  // & (38)
                                  {0x36, 0x49, 0x55, 0x22, 0x50},
                                  // ' (39)
                                  {0x00, 0x05, 0x03, 0x00, 0x00},
                                  // ( (40)
                                  {0x00, 0x1C, 0x22, 0x41, 0x00},
                                  // ) (41)
                                  {0x00, 0x41, 0x22, 0x1C, 0x00},
                                  // * (42)
                                  {0x14, 0x08, 0x3E, 0x08, 0x14},
                                  // + (43)
                                  {0x08, 0x08, 0x3E, 0x08, 0x08},
                                  // , (44)
                                  {0x00, 0x50, 0x30, 0x00, 0x00},
                                  // - (45)
                                  {0x08, 0x08, 0x08, 0x08, 0x08},
                                  // . (46)
                                  {0x00, 0x60, 0x60, 0x00, 0x00},
                                  // / (47)
                                  {0x20, 0x10, 0x08, 0x04, 0x02},
                                  // 0-9 (48-57)
                                  {0x3E, 0x51, 0x49, 0x45, 0x3E},
                                  {0x00, 0x42, 0x7F, 0x40, 0x00},
                                  {0x42, 0x61, 0x51, 0x49, 0x46},
                                  {0x21, 0x41, 0x45, 0x4B, 0x31},
                                  {0x18, 0x14, 0x12, 0x7F, 0x10},
                                  {0x27, 0x45, 0x45, 0x45, 0x39},
                                  {0x3C, 0x4A, 0x49, 0x49, 0x30},
                                  {0x01, 0x71, 0x09, 0x05, 0x03},
                                  {0x36, 0x49, 0x49, 0x49, 0x36},
                                  {0x06, 0x49, 0x49, 0x29, 0x1E},
                                  // : (58)
                                  {0x00, 0x36, 0x36, 0x00, 0x00},
                                  // ; (59)
                                  {0x00, 0x56, 0x36, 0x00, 0x00},
                                  // < (60)
                                  {0x08, 0x14, 0x22, 0x41, 0x00},
                                  // = (61)
                                  {0x14, 0x14, 0x14, 0x14, 0x14},
                                  // > (62)
                                  {0x00, 0x41, 0x22, 0x14, 0x08},
                                  // ? (63)
                                  {0x02, 0x01, 0x51, 0x09, 0x06},
                                  // @ (64)
                                  {0x32, 0x49, 0x79, 0x41, 0x3E},
                                  // A-Z (65-90)
                                  {0x7E, 0x11, 0x11, 0x11, 0x7E},
                                  {0x7F, 0x49, 0x49, 0x49, 0x36},
                                  {0x3E, 0x41, 0x41, 0x41, 0x22},
                                  {0x7F, 0x41, 0x41, 0x22, 0x1C},
                                  {0x7F, 0x49, 0x49, 0x49, 0x41},
                                  {0x7F, 0x09, 0x09, 0x09, 0x01},
                                  {0x3E, 0x41, 0x49, 0x49, 0x7A},
                                  {0x7F, 0x08, 0x08, 0x08, 0x7F},
                                  {0x00, 0x41, 0x7F, 0x41, 0x00},
                                  {0x20, 0x40, 0x41, 0x3F, 0x01},
                                  {0x7F, 0x08, 0x14, 0x22, 0x41},
                                  {0x7F, 0x40, 0x40, 0x40, 0x40},
                                  {0x7F, 0x02, 0x0C, 0x02, 0x7F},
                                  {0x7F, 0x04, 0x08, 0x10, 0x7F},
                                  {0x3E, 0x41, 0x41, 0x41, 0x3E},
                                  {0x7F, 0x09, 0x09, 0x09, 0x06},
                                  {0x3E, 0x41, 0x51, 0x21, 0x5E},
                                  {0x7F, 0x09, 0x19, 0x29, 0x46},
                                  {0x46, 0x49, 0x49, 0x49, 0x31},
                                  {0x01, 0x01, 0x7F, 0x01, 0x01},
                                  {0x3F, 0x40, 0x40, 0x40, 0x3F},
                                  {0x1F, 0x20, 0x40, 0x20, 0x1F},
                                  {0x3F, 0x40, 0x38, 0x40, 0x3F},
                                  {0x63, 0x14, 0x08, 0x14, 0x63},
                                  {0x07, 0x08, 0x70, 0x08, 0x07},
                                  {0x61, 0x51, 0x49, 0x45, 0x43}};

    // ===== 测试场景1: 经典帧率测试 =====
    printf("\n===== 测试场景1: 经典帧率测试 =====\n");

    // 重置帧率统计
    frame_count = 0;
    last_time = HAL_GetTick();
    min_fps = 999.0f;
    max_fps = 0.0f;
    avg_fps = 0.0f;

    // 测试开始时间
    uint32_t start_time = HAL_GetTick();
    uint32_t end_time = start_time + (test_duration * 1000);

    // 测试循环
    while (HAL_GetTick() < end_time) {
        // 清空缓冲区
        clear_buffers();

        // 绘制移动的圆
        int circle_x = (HAL_GetTick() / 20) % VFD_DISPLAY_WIDTH;
        draw_circle(circle_x, VFD_DISPLAY_HIGHT / 2, 5);

        // 绘制移动的线
        int line_y = (HAL_GetTick() / 30) % VFD_DISPLAY_HIGHT;
        draw_line(0, line_y, VFD_DISPLAY_WIDTH - 1, line_y);

        // 绘制帧率文本
        draw_text(fps_text, 5, 2, font);

        // 发送到屏幕
        send_buffer();

        // 更新帧率
        update_fps(&frame_count, &last_time, &fps, &min_fps, &max_fps, &avg_fps,
                   fps_text);
    }

    // 显示测试结果
    printf("经典帧率测试结果: 最小: %.1f FPS, 最大: %.1f FPS, 平均: %.1f FPS\n",
           min_fps, max_fps, avg_fps);

    // 显示测试完成
    clear_buffers();
    sprintf(fps_text, "Test 1: %.1f FPS", avg_fps);
    draw_text(fps_text, 5, 5, font);
    send_buffer();
    HAL_Delay(2000);

    // ===== 测试场景2: 大图像刷新测试 =====
    printf("\n===== 测试场景2: 大图像刷新测试 =====\n");

    // 重置帧率统计
    frame_count = 0;
    last_time = HAL_GetTick();
    min_fps = 999.0f;
    max_fps = 0.0f;
    avg_fps = 0.0f;

    // 测试开始时间
    start_time = HAL_GetTick();
    end_time = start_time + (test_duration * 1000);

    // 创建一个大图像模式 (多个重叠的几何图形)
    while (HAL_GetTick() < end_time) {
        // 清空缓冲区
        clear_buffers();

        // 绘制多个圆形
        for (int i = 0; i < 5; i++) {
            int radius = 10 + i * 5;
            int offset = (HAL_GetTick() / 50) % 20;
            draw_circle(VFD_DISPLAY_WIDTH / 2, VFD_DISPLAY_HIGHT / 2,
                        radius + offset);
        }

        // 绘制多个矩形
        for (int i = 0; i < 4; i++) {
            int size = 20 + i * 10;
            int offset = (HAL_GetTick() / 70) % 15;
            draw_rectangle(VFD_DISPLAY_WIDTH / 2 - size / 2 + offset,
                           VFD_DISPLAY_HIGHT / 2 - size / 2 + offset,
                           VFD_DISPLAY_WIDTH / 2 + size / 2 - offset,
                           VFD_DISPLAY_HIGHT / 2 + size / 2 - offset);
        }
        // 绘制对角线
        draw_line(0, 0, VFD_DISPLAY_WIDTH - 1, VFD_DISPLAY_HIGHT - 1);
        draw_line(0, VFD_DISPLAY_HIGHT - 1, VFD_DISPLAY_WIDTH - 1, 0);

        // 绘制帧率文本
        draw_text(fps_text, 5, 2, font);

        // 发送到屏幕
        send_buffer();

        // 更新帧率
        update_fps(&frame_count, &last_time, &fps, &min_fps, &max_fps, &avg_fps,
                   fps_text);
    }

    // 显示测试结果
    printf(
        "大图像刷新测试结果: 最小: %.1f FPS, 最大: %.1f FPS, 平均: %.1f FPS\n",
        min_fps, max_fps, avg_fps);

    // 显示测试完成
    clear_buffers();
    sprintf(fps_text, "Test 2: %.1f FPS", avg_fps);
    draw_text(fps_text, 5, 5, font);
    send_buffer();
    HAL_Delay(2000);

    // ===== 测试场景3: 暴力刷屏性能测试 =====
    printf("\n===== 测试场景3: 暴力刷屏性能测试 =====\n");

    // 重置帧率统计
    frame_count = 0;
    last_time = HAL_GetTick();
    min_fps = 999.0f;
    max_fps = 0.0f;
    avg_fps = 0.0f;

    // 测试开始时间
    start_time = HAL_GetTick();
    end_time = start_time + (test_duration * 1000);

    // 暴力刷屏测试 - 每帧随机像素
    while (HAL_GetTick() < end_time) {
        // 清空缓冲区
        clear_buffers();

        // 随机绘制大量像素点
        for (int i = 0; i < 500; i++) {
            int x = rand() % VFD_DISPLAY_WIDTH;
            int y = rand() % VFD_DISPLAY_HIGHT;
            draw_pixel(x, y);
        }

        // 绘制帧率文本
        draw_text(fps_text, 5, 2, font);

        // 发送到屏幕
        send_buffer();

        // 更新帧率
        update_fps(&frame_count, &last_time, &fps, &min_fps, &max_fps, &avg_fps,
                   fps_text);
    }

    // 显示测试结果
    printf(
        "暴力刷屏性能测试结果: 最小: %.1f FPS, 最大: %.1f FPS, 平均: %.1f "
        "FPS\n",
        min_fps, max_fps, avg_fps);

    // 显示测试完成
    clear_buffers();
    sprintf(fps_text, "Test 3: %.1f FPS", avg_fps);
    draw_text(fps_text, 5, 5, font);
    send_buffer();
    HAL_Delay(2000);

    // ===== 测试场景4: 全屏刷新测试 =====
    printf("\n===== 测试场景4: 全屏刷新测试 =====\n");

    // 重置帧率统计
    frame_count = 0;
    last_time = HAL_GetTick();
    min_fps = 999.0f;
    max_fps = 0.0f;
    avg_fps = 0.0f;

    // 测试开始时间
    start_time = HAL_GetTick();
    end_time = start_time + (test_duration * 1000);

    // 全屏刷新测试 - 交替黑白屏
    int fill_screen = 0;
    while (HAL_GetTick() < end_time) {
        // 清空缓冲区
        clear_buffers();

        // 全屏填充或清空
        if (fill_screen) {
            for (int y = 0; y < VFD_DISPLAY_HIGHT; y++) {
                for (int x = 0; x < VFD_DISPLAY_WIDTH; x++) {
                    draw_pixel(x, y);
                }
            }
        }

        // 绘制帧率文本 (在黑屏时才显示)
        if (!fill_screen) {
            draw_text(fps_text, 5, 2, font);
        }

        // 发送到屏幕
        send_buffer();

        // 切换填充状态
        fill_screen = !fill_screen;

        // 更新帧率
        update_fps(&frame_count, &last_time, &fps, &min_fps, &max_fps, &avg_fps,
                   fps_text);
    }

    // 显示测试结果
    printf("全屏刷新测试结果: 最小: %.1f FPS, 最大: %.1f FPS, 平均: %.1f FPS\n",
           min_fps, max_fps, avg_fps);

    // 显示测试完成
    clear_buffers();
    sprintf(fps_text, "Test 4: %.1f FPS", avg_fps);
    draw_text(fps_text, 5, 5, font);
    send_buffer();
    HAL_Delay(2000);

    // ===== 测试场景5: 复杂图形动画测试 =====
    printf("\n===== 测试场景5: 复杂图形动画测试 =====\n");

    // 重置帧率统计
    frame_count = 0;
    last_time = HAL_GetTick();
    min_fps = 999.0f;
    max_fps = 0.0f;
    avg_fps = 0.0f;

    // 测试开始时间
    start_time = HAL_GetTick();
    end_time = start_time + (test_duration * 1000);

    // 复杂图形动画测试
    while (HAL_GetTick() < end_time) {
        // 清空缓冲区
        clear_buffers();

        // 获取当前时间用于动画
        uint32_t t = HAL_GetTick();

        // 绘制旋转的线条
        for (int i = 0; i < 12; i++) {
            float angle = (i * 30 + t / 20) % 360;
            float rad = angle * 3.14159f / 180.0f;
            int x1 = VFD_DISPLAY_WIDTH / 2;
            int y1 = VFD_DISPLAY_HIGHT / 2;
            int x2 = x1 + (int)(cos(rad) * 40);
            int y2 = y1 + (int)(sin(rad) * 40);
            draw_line(x1, y1, x2, y2);
        }

        // 绘制移动的小圆点
        for (int i = 0; i < 5; i++) {
            float angle = (i * 72 + t / 10) % 360;
            float rad = angle * 3.14159f / 180.0f;
            int x = VFD_DISPLAY_WIDTH / 2 + (int)(cos(rad) * 20);
            int y = VFD_DISPLAY_HIGHT / 2 + (int)(sin(rad) * 20);
            draw_circle(x, y, 2);
        }

        // 绘制脉动的矩形
        int size = 10 + (int)(sin(t / 200.0f) * 5);
        draw_rectangle(
            VFD_DISPLAY_WIDTH / 2 - size, VFD_DISPLAY_HIGHT / 2 - size,
            VFD_DISPLAY_WIDTH / 2 + size, VFD_DISPLAY_HIGHT / 2 + size);

        // 绘制帧率文本
        draw_text(fps_text, 5, 2, font);

        // 发送到屏幕
        send_buffer();

        // 更新帧率
        update_fps(&frame_count, &last_time, &fps, &min_fps, &max_fps, &avg_fps,
                   fps_text);
    }

    // 显示测试结果
    printf(
        "复杂图形动画测试结果: 最小: %.1f FPS, 最大: %.1f FPS, 平均: %.1f "
        "FPS\n",
        min_fps, max_fps, avg_fps);

    // 显示测试完成
    clear_buffers();
    sprintf(fps_text, "Test 5: %.1f FPS", avg_fps);
    draw_text(fps_text, 5, 5, font);
    send_buffer();
    HAL_Delay(2000);

    // 显示所有测试的总结
    clear_buffers();
    draw_text("Test Complete", 5, 2, font);
    send_buffer();
    HAL_Delay(3000);

    printf("帧率测试完成\n");
}