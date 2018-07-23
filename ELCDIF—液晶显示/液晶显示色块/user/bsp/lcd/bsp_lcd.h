#ifndef __BSP_LCD_H
#define	__BSP_LCD_H

#include "fsl_common.h"


#define APP_ELCDIF    LCDIF
#define APP_PXP       PXP

/* LCD �ֱ��� */
#define LCD_PIXEL_WIDTH     800
#define LCD_PIXEL_HEIGHT    480
//���ݹٷ�demo
#define APP_IMG_WIDTH   LCD_PIXEL_WIDTH
#define APP_IMG_HEIGHT  LCD_PIXEL_HEIGHT

/* LCDʱ�� */
#define APP_HSW 1
#define APP_HFP 22
#define APP_HBP 46
#define APP_VSW 1
#define APP_VFP 22
#define APP_VBP 23
#define APP_POL_FLAGS \
    (kELCDIF_DataEnableActiveHigh | kELCDIF_VsyncActiveLow | kELCDIF_HsyncActiveLow | kELCDIF_DriveDataOnRisingClkEdge)


/* LCD����������*/
#define APP_LCDIF_DATA_BUS    kELCDIF_DataBus16Bit


/* LCD�������ţ��ߵ�ƽ���� */
#define LCD_BL_GPIO 				      GPIO1
#define LCD_BL_GPIO_PIN 		      (15U)
#define LCD_BL_IOMUXC			        IOMUXC_GPIO_AD_B0_15_GPIO1_IO15

/* ��������������, Ϊ���������, LCDIF������Ҫ64B���� */
#define FRAME_BUFFER_ALIGN    64


/* PS input buffer is square. */
#if APP_IMG_WIDTH > APP_IMG_HEIGHT
#define APP_PS_SIZE (APP_IMG_HEIGHT / 2U)
#else
#define APP_PS_SIZE (APP_IMG_WIDTH / 2U)
#endif

#define APP_PS_ULC_X 0U
#define APP_PS_ULC_Y 0U

#define APP_BPP 4U /* Use 24-bit RGB888 format. */

/* ��������ʹ�õ���Һ�����ظ�ʽ�� */
#define LCD_XRGB_8888             1
//#define LCD_RGB_888             1
//#define LCD_RGB_565             1

#define LCD_INTERRUPT_DISABLE   0
#define LCD_INTERRUPT_ENABLE    (!LCD_INTERRUPT_DISABLE)


/** 
  * @brief  LCD color  
  */
#define LCD_COLOR_WHITE          0xFFFFFF
#define LCD_COLOR_BLACK          0x000000
#define LCD_COLOR_GREY           0xC0C0C0
#define LCD_COLOR_BLUE           0x0000FF
#define LCD_COLOR_BLUE2          0x0000A0
#define LCD_COLOR_RED            0xFF0000
#define LCD_COLOR_MAGENTA        0xFF00FF
#define LCD_COLOR_GREEN          0x00FF00
#define LCD_COLOR_CYAN           0x00FFFF
#define LCD_COLOR_YELLOW         0xFFFF00


/*******************************************************************************
 * ȫ�ֱ�������
 ******************************************************************************/
extern volatile bool s_frameDone;
extern uint32_t s_psBufferLcd[2][APP_IMG_HEIGHT][APP_IMG_WIDTH];

/*******************************************************************************
 * ��������
 ******************************************************************************/
void LCD_BackLight_ON(void);
void LCD_Init(bool enableInterrupt);
void LCD_InterruptConfig(void);


#endif /* __BSP_LCD_H */
