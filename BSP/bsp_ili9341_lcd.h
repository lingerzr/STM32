#ifndef __BSP_ILI9341_LCD_H
#define __BSP_ILI9341_LCD_H

#include "stm32f4xx_hal.h"

/*
 * 硬件连接 (根据 STM32F407ZGT6 板级原理图):
 *   FSMC Bank4 (NE4=PG12)  → LCD CS    (CubeMX MSP 初始化)
 *   FSMC A6    (PF12)       → LCD RS/DC (CubeMX MSP 初始化)
 *   FSMC NWE   (PD5)        → LCD WR    (CubeMX MSP 初始化)
 *   FSMC NOE   (PD4)        → LCD RD    (CubeMX MSP 初始化)
 *   D0-D15: PD/PE数据线    (CubeMX MSP 初始化)
 *   PG11 (GPIO OUT)         → LCD RST   (驱动内初始化)
 *   PG6  (GPIO OUT, 低有效) → LCD BL    (PNP Q2驱动, 低电平点亮, 驱动内初始化)
 *
 * 内存映射地址 (NE4 + A6):
 *   NE4基址: 0x6C000000
 *   A6在16位模式下: FSMC_A6 = HADDR[7] = bit7 = 0x00000080
 *   CMD地址(RS=0): 0x6C000000
 *   DATA地址(RS=1): 0x6C000080
 */

/* FSMC Bank4 */
#define ILI9341_FSMC_BANK           FSMC_NORSRAM_BANK4

/* LCD内存映射寄存器 */
#define ILI9341_CMD_ADDR            ((uint32_t)0x6C000000)
#define ILI9341_DATA_ADDR           ((uint32_t)0x6C000080)
#define LCD_CMD                     (*(__IO uint16_t *)(ILI9341_CMD_ADDR))
#define LCD_DATA                    (*(__IO uint16_t *)(ILI9341_DATA_ADDR))

/* 复位引脚 (PG11) */
#define ILI9341_RST_GPIO_PORT       GPIOG
#define ILI9341_RST_GPIO_PIN        GPIO_PIN_11

/* 背光引脚 (PG6, PNP三极管, 低电平点亮) */
#define ILI9341_BK_GPIO_PORT        GPIOG
#define ILI9341_BK_GPIO_PIN         GPIO_PIN_6

/* 控制宏 */
#define ILI9341_RST_LOW()           HAL_GPIO_WritePin(ILI9341_RST_GPIO_PORT, ILI9341_RST_GPIO_PIN, GPIO_PIN_RESET)
#define ILI9341_RST_HIGH()          HAL_GPIO_WritePin(ILI9341_RST_GPIO_PORT, ILI9341_RST_GPIO_PIN, GPIO_PIN_SET)
/* 背光低电平点亮: BK_ON=LOW, BK_OFF=HIGH */
#define ILI9341_BK_ON()             HAL_GPIO_WritePin(ILI9341_BK_GPIO_PORT,  ILI9341_BK_GPIO_PIN,  GPIO_PIN_RESET)
#define ILI9341_BK_OFF()            HAL_GPIO_WritePin(ILI9341_BK_GPIO_PORT,  ILI9341_BK_GPIO_PIN,  GPIO_PIN_SET)

/* 屏幕分辨率 */
#define ILI9341_WIDTH               240
#define ILI9341_HEIGHT              320

/* RGB565 颜色定义 */
#define COLOR_WHITE                 0xFFFF
#define COLOR_BLACK                 0x0000
#define COLOR_RED                   0xF800
#define COLOR_GREEN                 0x07E0
#define COLOR_BLUE                  0x001F
#define COLOR_YELLOW                0xFFE0
#define COLOR_CYAN                  0x07FF
#define COLOR_MAGENTA               0xF81F
#define COLOR_ORANGE                0xFD20
#define COLOR_GRAY                  0x8410
#define COLOR_DARKGRAY              0x4208
#define COLOR_LIGHTGRAY             0xC618

/* ILI9341 命令集 */
#define ILI9341_NOP                 0x00
#define ILI9341_SWRESET             0x01
#define ILI9341_SLEEP_OUT           0x11
#define ILI9341_NORMAL_MODE         0x13
#define ILI9341_INV_OFF             0x20
#define ILI9341_INV_ON              0x21
#define ILI9341_GAMMA_SET           0x26
#define ILI9341_DISPLAY_OFF         0x28
#define ILI9341_DISPLAY_ON          0x29
#define ILI9341_COL_ADDR_SET        0x2A
#define ILI9341_PAGE_ADDR_SET       0x2B
#define ILI9341_GRAM_WR             0x2C
#define ILI9341_GRAM_RD             0x2E
#define ILI9341_MEM_ACCESS_CTRL     0x36
#define ILI9341_PIXEL_FORMAT        0x3A
#define ILI9341_FRAME_CTRL          0xB1
#define ILI9341_DISPLAY_FUNC        0xB6
#define ILI9341_POWER_CTRL1         0xC0
#define ILI9341_POWER_CTRL2         0xC1
#define ILI9341_VCOM_CTRL1          0xC5
#define ILI9341_VCOM_CTRL2          0xC7
#define ILI9341_POWER_CTRL_A        0xCB
#define ILI9341_POWER_CTRL_B        0xCF
#define ILI9341_TIMING_CTRL_A       0xE8
#define ILI9341_TIMING_CTRL_B       0xEA
#define ILI9341_POWER_ON_SEQ        0xED
#define ILI9341_PUMP_RATIO          0xF7
#define ILI9341_POS_GAMMA           0xE0
#define ILI9341_NEG_GAMMA           0xE1
#define ILI9341_ENABLE_3G           0xF2

/* 显示方向 */
#define ILI9341_DIR_0               0   /* 竖屏, 原点左上, 0° */
#define ILI9341_DIR_90              1   /* 横屏, 原点左上, 90° */
#define ILI9341_DIR_180             2   /* 竖屏, 原点右下, 180° */
#define ILI9341_DIR_270             3   /* 横屏, 原点右下, 270° */

/* 字符大小倍数 */
#define FONT_SIZE_SMALL             1
#define FONT_SIZE_MEDIUM            2
#define FONT_SIZE_LARGE             3

/* 函数声明 */
void     ILI9341_Init(void);
void     ILI9341_WriteCmd(uint16_t cmd);
void     ILI9341_WriteData(uint16_t data);
void     ILI9341_SetRegion(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2);
void     ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color);
void     ILI9341_FillRegion(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void     ILI9341_FillScreen(uint16_t color);
void     ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color);
void     ILI9341_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color);
void     ILI9341_DrawCircle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color);
void     ILI9341_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t fg, uint16_t bg, uint8_t size);
void     ILI9341_DrawString(uint16_t x, uint16_t y, const char *str, uint16_t fg, uint16_t bg, uint8_t size);
void     ILI9341_SetDirection(uint8_t dir);
void     ILI9341_BacklightOn(void);
void     ILI9341_BacklightOff(void);
uint16_t ILI9341_RGB(uint8_t r, uint8_t g, uint8_t b);

#endif /* __BSP_ILI9341_LCD_H */
