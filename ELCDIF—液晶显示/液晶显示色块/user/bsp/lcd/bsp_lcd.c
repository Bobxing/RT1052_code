/**
  ******************************************************************
  * @file    bsp_lcd.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   lcd应用函数接口
  ******************************************************************
  * @attention
  *
  * 实验平台:野火  i.MXRT1052开发板 
  * 论坛    :http://www.firebbs.cn
  * 淘宝    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
#include "fsl_iomuxc.h"
#include "fsl_gpio.h"  
#include "fsl_elcdif.h" 
#include "fsl_clock.h"
#include "fsl_pxp.h"

	
#include "pad_config.h"  
#include "./lcd/bsp_lcd.h" 



/*******************************************************************************
 * 变量
 ******************************************************************************/

/* 帧中断标志 */
volatile bool s_frameDone = false;

/* 显存 */
AT_NONCACHEABLE_SECTION_ALIGN(uint32_t s_psBufferLcd[2][LCD_PIXEL_HEIGHT][LCD_PIXEL_WIDTH], FRAME_BUFFER_ALIGN);


/*******************************************************************************
 * 宏
 ******************************************************************************/
/* 所有引脚均使用同样的PAD配置 */
#define LCD_PAD_CONFIG_DATA            (SRE_0_SLOW_SLEW_RATE| \
                                        DSE_6_R0_6| \
                                        SPEED_1_MEDIUM_100MHz| \
                                        ODE_0_OPEN_DRAIN_DISABLED| \
                                        PKE_1_PULL_KEEPER_ENABLED| \
                                        PUE_0_KEEPER_SELECTED| \
                                        PUS_0_100K_OHM_PULL_DOWN| \
                                        HYS_0_HYSTERESIS_DISABLED)   
    /* 配置说明 : */
    /* 转换速率: 转换速率慢
        驱动强度: R0/6 
        带宽配置 : medium(100MHz)
        开漏配置: 关闭 
        拉/保持器配置: 使能
        拉/保持器选择: 保持器
        上拉/下拉选择: 100K欧姆下拉(选择了保持器此配置无效)
        滞回器配置: 禁止 */
        
/*******************************************************************************
 * 声明
 ******************************************************************************/
static void LCD_IOMUXC_MUX_Config(void);
static void LCD_IOMUXC_PAD_Config(void);
static void LCD_ELCDIF_Config(void);


/**
* @brief  初始化LCD相关IOMUXC的MUX复用配置
* @param  无
* @retval 无
*/
static void LCD_IOMUXC_MUX_Config(void)
{
    /* 所有引脚均不开启SION功能 */
    /* 时序控制信号线 */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_00_LCD_CLK, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_01_LCD_ENABLE, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_02_LCD_HSYNC, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_03_LCD_VSYNC, 0U);
  
    /* RGB565数据信号线，
     DATA0~DATA4:B3~B7
     DATA5~DATA10:G2~G7
     DATA11~DATA15:R3~R7 */
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_04_LCD_DATA00, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_05_LCD_DATA01, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_06_LCD_DATA02, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_07_LCD_DATA03, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_08_LCD_DATA04, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_09_LCD_DATA05, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_10_LCD_DATA06, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_11_LCD_DATA07, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_12_LCD_DATA08, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_13_LCD_DATA09, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_14_LCD_DATA10, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B0_15_LCD_DATA11, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_00_LCD_DATA12, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_01_LCD_DATA13, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_02_LCD_DATA14, 0U);                                    
    IOMUXC_SetPinMux(IOMUXC_GPIO_B1_03_LCD_DATA15, 0U);   
    
    /* LCD_BL背光控制信号线 */
    IOMUXC_SetPinMux(LCD_BL_IOMUXC, 0U); 
}


/**
* @brief  初始化LCD相关IOMUXC的PAD属性配置
* @param  无
* @retval 无
*/
static void LCD_IOMUXC_PAD_Config(void)
{  
    /* 所有引脚均使用同样的PAD配置 */
    /* 时序控制信号线 */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_00_LCD_CLK,LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_01_LCD_ENABLE, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_02_LCD_HSYNC, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_03_LCD_VSYNC, LCD_PAD_CONFIG_DATA); 

    /* RGB565数据信号线，
     DATA0~DATA4:B3~B7
     DATA5~DATA10:G2~G7
     DATA11~DATA15:R3~R7 */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_04_LCD_DATA00, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_05_LCD_DATA01, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_06_LCD_DATA02, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_07_LCD_DATA03, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_08_LCD_DATA04, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_09_LCD_DATA05, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_10_LCD_DATA06, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_11_LCD_DATA07, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_12_LCD_DATA08, LCD_PAD_CONFIG_DATA);  
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_13_LCD_DATA09, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_14_LCD_DATA10, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B0_15_LCD_DATA11, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_00_LCD_DATA12, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_01_LCD_DATA13, LCD_PAD_CONFIG_DATA);
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_02_LCD_DATA14, LCD_PAD_CONFIG_DATA); 
    IOMUXC_SetPinConfig(IOMUXC_GPIO_B1_03_LCD_DATA15, LCD_PAD_CONFIG_DATA); 
    
    /* LCD_BL背光控制信号线 */
    IOMUXC_SetPinConfig(IOMUXC_GPIO_AD_B0_15_GPIO1_IO15, LCD_PAD_CONFIG_DATA);
}

/**
* @brief  初始化ELCDIF外设
* @param  无
* @retval 无
*/
static void LCD_ELCDIF_Config(void)
{	
    const elcdif_rgb_mode_config_t config = {
        .panelWidth = LCD_PIXEL_WIDTH,
        .panelHeight = LCD_PIXEL_HEIGHT,
        .hsw = APP_HSW,
        .hfp = APP_HFP,
        .hbp = APP_HBP,
        .vsw = APP_VSW,
        .vfp = APP_VFP,
        .vbp = APP_VBP,
        .polarityFlags = APP_POL_FLAGS,
        .bufferAddr = (uint32_t)s_psBufferLcd[0],
        .pixelFormat = kELCDIF_PixelFormatXRGB8888,
        .dataBus = APP_LCDIF_DATA_BUS,
    };
 
  ELCDIF_RgbModeInit(APP_ELCDIF, &config);
  ELCDIF_RgbModeStart(APP_ELCDIF);
}

/**
* @brief  初始化ELCDIF使用的时钟
* @param  无
* @retval 无
*/
void LCD_InitClock(void)
{
    /*
     * 要把帧率设置成60Hz，所以像素时钟频率为:
     * 水平像素时钟个数：(APP_IMG_WIDTH + APP_HSW + APP_HFP + APP_HBP ) 
     * 垂直像素时钟个数：(APP_IMG_HEIGHT + APP_VSW + APP_VFP + APP_VBP)
     * 
     * 像素时钟频率：(800 + 1 + 10 + 46) * (480 + 1 + 22 + 23) * 60 = 27.05M.
     * 本例子设置 LCDIF 像素时钟频率为 27M.
     */

    /*
     * 初始化 Video PLL.
     * Video PLL 输出频率为 
     * OSC24M * (loopDivider + (denominator / numerator)) / postDivider = 108MHz.
     */
    clock_video_pll_config_t config = {
        .loopDivider = 36, .postDivider = 8, .numerator = 0, .denominator = 0,
    };

    CLOCK_InitVideoPll(&config);

    /*
     * 000 derive clock from PLL2
     * 001 derive clock from PLL3 PFD3
     * 010 derive clock from PLL5
     * 011 derive clock from PLL2 PFD0
     * 100 derive clock from PLL2 PFD1
     * 101 derive clock from PLL3 PFD1
     */
    /* 选择为vedio PLL*/
    CLOCK_SetMux(kCLOCK_LcdifPreMux, 2);

    /* 设置分频 */  
    CLOCK_SetDiv(kCLOCK_LcdifPreDiv, 1);

    CLOCK_SetDiv(kCLOCK_LcdifDiv, 1);
}

/**
* @brief  初始化背光引脚并点亮
* @param  无
* @retval 无
*/
void LCD_BackLight_ON(void)
{    
    /* 背光，高电平点亮 */
    gpio_pin_config_t config = {
      kGPIO_DigitalOutput, 
      1,
      kGPIO_NoIntmode
    };

    GPIO_PinInit(LCD_BL_GPIO, LCD_BL_GPIO_PIN, &config);
}



/**
* @brief  初始化液晶屏
* @param  enableInterrupt ：是否使能中断
* @retval 无
*/
void LCD_Init(bool enableInterrupt)
{
  *((uint32_t *)0x41044100) = 0x0000000f;
	*((uint32_t *)0x41044104) = 0x0000000f;

  LCD_IOMUXC_MUX_Config();
  LCD_IOMUXC_PAD_Config();
  LCD_ELCDIF_Config();
  LCD_InitClock();
  LCD_BackLight_ON();
  
  if(enableInterrupt)
  {
    LCD_InterruptConfig();
  }
}

/***************************中断相关******************************/
/**
* @brief  配置ELCDIF中断
* @param  无
* @retval 无
*/
void LCD_InterruptConfig(void)
{
  /* 使能中断 */
  EnableIRQ(LCDIF_IRQn);
   
  /* 配置ELCDIF为CurFrameDoneInterrupt中断 */
  ELCDIF_EnableInterrupts(APP_ELCDIF, kELCDIF_CurFrameDoneInterruptEnable);
}

/**
* @brief  ELCDIF中断服务函数
* @param  无
* @retval 无
*/
void LCDIF_IRQHandler(void)
{
    uint32_t intStatus;

    intStatus = ELCDIF_GetInterruptStatus(APP_ELCDIF);

    ELCDIF_ClearInterruptStatus(APP_ELCDIF, intStatus);

    if (intStatus & kELCDIF_CurFrameDone)
    {
        s_frameDone = true;
    }

    /* 以下部分是为 ARM 的勘误838869添加的, 
       该错误影响 Cortex-M4, Cortex-M4F内核，       
       立即存储覆盖重叠异常，导致返回操作可能会指向错误的中断
        CM7不受影响，此处保留该代码
    */  
#if defined __CORTEX_M && (__CORTEX_M == 4U)
    __DSB();
#endif
}

/*********************************************END OF FILE**********************/
