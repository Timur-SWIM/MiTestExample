/**
  ******************************************************************************
  * @file    main.c
  * @author  Milandr Application Team
  * @version V2.0.3
  * @date    22/06/2023
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
#include "MDR32FxQI_timer.h"
#include "MDR32FxQI_rst_clk.h"
#include "MDR32FxQI_port.h"

/** @addtogroup __MDR32FxQI_StdPeriph_Examples MDR32FxQI StdPeriph Examples
  * @{
  */

/** @addtogroup __MDR32F9Q2I_EVAL MDR32F9Q2I Evaluation Board
  * @{
  */

/** @addtogroup TIMER_5PWM_Output_MDR32F9Q2I TIMER_5PWM_Output
  * @{
  */

/* Private variables ---------------------------------------------------------*/
TIMER_CntInitTypeDef sTIM_CntInit;
TIMER_ChnInitTypeDef sTIM_ChnInit;
TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;
PORT_InitTypeDef PORT_InitStructure;

uint16_t CCR1_Val = 500;

/* Private functions ---------------------------------------------------------*/

/**
  * @brief  Main program
  * @param  None
  * @retval None
  */
int main(void)
{
    RST_CLK_DeInit();
    SystemCoreClockUpdate();

    /* Enable peripheral clocks */
    RST_CLK_PCLKcmd((RST_CLK_PCLK_RST_CLK | RST_CLK_PCLK_TIMER3 |
                    RST_CLK_PCLK_PORTB), ENABLE);

    /* Reset PORTA settings */
    PORT_DeInit(MDR_PORTB);

    /* Configure TIMER3 pins: CH3N */
    /* Configure PORTB pins 7 */
    PORT_StructInit(&PORT_InitStructure);
    PORT_InitStructure.PORT_Pin   = (PORT_Pin_7);
    PORT_InitStructure.PORT_OE    = PORT_OE_OUT;
    PORT_InitStructure.PORT_FUNC  = PORT_FUNC_OVERRID;
    PORT_InitStructure.PORT_MODE  = PORT_MODE_DIGITAL;
    PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
    PORT_Init(MDR_PORTB, &PORT_InitStructure);

    /* Reset all TIMER3 settings */
    TIMER_DeInit(MDR_TIMER3);

    /* TIM3 Configuration:
       TIM3CLK = 8 MHz, Prescaler = 8000, TIM3 counter clock = 1 kHz
       TIM3 frequency = TIM3CLK/(TIM3_Period + 1) = 1 Hz
        - TIM3 Channel1 duty cycle = TIM3->CCR1 / (TIM3_Period + 1) = 50%
    */

    /* Initializes the TIMERx Counter */
    sTIM_CntInit.TIMER_Prescaler        = 8000-1;
    sTIM_CntInit.TIMER_Period           = 1000-1;
    sTIM_CntInit.TIMER_CounterMode      = TIMER_CntMode_ClkFixedDir;
    sTIM_CntInit.TIMER_CounterDirection = TIMER_CntDir_Up;
    sTIM_CntInit.TIMER_EventSource      = TIMER_EvSrc_TIM_CLK;
    sTIM_CntInit.TIMER_FilterSampling   = TIMER_FDTS_TIMER_CLK_div_1;
    sTIM_CntInit.TIMER_ARR_UpdateMode   = TIMER_ARR_Update_Immediately;
    sTIM_CntInit.TIMER_ETR_FilterConf   = TIMER_Filter_1FF_at_TIMER_CLK;
    sTIM_CntInit.TIMER_ETR_Prescaler    = TIMER_ETR_Prescaler_None;
    sTIM_CntInit.TIMER_ETR_Polarity     = TIMER_ETRPolarity_NonInverted;
    sTIM_CntInit.TIMER_BRK_Polarity     = TIMER_BRKPolarity_NonInverted;
    TIMER_CntInit(MDR_TIMER3,&sTIM_CntInit);

    /* Initializes the TIMER3 Channel 4 */
    TIMER_ChnStructInit(&sTIM_ChnInit);

    sTIM_ChnInit.TIMER_CH_Mode       = TIMER_CH_MODE_PWM;
    sTIM_ChnInit.TIMER_CH_REF_Format = TIMER_CH_REF_Format6;
    sTIM_ChnInit.TIMER_CH_Number     = TIMER_CHANNEL4;
    TIMER_ChnInit(MDR_TIMER3, &sTIM_ChnInit);


    TIMER_SetChnCompare(MDR_TIMER3, TIMER_CHANNEL4, CCR1_Val);

    /* Initializes the TIMER3 Channel 4 */
    TIMER_ChnOutStructInit(&sTIM_ChnOutInit);
    sTIM_ChnOutInit.TIMER_CH_DirOut_Polarity = TIMER_CHOPolarity_NonInverted;
    sTIM_ChnOutInit.TIMER_CH_DirOut_Source   = TIMER_CH_OutSrc_REF;
    sTIM_ChnOutInit.TIMER_CH_DirOut_Mode     = TIMER_CH_OutMode_Output;
    sTIM_ChnOutInit.TIMER_CH_NegOut_Polarity = TIMER_CHOPolarity_NonInverted;
    sTIM_ChnOutInit.TIMER_CH_NegOut_Source   = TIMER_CH_OutSrc_REF;
    sTIM_ChnOutInit.TIMER_CH_NegOut_Mode     = TIMER_CH_OutMode_Output;
    sTIM_ChnOutInit.TIMER_CH_Number          = TIMER_CHANNEL4;
    TIMER_ChnOutInit(MDR_TIMER3, &sTIM_ChnOutInit);

    /* Enable TIMER1 clock */
    TIMER_BRGInit(MDR_TIMER3,TIMER_HCLKdiv1);

    /* Enable TIMER1 */
    TIMER_Cmd(MDR_TIMER3,ENABLE);

    while(1) {}
}

/**
  * @brief  Reports the source file name, the source line number
  *         and expression text (if USE_ASSERT_INFO == 2) where
  *         the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @param  expr:
  * @retval None
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

/** @} */ /* End of group TIMER_5PWM_Output_MDR32F9Q2I */

/** @} */ /* End of group __MDR32F9Q2I_EVAL */

/** @} */ /* End of group __MDR32FxQI_StdPeriph_Examples */

/******************* (C) COPYRIGHT 2025 Milandr *******************************/

/* END OF FILE main.c */


