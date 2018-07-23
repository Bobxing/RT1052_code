/**
  ******************************************************************
  * @file    lcd_test.c
  * @author  fire
  * @version V1.0
  * @date    2018-xx-xx
  * @brief   lcd����Ӧ�ýӿ�
  ******************************************************************
  * @attention
  *
  * ʵ��ƽ̨:Ұ��  i.MXRT1052������ 
  * ��̳    :http://www.firebbs.cn
  * �Ա�    :https://fire-stm32.taobao.com
  *
  ******************************************************************
  */
  
#include "fsl_elcdif.h"

#include "./lcd/bsp_lcd.h" 


/**
* @brief  ��Һ��������������ͼ��
* @param  ��
* @retval ��
*/
void APP_FillFrameBuffer(uint32_t frameBuffer[LCD_PIXEL_HEIGHT][LCD_PIXEL_WIDTH])
{
    /* �����ƶ��ٶ� */
    #define   SPEED     10
  
    /* Background color. */
    static const uint32_t bgColor = 0U;
    /* Foreground color. */
    static uint8_t fgColorIndex = 0U;
    static const uint32_t fgColorTable[] = {0x000000FFU, 0x0000FF00U, 0x0000FFFFU, 0x00FF0000U,
                                            0x00FF00FFU, 0x00FFFF00U, 0x00FFFFFFU};
    uint32_t fgColor = fgColorTable[fgColorIndex];

    /* Position of the foreground rectangle. */
    static uint16_t upperLeftX = 0U;
    static uint16_t upperLeftY = 0U;
    static uint16_t lowerRightX = (LCD_PIXEL_WIDTH - 1U) / 2U;
    static uint16_t lowerRightY = (LCD_PIXEL_HEIGHT - 1U) / 2U;

    static int8_t incX = SPEED;
    static int8_t incY = SPEED;

    /* Change color in next forame or not. */
    static bool changeColor = false;

    uint32_t i, j;

    /* Background color. */
    for (i = 0; i < LCD_PIXEL_HEIGHT; i++)
    {
        for (j = 0; j < LCD_PIXEL_WIDTH; j++)
        {
            frameBuffer[i][j] = bgColor;
        }
    }

    /* Foreground color. */
    for (i = upperLeftY; i < lowerRightY; i++)
    {
        for (j = upperLeftX; j < lowerRightX; j++)
        {
            frameBuffer[i][j] = fgColor;
        }
    }

    /* Update the format: color and rectangle position. */
    upperLeftX += incX;
    upperLeftY += incY;
    lowerRightX += incX;
    lowerRightY += incY;

    changeColor = false;

    if (0U == upperLeftX)
    {
        incX = SPEED;
        changeColor = true;
    }
    else if (LCD_PIXEL_WIDTH - 1 == lowerRightX)
    {
        incX = -SPEED;
        changeColor = true;
    }

    if (0U == upperLeftY)
    {
        incY = SPEED;
        changeColor = true;
    }
    else if (LCD_PIXEL_HEIGHT - 1 == lowerRightY)
    {
        incY = -SPEED;
        changeColor = true;
    }

    if (changeColor)
    {
        fgColorIndex++;

        if (ARRAY_SIZE(fgColorTable) == fgColorIndex)
        {
            fgColorIndex = 0U;
        }
    }
}

/**
* @brief  ��Һ��������������ͼ��
* @param  ��
* @retval ��
*/
void APP_ChangeAndSetNextBuffer(void)
{
  static uint8_t frameBufferIndex = 0;

  /* �л��������� */
  frameBufferIndex ^= 1U;

  /* �����һ�������� */
  APP_FillFrameBuffer(s_psBufferLcd[frameBufferIndex]);

  /* ����ELCDIF����һ����������ַ */
  ELCDIF_SetNextBufferAddr(APP_ELCDIF, (uint32_t)s_psBufferLcd[frameBufferIndex]);

  /* �����жϱ�־ */
  s_frameDone = false;
  /* �ȴ�ֱ���ж���� */
  while (!s_frameDone)
  {
  }
}

/*********************************************END OF FILE**********************/
