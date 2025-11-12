/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.4
  * @date    30/05/2024
  * @brief   Main program body.
  ******************************************************************************
  * <br><br>
  *
  * THE PRESENT FIRMWARE IS FOR GUIDANCE ONLY. IT AIMS AT PROVIDING CUSTOMERS
  * WITH CODING INFORMATION REGARDING MILANDR'S PRODUCTS IN ORDER TO FACILITATE
  * THE USE AND SAVE TIME. MILANDR SHALL NOT BE HELD LIABLE FOR ANY
  * DIRECT, INDIRECT OR CONSEQUENTIAL DAMAGES RESULTING
  * FROM THE CONTENT OF SUCH FIRMWARE AND/OR A USE MADE BY CUSTOMERS OF THE
  * CODING INFORMATION CONTAINED HEREIN IN THEIR PRODUCTS.
  *
  * <h2><center>&copy; COPYRIGHT 2025 Milandr</center></h2>
  */

/* Includes ------------------------------------------------------------------*/
#include "MDR32FxQI_port.h"
#include "MDR32FxQI_rst_clk.h"
#include "MDR32FxQI_eeprom.h"
#include "MDR32FxQI_bkp.h"

/** @addtogroup __MDR32FxQI_StdPeriph_Examples MDR32FxQI StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR32F9Q2I_EVAL MDR32F9Q2I Evaluation Board
  * @{
  */

/** @addtogroup RST_CLK_CPU_Clock_MDR32F9Q2I RST_CLK_CPU_Clock
  * @{
  */

/* Private define ------------------------------------------------------------*/
#define BLINK_NUM 5
#define BLINK_DELAY 200000

#define LED1            PORT_Pin_7

/* Private variables ---------------------------------------------------------*/
RST_CLK_FreqTypeDef CLK_FrequencyStructure;
PORT_InitTypeDef PORT_InitStructure;

/* Private function prototypes -----------------------------------------------*/
void Delay(__IO uint32_t nCount);

/* Private functions ---------------------------------------------------------*/

void LEDOn(uint32_t LED_Num)
{
    PORT_SetBits(MDR_PORTB, LED_Num);
}

void LEDOff(uint32_t LED_Num)
{
    PORT_ResetBits(MDR_PORTB, LED_Num);
}

/**
  * @brief  Blink with LED1
  * @param  num - blinks number
  * @param  del - delay
  * @retval None
  */
void BlinkLED1(uint32_t num, uint32_t del)
{
    uint32_t cnt;

    for ( cnt = 0; cnt < num; cnt++)
    {
        LEDOn(LED1);
        Delay(del);
        LEDOff(LED1);
        Delay(del);
    }
}

/**
  * @brief  Indicate error condition with LED3
  * @param  None
  * @retval None
  */
void IndicateError(void)
{
    /* Switch LED3 on and off in case of error */
    LEDOn(LED1);
    Delay(BLINK_DELAY);
    LEDOff(LED1);
}

/**
  * @brief  Main program.
  * @param  None
  * @retval None
  */
int main(void)
{
	  RST_CLK_DeInit();
    SystemCoreClockUpdate();
	
		RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	
		if (RST_CLK_HSEstatus() != SUCCESS) {
				while(1);
		}
	
		RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv2, RST_CLK_CPU_PLLmul4);
	
	  RST_CLK_CPU_PLLcmd(ENABLE);

    // Wait for CPU PLL to lock
    if (RST_CLK_CPU_PLLstatus() != SUCCESS) {        
        /* Trap if PLL is not successfully locked */        
        while (1)
            ;    
    }

    // Set CPU clock prescaler to 1 (no division)
    RST_CLK_CPUclkPrescaler(RST_CLK_CPUclkDIV1);

    // Enable CPU PLL as the clock source
    RST_CLK_CPU_PLLuse(ENABLE);

    // Select CPU clock C3 (PLL output as the CPU clock source)
    RST_CLK_CPUclkSelection(RST_CLK_CPUclkCPU_C3);
	
		RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_SSP1 | RST_CLK_PCLK_SSP2), ENABLE);
		RST_CLK_PCLKcmd((RST_CLK_PCLK_PORTF | RST_CLK_PCLK_PORTD | RST_CLK_PCLK_PORTC | RST_CLK_PCLK_PORTA| RST_CLK_PCLK_PORTB), ENABLE);
		
		PORT_DeInit(MDR_PORTB);
		
		PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin   = (PORT_Pin_7);
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_PORT;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_SLOW;		
		
		PORT_Init(MDR_PORTB, &PORT_InitStructure);
		//BlinkLED1(5, 1000);
		while(1) {
			BlinkLED1(5, 100000);
			Delay(2000000);
		};
}

/**
  * @brief  Executes delay loop.
  * @param  nCount: specifies the delay.
  * @retval None.
  */
void Delay(__IO uint32_t nCount)
{
    for (; nCount != 0; nCount--);
}

/**
  * @brief  Reports the source file name, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file: pointer to the source file name.
  * @param  line: assert_param error line source number.
  * @param  expr:
  * @retval None.
  */
#if (USE_ASSERT_INFO == 1)
void assert_failed(uint8_t* file, uint32_t line)
{
    /* User can add his own implementation to report the source file name and line number.
       Ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

    /* Infinite loop */
    while(1) {}
}
#elif (USE_ASSERT_INFO == 2)
void assert_failed(uint8_t* file, uint32_t line, const uint8_t* expr)
{
    /* User can add his own implementation to report the source file name, line number and
       expression text.
       Ex: printf("Wrong parameters value (%s): file %s on line %d\r\n", expr, file, line) */

    /* Infinite loop */
    while(1) {}
}
#endif /* USE_ASSERT_INFO */

/** @} */ /* End of group RST_CLK_CPU_Clock_MDR32F9Q2I */

/** @} */ /* End of group __MDR32F9Q2I_EVAL */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2025 Milandr *******************************/

/* END OF FILE main.c */


