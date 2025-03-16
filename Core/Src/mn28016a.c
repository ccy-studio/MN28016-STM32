#include "mn28016a.h"
#include "spi.h"
#include "sys.h"

// 定义VFD屏幕参数
#define GRID_BYTE_8 12   // 栅极字节
#define SEG_BYTE_8 12    // 段字节
#define TOTAL_BYTE_8 24  // 移位寄存器总字节
#define GRID_END_BIT 96  // 栅极结束位

#define VFD_LOGIC_WIDTH 282  // 屏幕逻辑像素长度 X
#define VFD_LOGIC_HIGHT 16   // 屏幕逻辑像素宽度 Y

// 定义栅极扫描参数
#define TOTAL_FRAMES 94  // 总帧数(T1-T94)

#define SEG_A_START 0  // a段起始位
#define SEG_B_START 2  // b段起始位
#define SEG_C_START 4  // c段起始位
#define SEG_D_START 5  // d段起始位
#define SEG_E_START 3  // e段起始位
#define SEG_F_START 1  // f段起始位

// 定义像素缓冲区位图
static uint8_t pixel_buffer[VFD_LOGIC_HIGHT][(VFD_LOGIC_WIDTH + 7) / 8];

// 定义DMA发送缓冲区 配置SPI发送为MSB
// 栅极数据放在后12位,行段数据放在前12位,共计24位
// 屏幕的时序发送从DO192开始发送，最后结尾发送DO1
static uint8_t dma_buffer[TOTAL_FRAMES][TOTAL_BYTE_8];

// 当前活动的栅极对索引 MAX = TOTAL_FRAMES - 1
static volatile uint8_t current_frame = 0;

// 当前亮度等级
static volatile uint8_t brightness;

// 初始化栅极数据模板-随意修改可导致屏幕损坏
static void init_grid_data() {
    uint8_t grid_buffer[TOTAL_FRAMES][GRID_BYTE_8];
    // 清空Gird模板数据
    memset(grid_buffer, 0, sizeof(grid_buffer));

    // 根据数据表的栅极扫描时序设置栅极数据
    // T1 = G1+G2, T2 = G2+G3, ..., T94 = G94+G95
    for (int frame = 0; frame < TOTAL_FRAMES; frame++) {
        // 计算当前激活的栅极对需要的位移
        // 当frame = 0 part1 = G1 pare2 = G2
        // 当 frame = 1 part1 = G2 part2  =G3
        // 当frame = 93 part1 = G94 part2 = G95
        // 计算位置 栅极结束位 96 开始位置 2
        // 对应G1 = 96、G2=95  G95 = 2
        uint8_t grid1, grid2;

        grid1 = frame + 1;
        grid2 = frame + 2;

        // 设置栅极位 - 注意G95对应Bit 2，G1对应Bit 96
        // 一个grid_buffer[frame]是96个Bit展开96个比特平铺后从最左边认为是第96位，最右边是第1位，依据规格书的DO1~DO96
        // 整除8为了找出在那个字节下标，%8为了找出在那个Bit位
        // 计算字节位置和位偏移
        uint8_t byte_pos1 = (GRID_END_BIT - grid1) / 8;
        uint8_t bit_offset1 = (GRID_END_BIT - grid1) % 8;

        uint8_t byte_pos2 = (GRID_END_BIT - grid2) / 8;
        uint8_t bit_offset2 = (GRID_END_BIT - grid2) % 8;

        // G1 95/8=11
        // G95
        uint8_t base_idx = GRID_BYTE_8 - 1;
        grid_buffer[frame][base_idx - byte_pos1] |= (0x80 >> (7 - bit_offset1));
        grid_buffer[frame][base_idx - byte_pos2] |= (0x80 >> (7 - bit_offset2));
    }

    // 准备栅极数据到DMA预填充
    //  SPI配置位MSB,存储顺序则左边位0Bit，右边是191Bit
    //  前96个bit填充的是栅极数据
    memset(dma_buffer, 0x00, sizeof(dma_buffer));
    for (size_t i = 0; i < TOTAL_FRAMES; i++) {
        memcpy(&dma_buffer[i][0] + SEG_BYTE_8, grid_buffer[i], GRID_BYTE_8);
    }
}

// DMA传输完成回调
void HAL_SPI_TxCpltCallback(SPI_HandleTypeDef* hspi) {
    if (hspi->Instance == SPI1) {
        // 在帧结束时拉高BLK信号实现消隐
        HAL_GPIO_WritePin(BLK_GPIO_Port, BLK_Pin, GPIO_PIN_SET);
        delay_us(2);
        HAL_GPIO_WritePin(LAT_GPIO_Port, LAT_Pin, GPIO_PIN_SET);
        delay_us(1);
        HAL_GPIO_WritePin(LAT_GPIO_Port, LAT_Pin, GPIO_PIN_RESET);

        // 在这里控制延迟便可以控制亮度，注意延迟太高会导致刷新率严重下降
        uint8_t tim_us = 255 - brightness;
        delay_us(tim_us == 0 ? 1 : tim_us / 50);

        // 在新帧开始前拉低BLK信号
        HAL_GPIO_WritePin(BLK_GPIO_Port, BLK_Pin, GPIO_PIN_RESET);

        current_frame = (current_frame + 1) % TOTAL_FRAMES;
        // 发送下一帧数据
        HAL_SPI_Transmit_DMA(&hspi1, &dma_buffer[current_frame], TOTAL_BYTE_8);
    }
}

/**
 * 亮度调整0~255级
 */
void set_brightness(uint8_t level) {
    brightness = level;
}

void init_vfd() {
    // 为每个栅极对(T1-T94)生成数据
    init_grid_data();

    // 初始设置BLK信号为高电平
    HAL_GPIO_WritePin(BLK_GPIO_Port, BLK_Pin, GPIO_PIN_SET);

    // 配置SPI和DMA
    // 每次只发送一帧数据，在回调函数中手动启动下一帧
    current_frame = 0;
    brightness = 255;

    clear_buffers();

    // 使用非阻塞方式启动DMA传输
    HAL_StatusTypeDef status =
        HAL_SPI_Transmit_DMA(&hspi1, &dma_buffer[current_frame], TOTAL_BYTE_8);
    if (status != HAL_OK) {
        printf("DMAR Run Fail: %d\n", status);
    } else {
        printf("DMA Run Success\n");
    }
}

/**
 * 清空显示缓冲区
 */
void clear_buffers() {
    // 清空像素缓冲区
    memset(pixel_buffer, 0, sizeof(pixel_buffer));
}

/**
 * 设置Bit位
 */
static void set_bit(uint8_t* buf, uint16_t bit_pos) {
    buf[(bit_pos / 8)] |= (1 << (7 - (bit_pos % 8)));
}

/**
 * 获取Bit位
 */
static uint8_t get_bit(const uint8_t* buf, uint16_t bit_pos) {
    return buf[(bit_pos / 8)] & (1 << (7 - (bit_pos % 8)));
}

/**
 * 发送缓冲区数据到屏幕
 */
void send_buffer() {
    /**
     * 根据像素缓冲区更新段数据
     * Note1:
     *      所选网格中设置 1a~16a、1b~16b、1c~16c 的点数据。
     *      此时1d~16d、1e~16e、1f~16f 的点数据关闭。
     *      周期是奇数帧
     * Note2:
     *      在所选网格中设置 1d~16d、1e~16e、1f~16f的点数据。
     *      此时1a~16a、1b~16b、1c~16c 的点数据关闭。
     *      T周期是偶数帧
     *
     *  在这里循环去修改DMA的数据，无需担心DMA读取新旧叠加的错误数据，因为SPI刷新很快可以忽略肉眼观察范围
     *  DMA缓冲区的前96位存储的是行段数据
     *      1. 先根据X轴坐标找出所在的帧的下标
     *      2. 根据Y坐标的像素点状态，在依据Note1.2协议选择性激活对应的段
     */
    for (size_t i = 0; i < TOTAL_FRAMES; i++) {
        bool even = (i + 1) % 2 == 0;  // 判断是否是偶数帧
        uint8_t seg_buf[SEG_BYTE_8];   // 建立行段临时缓冲区
        memset(seg_buf, 0x00, sizeof(seg_buf));

        // 遍历所有行
        for (size_t y = 0; y < VFD_LOGIC_HIGHT; y++) {
            // 计算当前帧在当前行中对应的像素起始位置
            // 每个帧对应3个像素点
            uint16_t start_x = i * 3;
            // pixel_buffer[y];//row? 判断X在那个像素点的映射
            for (uint8_t offset = 0; offset < 3; offset++) {
                uint16_t x = start_x + offset;
                // 检查x是否超出显示范围
                if (x >= VFD_LOGIC_WIDTH) {
                    continue;
                }
                // 检查这个像素是否需要点亮
                if (get_bit(pixel_buffer[y], x)) {
                    // 获取映射Base

                    uint16_t address_base;

                    if (even) {
                        // 偶数帧 点d、e、f (Note 2)
                        if (offset == 0) {
                            // SEG_D_START + y * 6;
                            address_base = SEG_D_START;
                        } else if (offset == 1) {
                            address_base = SEG_E_START;
                        } else {
                            address_base = SEG_F_START;
                        }
                    } else {
                        // 奇数帧 点a、b、c(Note 1)
                        if (offset == 0) {
                            address_base = SEG_A_START;
                        } else if (offset == 1) {
                            address_base = SEG_B_START;
                        } else {
                            address_base = SEG_C_START;
                        }
                    }
                    uint16_t seg_pos = (address_base + y * 6);
                    uint16_t byte_pos = seg_pos / 8;
                    uint8_t bit_offset = seg_pos % 8;
                    seg_buf[byte_pos] |= (1 << (7 - bit_offset));
                }
            }
        }
        // 拷贝段数据到DMA缓冲区
        memcpy(&dma_buffer[i][0], seg_buf, SEG_BYTE_8);
    }
}

/**
 * 画一个点,起点从0,0开始
 */
void draw_pixel(uint16_t x, uint16_t y) {
    if (x >= VFD_DISPLAY_WIDTH || y >= VFD_DISPLAY_HIGHT)
        return;
    x++;
    set_bit(pixel_buffer[y], x);
}

void test_vfd() {
    printf("开始VFD全亮测试\n");
    // 清空缓冲区
    clear_buffers();

    // 设置所有像素点
    for (int y = 0; y < VFD_LOGIC_HIGHT; y++) {
        for (int x = 0; x < VFD_LOGIC_WIDTH; x++) {
            draw_pixel(x, y);
        }
    }

    // 发送到屏幕
    send_buffer();
    printf("VFD全亮测试完成\n");
}