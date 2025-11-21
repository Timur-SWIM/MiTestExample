/* Includes ------------------------------------------------------------------*/
#include "MDR32FxQI_port.h"
#include "MDR32FxQI_rst_clk.h"
#include "MDR32FxQI_timer.h"

#define LED_PORT     MDR_PORTB
#define LED1         PORT_Pin_7

/* Private variables ---------------------------------------------------------*/
PORT_InitTypeDef PORT_InitStructure;
TIMER_CntInitTypeDef sTIM_CntInit;
TIMER_ChnInitTypeDef sTIM_ChnInit;
TIMER_ChnOutInitTypeDef sTIM_ChnOutInit;

uint16_t CCR1_Val = 300+1;
/* Private function prototypes -----------------------------------------------*/
void ClockInit(void){
    RST_CLK_DeInit();
    SystemCoreClockUpdate();

		RST_CLK_HSEconfig(RST_CLK_HSE_ON);
	
		if (RST_CLK_HSEstatus() != SUCCESS) {
				while(1);
		}
	
		RST_CLK_CPU_PLLconfig(RST_CLK_CPU_PLLsrcHSEdiv1, RST_CLK_CPU_PLLmul4);
	
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
}

void PortInit(void) {
    /* Enable peripheral clocks */
    RST_CLK_PCLKcmd(RST_CLK_PCLK_PORTB, ENABLE);

    /* Reset PORTB settings */
    PORT_DeInit(LED_PORT);	
	
	/* Configure PORTB pin7 */
	PORT_StructInit(&PORT_InitStructure);
	PORT_InitStructure.PORT_Pin = LED1;
	PORT_InitStructure.PORT_OE = PORT_OE_OUT;
	PORT_InitStructure.PORT_FUNC = PORT_FUNC_OVERRID;
	PORT_InitStructure.PORT_MODE = PORT_MODE_DIGITAL;
	PORT_InitStructure.PORT_SPEED = PORT_SPEED_FAST;
	
	PORT_Init(LED_PORT, &PORT_InitStructure);
}

void TimerInit(void) {
	/* Enable peripheral clocks */
	RST_CLK_PCLKcmd(RST_CLK_PCLK_TIMER3, ENABLE);
	/* Reset all TIM3 settings */
	TIMER_DeInit(MDR_TIMER3);
	
	/* TIM3 config:
		Genreate PWM signal:
		TIM3CLK = 32MHz, Prescaller = 0, TIM3 counter clock = ?MHz
		TIM3 frequency = TIM3CLK/(TIM3_Period + 1) = ? KHz
		- TIM3 Channel4 duty cycle = TIM3->CCR1 / (TIM3_Period + 1) = ?%
	*/
	
	/* Init TIMx Counter */
	sTIM_CntInit.TIMER_Prescaler = 0;
	sTIM_CntInit.TIMER_Period    = 1280+1;
	sTIM_CntInit.TIMER_CounterMode = TIMER_CntMode_ClkFixedDir;
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
	
    /* Enable TIMER3 clock */
    TIMER_BRGInit(MDR_TIMER3,TIMER_HCLKdiv1);

    /* Enable TIMER3 */
    TIMER_Cmd(MDR_TIMER3,ENABLE);
}

int main(void) {
	
	ClockInit();
	PortInit();
	TimerInit();
	
	while(1) {}
}

