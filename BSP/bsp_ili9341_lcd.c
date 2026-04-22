#include "bsp_ili9341_lcd.h"

/* ------------------------------------------------------------------ */
/* 8x8 ASCII字体 (0x20 ~ 0x7E), 每字符8行×8列, MSB为左侧像素        */
/* ------------------------------------------------------------------ */
static const uint8_t font8x8[][8] = {
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0x00}, /* 0x20 ' ' */
    {0x18,0x3C,0x3C,0x18,0x18,0x00,0x18,0x00}, /* 0x21 '!' */
    {0x6C,0x6C,0x24,0x00,0x00,0x00,0x00,0x00}, /* 0x22 '"' */
    {0x6C,0x6C,0xFE,0x6C,0xFE,0x6C,0x6C,0x00}, /* 0x23 '#' */
    {0x18,0x7C,0xC0,0x78,0x0C,0xF8,0x18,0x00}, /* 0x24 '$' */
    {0x00,0xC6,0xCC,0x18,0x30,0x66,0xC6,0x00}, /* 0x25 '%' */
    {0x38,0x6C,0x38,0x76,0xDC,0xCC,0x76,0x00}, /* 0x26 '&' */
    {0x30,0x30,0x60,0x00,0x00,0x00,0x00,0x00}, /* 0x27 '\'' */
    {0x18,0x30,0x60,0x60,0x60,0x30,0x18,0x00}, /* 0x28 '(' */
    {0x60,0x30,0x18,0x18,0x18,0x30,0x60,0x00}, /* 0x29 ')' */
    {0x00,0x66,0x3C,0xFF,0x3C,0x66,0x00,0x00}, /* 0x2A '*' */
    {0x00,0x18,0x18,0x7E,0x18,0x18,0x00,0x00}, /* 0x2B '+' */
    {0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x60}, /* 0x2C ',' */
    {0x00,0x00,0x00,0x7E,0x00,0x00,0x00,0x00}, /* 0x2D '-' */
    {0x00,0x00,0x00,0x00,0x00,0x30,0x30,0x00}, /* 0x2E '.' */
    {0x06,0x0C,0x18,0x30,0x60,0xC0,0x80,0x00}, /* 0x2F '/' */
    {0x78,0xCC,0xCE,0xDE,0xF6,0xE6,0x7C,0x00}, /* 0x30 '0' */
    {0x30,0x70,0x30,0x30,0x30,0x30,0xFC,0x00}, /* 0x31 '1' */
    {0x78,0xCC,0x0C,0x38,0x60,0xCC,0xFC,0x00}, /* 0x32 '2' */
    {0x78,0xCC,0x0C,0x38,0x0C,0xCC,0x78,0x00}, /* 0x33 '3' */
    {0x1C,0x3C,0x6C,0xCC,0xFE,0x0C,0x1E,0x00}, /* 0x34 '4' */
    {0xFC,0xC0,0xF8,0x0C,0x0C,0xCC,0x78,0x00}, /* 0x35 '5' */
    {0x38,0x60,0xC0,0xF8,0xCC,0xCC,0x78,0x00}, /* 0x36 '6' */
    {0xFC,0xCC,0x0C,0x18,0x30,0x30,0x30,0x00}, /* 0x37 '7' */
    {0x78,0xCC,0xCC,0x78,0xCC,0xCC,0x78,0x00}, /* 0x38 '8' */
    {0x78,0xCC,0xCC,0x7C,0x0C,0x18,0x70,0x00}, /* 0x39 '9' */
    {0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x00}, /* 0x3A ':' */
    {0x00,0x30,0x30,0x00,0x00,0x30,0x30,0x60}, /* 0x3B ';' */
    {0x18,0x30,0x60,0xC0,0x60,0x30,0x18,0x00}, /* 0x3C '<' */
    {0x00,0x00,0x7E,0x00,0x00,0x7E,0x00,0x00}, /* 0x3D '=' */
    {0x60,0x30,0x18,0x0C,0x18,0x30,0x60,0x00}, /* 0x3E '>' */
    {0x78,0xCC,0x0C,0x18,0x30,0x00,0x30,0x00}, /* 0x3F '?' */
    {0x7C,0xC6,0xDE,0xDE,0xDE,0xC0,0x7C,0x00}, /* 0x40 '@' */
    {0x30,0x78,0xCC,0xCC,0xFC,0xCC,0xCC,0x00}, /* 0x41 'A' */
    {0xFC,0x66,0x66,0x7C,0x66,0x66,0xFC,0x00}, /* 0x42 'B' */
    {0x3C,0x66,0xC0,0xC0,0xC0,0x66,0x3C,0x00}, /* 0x43 'C' */
    {0xF8,0x6C,0x66,0x66,0x66,0x6C,0xF8,0x00}, /* 0x44 'D' */
    {0xFE,0x62,0x68,0x78,0x68,0x62,0xFE,0x00}, /* 0x45 'E' */
    {0xFE,0x62,0x68,0x78,0x68,0x60,0xF0,0x00}, /* 0x46 'F' */
    {0x3C,0x66,0xC0,0xC0,0xCE,0x66,0x3E,0x00}, /* 0x47 'G' */
    {0xCC,0xCC,0xCC,0xFC,0xCC,0xCC,0xCC,0x00}, /* 0x48 'H' */
    {0x78,0x30,0x30,0x30,0x30,0x30,0x78,0x00}, /* 0x49 'I' */
    {0x1E,0x0C,0x0C,0x0C,0xCC,0xCC,0x78,0x00}, /* 0x4A 'J' */
    {0xE6,0x66,0x6C,0x78,0x6C,0x66,0xE6,0x00}, /* 0x4B 'K' */
    {0xF0,0x60,0x60,0x60,0x62,0x66,0xFE,0x00}, /* 0x4C 'L' */
    {0xC6,0xEE,0xFE,0xFE,0xD6,0xC6,0xC6,0x00}, /* 0x4D 'M' */
    {0xC6,0xE6,0xF6,0xDE,0xCE,0xC6,0xC6,0x00}, /* 0x4E 'N' */
    {0x38,0x6C,0xC6,0xC6,0xC6,0x6C,0x38,0x00}, /* 0x4F 'O' */
    {0xFC,0x66,0x66,0x7C,0x60,0x60,0xF0,0x00}, /* 0x50 'P' */
    {0x78,0xCC,0xCC,0xCC,0xDC,0x78,0x1C,0x00}, /* 0x51 'Q' */
    {0xFC,0x66,0x66,0x7C,0x6C,0x66,0xE6,0x00}, /* 0x52 'R' */
    {0x78,0xCC,0xE0,0x70,0x1C,0xCC,0x78,0x00}, /* 0x53 'S' */
    {0xFC,0xB4,0x30,0x30,0x30,0x30,0x78,0x00}, /* 0x54 'T' */
    {0xCC,0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x00}, /* 0x55 'U' */
    {0xCC,0xCC,0xCC,0xCC,0xCC,0x78,0x30,0x00}, /* 0x56 'V' */
    {0xC6,0xC6,0xC6,0xD6,0xFE,0xEE,0xC6,0x00}, /* 0x57 'W' */
    {0xC6,0xC6,0x6C,0x38,0x38,0x6C,0xC6,0x00}, /* 0x58 'X' */
    {0xCC,0xCC,0xCC,0x78,0x30,0x30,0x78,0x00}, /* 0x59 'Y' */
    {0xFE,0xC6,0x86,0x0C,0x18,0x32,0xFE,0x00}, /* 0x5A 'Z' */
    {0x78,0x60,0x60,0x60,0x60,0x60,0x78,0x00}, /* 0x5B '[' */
    {0xC0,0x60,0x30,0x18,0x0C,0x06,0x02,0x00}, /* 0x5C '\' */
    {0x78,0x18,0x18,0x18,0x18,0x18,0x78,0x00}, /* 0x5D ']' */
    {0x10,0x38,0x6C,0xC6,0x00,0x00,0x00,0x00}, /* 0x5E '^' */
    {0x00,0x00,0x00,0x00,0x00,0x00,0x00,0xFF}, /* 0x5F '_' */
    {0x30,0x30,0x18,0x00,0x00,0x00,0x00,0x00}, /* 0x60 '`' */
    {0x00,0x00,0x78,0x0C,0x7C,0xCC,0x76,0x00}, /* 0x61 'a' */
    {0xE0,0x60,0x7C,0x66,0x66,0x66,0xDC,0x00}, /* 0x62 'b' */
    {0x00,0x00,0x78,0xCC,0xC0,0xCC,0x78,0x00}, /* 0x63 'c' */
    {0x1C,0x0C,0x7C,0xCC,0xCC,0xCC,0x76,0x00}, /* 0x64 'd' */
    {0x00,0x00,0x78,0xCC,0xFC,0xC0,0x78,0x00}, /* 0x65 'e' */
    {0x38,0x6C,0x64,0xF0,0x60,0x60,0xF0,0x00}, /* 0x66 'f' */
    {0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C,0xF8}, /* 0x67 'g' */
    {0xE0,0x60,0x6C,0x76,0x66,0x66,0xE6,0x00}, /* 0x68 'h' */
    {0x30,0x00,0x70,0x30,0x30,0x30,0xFC,0x00}, /* 0x69 'i' */
    {0x0C,0x00,0x0C,0x0C,0x0C,0xCC,0xCC,0x78}, /* 0x6A 'j' */
    {0xE0,0x60,0x66,0x6C,0x78,0x6C,0xE6,0x00}, /* 0x6B 'k' */
    {0x70,0x30,0x30,0x30,0x30,0x30,0xFC,0x00}, /* 0x6C 'l' */
    {0x00,0x00,0xCC,0xFE,0xFE,0xD6,0xC6,0x00}, /* 0x6D 'm' */
    {0x00,0x00,0xD8,0xCC,0xCC,0xCC,0xCC,0x00}, /* 0x6E 'n' */
    {0x00,0x00,0x78,0xCC,0xCC,0xCC,0x78,0x00}, /* 0x6F 'o' */
    {0x00,0x00,0xDC,0x66,0x66,0x7C,0x60,0xF0}, /* 0x70 'p' */
    {0x00,0x00,0x76,0xCC,0xCC,0x7C,0x0C,0x1E}, /* 0x71 'q' */
    {0x00,0x00,0xDC,0x76,0x66,0x60,0xF0,0x00}, /* 0x72 'r' */
    {0x00,0x00,0x78,0xCC,0x70,0x1C,0xF8,0x00}, /* 0x73 's' */
    {0x10,0x30,0x7C,0x30,0x30,0x34,0x18,0x00}, /* 0x74 't' */
    {0x00,0x00,0xCC,0xCC,0xCC,0xCC,0x76,0x00}, /* 0x75 'u' */
    {0x00,0x00,0xCC,0xCC,0xCC,0x78,0x30,0x00}, /* 0x76 'v' */
    {0x00,0x00,0xC6,0xD6,0xFE,0xFE,0x6C,0x00}, /* 0x77 'w' */
    {0x00,0x00,0xC6,0x6C,0x38,0x6C,0xC6,0x00}, /* 0x78 'x' */
    {0x00,0x00,0xCC,0xCC,0xCC,0x7C,0x0C,0xF8}, /* 0x79 'y' */
    {0x00,0x00,0xFC,0x98,0x30,0x64,0xFC,0x00}, /* 0x7A 'z' */
    {0x1C,0x30,0x30,0xE0,0x30,0x30,0x1C,0x00}, /* 0x7B '{' */
    {0x18,0x18,0x18,0x00,0x18,0x18,0x18,0x00}, /* 0x7C '|' */
    {0xE0,0x30,0x30,0x1C,0x30,0x30,0xE0,0x00}, /* 0x7D '}' */
    {0x76,0xDC,0x00,0x00,0x00,0x00,0x00,0x00}, /* 0x7E '~' */
};

/* ------------------------------------------------------------------ */
/* FSMC句柄 (Bank4, LCD专用)                                          */
/* ------------------------------------------------------------------ */
static SRAM_HandleTypeDef hlcd_sram;

/* ------------------------------------------------------------------ */
/* 内部函数声明                                                        */
/* ------------------------------------------------------------------ */
static void ILI9341_GPIO_Config(void);
static void ILI9341_FSMC_Config(void);
static void ILI9341_Reset(void);
static void ILI9341_RegInit(void);

/* ================================================================== */
/* 基础读写                                                            */
/* ================================================================== */

void ILI9341_WriteCmd(uint16_t cmd)
{
    LCD_CMD = cmd;
}

void ILI9341_WriteData(uint16_t data)
{
    LCD_DATA = data;
}

/* ================================================================== */
/* 初始化                                                              */
/* ================================================================== */

static void ILI9341_GPIO_Config(void)
{
    GPIO_InitTypeDef GPIO_InitStruct = {0};

    /* NE4(PG12) / A23(PE2) / 数据线 / NOE / NWE 已由 CubeMX MSP 统一配置
     * 此处仅初始化 RST(PG11) 和 BK(PG6) 两个普通 GPIO
     * GPIOG 时钟由 MX_GPIO_Init() 已使能，再次调用无害 */
    __HAL_RCC_GPIOG_CLK_ENABLE();

    GPIO_InitStruct.Mode      = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStruct.Pull      = GPIO_NOPULL;
    GPIO_InitStruct.Speed     = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStruct.Alternate = 0;

    /* RST (PG11) */
    GPIO_InitStruct.Pin = ILI9341_RST_GPIO_PIN;
    HAL_GPIO_Init(ILI9341_RST_GPIO_PORT, &GPIO_InitStruct);

    /* BK (PG6), 默认高电平(背光关), 低电平点亮 */
    GPIO_InitStruct.Pin = ILI9341_BK_GPIO_PIN;
    HAL_GPIO_Init(ILI9341_BK_GPIO_PORT, &GPIO_InitStruct);
    ILI9341_BK_OFF();
}

static void ILI9341_FSMC_Config(void)
{
    FSMC_NORSRAM_TimingTypeDef Timing = {0};

    hlcd_sram.Instance = FSMC_NORSRAM_DEVICE;
    hlcd_sram.Extended = FSMC_NORSRAM_EXTENDED_DEVICE;

    hlcd_sram.Init.NSBank             = ILI9341_FSMC_BANK;
    hlcd_sram.Init.DataAddressMux     = FSMC_DATA_ADDRESS_MUX_DISABLE;
    hlcd_sram.Init.MemoryType         = FSMC_MEMORY_TYPE_NOR;
    hlcd_sram.Init.MemoryDataWidth    = FSMC_NORSRAM_MEM_BUS_WIDTH_16;
    hlcd_sram.Init.BurstAccessMode    = FSMC_BURST_ACCESS_MODE_DISABLE;
    hlcd_sram.Init.WaitSignalPolarity = FSMC_WAIT_SIGNAL_POLARITY_LOW;
    hlcd_sram.Init.WrapMode           = FSMC_WRAP_MODE_DISABLE;
    hlcd_sram.Init.WaitSignalActive   = FSMC_WAIT_TIMING_BEFORE_WS;
    hlcd_sram.Init.WriteOperation     = FSMC_WRITE_OPERATION_ENABLE;
    hlcd_sram.Init.WaitSignal         = FSMC_WAIT_SIGNAL_DISABLE;
    hlcd_sram.Init.ExtendedMode       = FSMC_EXTENDED_MODE_DISABLE;
    hlcd_sram.Init.AsynchronousWait   = FSMC_ASYNCHRONOUS_WAIT_DISABLE;
    hlcd_sram.Init.WriteBurst         = FSMC_WRITE_BURST_DISABLE;
    hlcd_sram.Init.PageSize           = FSMC_PAGE_SIZE_NONE;

    /* 模式B异步NOR, 匹配ILI9341 8080时序
     * ADDSET=1, DATAST=4 → 写周期=(1+1+4+1+1)×HCLK
     * @168MHz: ~48ns, 满足ILI9341 tWC≥66ns? → 适当加大DATAST
     * DATAST=9 → (1+1+9+1+1)=13×6ns=78ns ✓ */
    Timing.AddressSetupTime      = 0x01;
    Timing.AddressHoldTime       = 0x00;
    Timing.DataSetupTime         = 0x09;
    Timing.BusTurnAroundDuration = 0x00;
    Timing.CLKDivision           = 0x00;
    Timing.DataLatency           = 0x00;
    Timing.AccessMode            = FSMC_ACCESS_MODE_B;

    HAL_SRAM_Init(&hlcd_sram, &Timing, NULL);
}


static void ILI9341_Reset(void)
{
    ILI9341_RST_HIGH();
    HAL_Delay(5);
    ILI9341_RST_LOW();
    HAL_Delay(20);
    ILI9341_RST_HIGH();
    HAL_Delay(120);
}

static void ILI9341_RegInit(void)
{
    /* Power control B */
    ILI9341_WriteCmd(ILI9341_POWER_CTRL_B);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0xC1);
    ILI9341_WriteData(0x30);

    /* Power on sequence control */
    ILI9341_WriteCmd(ILI9341_POWER_ON_SEQ);
    ILI9341_WriteData(0x64);
    ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x12);
    ILI9341_WriteData(0x81);

    /* Driver timing control A */
    ILI9341_WriteCmd(ILI9341_TIMING_CTRL_A);
    ILI9341_WriteData(0x85);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x79);

    /* Power control A */
    ILI9341_WriteCmd(ILI9341_POWER_CTRL_A);
    ILI9341_WriteData(0x39);
    ILI9341_WriteData(0x2C);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x34);
    ILI9341_WriteData(0x02);

    /* Pump ratio control */
    ILI9341_WriteCmd(ILI9341_PUMP_RATIO);
    ILI9341_WriteData(0x20);

    /* Driver timing control B */
    ILI9341_WriteCmd(ILI9341_TIMING_CTRL_B);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x00);

    /* Power control 1 */
    ILI9341_WriteCmd(ILI9341_POWER_CTRL1);
    ILI9341_WriteData(0x1D); /* VRH=4.60V */

    /* Power control 2 */
    ILI9341_WriteCmd(ILI9341_POWER_CTRL2);
    ILI9341_WriteData(0x12); /* SAP, BT */

    /* VCOM control 1 */
    ILI9341_WriteCmd(ILI9341_VCOM_CTRL1);
    ILI9341_WriteData(0x33);
    ILI9341_WriteData(0x3F);

    /* VCOM control 2 */
    ILI9341_WriteCmd(ILI9341_VCOM_CTRL2);
    ILI9341_WriteData(0x92);

    /* Memory access control: BGR顺序, 竖屏 */
    ILI9341_WriteCmd(ILI9341_MEM_ACCESS_CTRL);
    ILI9341_WriteData(0x08);

    /* Pixel format: 16bpp RGB565 */
    ILI9341_WriteCmd(ILI9341_PIXEL_FORMAT);
    ILI9341_WriteData(0x55);

    /* Frame rate: 70Hz */
    ILI9341_WriteCmd(ILI9341_FRAME_CTRL);
    ILI9341_WriteData(0x00);
    ILI9341_WriteData(0x12);

    /* Display function control */
    ILI9341_WriteCmd(ILI9341_DISPLAY_FUNC);
    ILI9341_WriteData(0x0A);
    ILI9341_WriteData(0xA2);

    /* Enable 3G */
    ILI9341_WriteCmd(ILI9341_ENABLE_3G);
    ILI9341_WriteData(0x00);

    /* Gamma set */
    ILI9341_WriteCmd(ILI9341_GAMMA_SET);
    ILI9341_WriteData(0x01);

    /* Positive gamma correction */
    ILI9341_WriteCmd(ILI9341_POS_GAMMA);
    ILI9341_WriteData(0x0F); ILI9341_WriteData(0x22); ILI9341_WriteData(0x1C);
    ILI9341_WriteData(0x0B); ILI9341_WriteData(0x13); ILI9341_WriteData(0x09);
    ILI9341_WriteData(0x47); ILI9341_WriteData(0x75); ILI9341_WriteData(0x37);
    ILI9341_WriteData(0x06); ILI9341_WriteData(0x10); ILI9341_WriteData(0x03);
    ILI9341_WriteData(0x0E); ILI9341_WriteData(0x09); ILI9341_WriteData(0x00);

    /* Negative gamma correction */
    ILI9341_WriteCmd(ILI9341_NEG_GAMMA);
    ILI9341_WriteData(0x00); ILI9341_WriteData(0x1D); ILI9341_WriteData(0x23);
    ILI9341_WriteData(0x04); ILI9341_WriteData(0x0D); ILI9341_WriteData(0x06);
    ILI9341_WriteData(0x38); ILI9341_WriteData(0x53); ILI9341_WriteData(0x48);
    ILI9341_WriteData(0x09); ILI9341_WriteData(0x0F); ILI9341_WriteData(0x0C);
    ILI9341_WriteData(0x31); ILI9341_WriteData(0x36); ILI9341_WriteData(0x0F);

    /* Sleep out */
    ILI9341_WriteCmd(ILI9341_SLEEP_OUT);
    HAL_Delay(120);

    /* Display on */
    ILI9341_WriteCmd(ILI9341_DISPLAY_ON);
    HAL_Delay(20);
}

void ILI9341_Init(void)
{
    ILI9341_GPIO_Config();
    ILI9341_FSMC_Config();  /* 初始化 FSMC Bank4 (NE4+A23, 模式B) */
    ILI9341_Reset();
    ILI9341_RegInit();
    ILI9341_BK_ON();
    ILI9341_FillScreen(COLOR_BLACK);
}

/* ================================================================== */
/* 显示方向                                                            */
/* ================================================================== */

void ILI9341_SetDirection(uint8_t dir)
{
    ILI9341_WriteCmd(ILI9341_MEM_ACCESS_CTRL);
    switch (dir) {
        case ILI9341_DIR_0:   ILI9341_WriteData(0x08); break; /* 竖屏 */
        case ILI9341_DIR_90:  ILI9341_WriteData(0x68); break; /* 横屏90° */
        case ILI9341_DIR_180: ILI9341_WriteData(0xC8); break; /* 竖屏180° */
        case ILI9341_DIR_270: ILI9341_WriteData(0xA8); break; /* 横屏270° */
        default: ILI9341_WriteData(0x08); break;
    }
}

/* ================================================================== */
/* 区域设置与像素操作                                                  */
/* ================================================================== */

void ILI9341_SetRegion(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2)
{
    ILI9341_WriteCmd(ILI9341_COL_ADDR_SET);
    ILI9341_WriteData(x1 >> 8);
    ILI9341_WriteData(x1 & 0xFF);
    ILI9341_WriteData(x2 >> 8);
    ILI9341_WriteData(x2 & 0xFF);

    ILI9341_WriteCmd(ILI9341_PAGE_ADDR_SET);
    ILI9341_WriteData(y1 >> 8);
    ILI9341_WriteData(y1 & 0xFF);
    ILI9341_WriteData(y2 >> 8);
    ILI9341_WriteData(y2 & 0xFF);

    ILI9341_WriteCmd(ILI9341_GRAM_WR);
}

void ILI9341_DrawPixel(uint16_t x, uint16_t y, uint16_t color)
{
    ILI9341_SetRegion(x, y, x, y);
    LCD_DATA = color;
}

/* ================================================================== */
/* 填充操作                                                            */
/* ================================================================== */

void ILI9341_FillRegion(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    uint32_t pixels = (uint32_t)(x2 - x1 + 1) * (y2 - y1 + 1);
    ILI9341_SetRegion(x1, y1, x2, y2);
    while (pixels--) {
        LCD_DATA = color;
    }
}

void ILI9341_FillScreen(uint16_t color)
{
    ILI9341_FillRegion(0, 0, ILI9341_WIDTH - 1, ILI9341_HEIGHT - 1, color);
}

/* ================================================================== */
/* 基本图形                                                            */
/* ================================================================== */

void ILI9341_DrawLine(uint16_t x1, uint16_t y1, uint16_t x2, uint16_t y2, uint16_t color)
{
    int16_t dx = (x2 > x1) ? (x2 - x1) : (x1 - x2);
    int16_t dy = (y2 > y1) ? (y2 - y1) : (y1 - y2);
    int16_t sx = (x1 < x2) ? 1 : -1;
    int16_t sy = (y1 < y2) ? 1 : -1;
    int16_t err = dx - dy;
    int16_t x = (int16_t)x1;
    int16_t y = (int16_t)y1;

    while (1) {
        ILI9341_DrawPixel((uint16_t)x, (uint16_t)y, color);
        if (x == (int16_t)x2 && y == (int16_t)y2) break;
        int16_t e2 = err * 2;
        if (e2 > -dy) { err -= dy; x += sx; }
        if (e2 <  dx) { err += dx; y += sy; }
    }
}

void ILI9341_DrawRect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, uint16_t color)
{
    ILI9341_DrawLine(x,         y,         x + w - 1, y,         color);
    ILI9341_DrawLine(x,         y + h - 1, x + w - 1, y + h - 1, color);
    ILI9341_DrawLine(x,         y,         x,         y + h - 1, color);
    ILI9341_DrawLine(x + w - 1, y,         x + w - 1, y + h - 1, color);
}

void ILI9341_DrawCircle(uint16_t cx, uint16_t cy, uint16_t r, uint16_t color)
{
    int16_t x = 0;
    int16_t y = (int16_t)r;
    int16_t d = 3 - 2 * (int16_t)r;

    while (x <= y) {
        ILI9341_DrawPixel(cx + (uint16_t)x, cy + (uint16_t)y, color);
        ILI9341_DrawPixel(cx - (uint16_t)x, cy + (uint16_t)y, color);
        ILI9341_DrawPixel(cx + (uint16_t)x, cy - (uint16_t)y, color);
        ILI9341_DrawPixel(cx - (uint16_t)x, cy - (uint16_t)y, color);
        ILI9341_DrawPixel(cx + (uint16_t)y, cy + (uint16_t)x, color);
        ILI9341_DrawPixel(cx - (uint16_t)y, cy + (uint16_t)x, color);
        ILI9341_DrawPixel(cx + (uint16_t)y, cy - (uint16_t)x, color);
        ILI9341_DrawPixel(cx - (uint16_t)y, cy - (uint16_t)x, color);
        if (d < 0) {
            d += 4 * x + 6;
        } else {
            d += 4 * (x - y) + 10;
            y--;
        }
        x++;
    }
}

/* ================================================================== */
/* 文字显示                                                            */
/* ================================================================== */

void ILI9341_DrawChar(uint16_t x, uint16_t y, char ch, uint16_t fg, uint16_t bg, uint8_t size)
{
    if (ch < 0x20 || ch > 0x7E) ch = '?';
    const uint8_t *bitmap = font8x8[(uint8_t)(ch - 0x20)];

    for (uint8_t row = 0; row < 8; row++) {
        for (uint8_t col = 0; col < 8; col++) {
            uint16_t color = (bitmap[row] & (0x80 >> col)) ? fg : bg;
            if (size == 1) {
                ILI9341_DrawPixel(x + col, y + row, color);
            } else {
                ILI9341_FillRegion(x + col * size,
                                   y + row * size,
                                   x + col * size + size - 1,
                                   y + row * size + size - 1,
                                   color);
            }
        }
    }
}

void ILI9341_DrawString(uint16_t x, uint16_t y, const char *str,
                        uint16_t fg, uint16_t bg, uint8_t size)
{
    uint16_t cx = x;
    uint8_t char_w = 8 * size;
    uint8_t char_h = 8 * size;

    while (*str) {
        if (*str == '\n') {
            cx  = x;
            y  += char_h;
        } else if (*str == '\r') {
            cx = x;
        } else {
            if (cx + char_w > ILI9341_WIDTH) {
                cx  = x;
                y  += char_h;
            }
            ILI9341_DrawChar(cx, y, *str, fg, bg, size);
            cx += char_w;
        }
        str++;
    }
}

/* ================================================================== */
/* 背光控制                                                            */
/* ================================================================== */

void ILI9341_BacklightOn(void)
{
    ILI9341_BK_ON();
}

void ILI9341_BacklightOff(void)
{
    ILI9341_BK_OFF();
}

/* ================================================================== */
/* 颜色工具                                                            */
/* ================================================================== */

uint16_t ILI9341_RGB(uint8_t r, uint8_t g, uint8_t b)
{
    return (uint16_t)(((r & 0xF8) << 8) | ((g & 0xFC) << 3) | (b >> 3));
}
