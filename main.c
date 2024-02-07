#include "stm32l1xx.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_tim.h"
#include "stm32l1xx_ll_exti.h"

//note
#define A_4	(uint16_t) 932
#define A_6	(uint16_t) 1864
#define MUTE (uint16_t) 1
/*for 10ms update event*/
#define TIMx_PSC			3
/*Macro function for ARR calculation*/
#define ARR_CALCULATE(N) (SystemCoreClock) / ((TIMx_PSC) * (N))
void TIM_BASE_Confignote(uint16_t);
void TIM_OC_GPIO_Config(void);
void TIM_OC_Confignote(uint16_t);

//7-segment
void SystemClock_Config(void);
void GPIO_Config(void);
void ltc4727js_config(void);
//water pump
void TIM_OC_GPIO_Config(void);
void TIM_OC_Config(void);
void TIM_BASE_Config(void);
void l293d_Config(void);

int main(void)
{
	SystemClock_Config();
	GPIO_Config();
  ltc4727js_config();
	l293d_Config();
  TIM_OC_Config();

	uint8_t i;
	uint32_t segF [4] = {LL_GPIO_PIN_2 | LL_GPIO_PIN_14 | LL_GPIO_PIN_11 | LL_GPIO_PIN_15 | LL_GPIO_PIN_12,
	                     LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 |LL_GPIO_PIN_14,
	                     LL_GPIO_PIN_2 | LL_GPIO_PIN_14 | LL_GPIO_PIN_11 | LL_GPIO_PIN_15 | LL_GPIO_PIN_12}; //sos
	uint32_t digit [4] ={LL_GPIO_PIN_0, LL_GPIO_PIN_1, LL_GPIO_PIN_2};
  
	while (1){
  int button_pressed = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_8);
	if (button_pressed == 0) {
		  //speaker
		  TIM_OC_Confignote(ARR_CALCULATE(A_4));
			LL_mDelay(300);
			TIM_OC_Confignote(ARR_CALCULATE(A_6));
		  LL_mDelay(300);
		  //water pump
		  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_7);
      LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_4);
      LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
		  
     //7-segment
	   for (i= 0; i < 4; ++i){
		    LL_GPIO_ResetOutputPin (GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3); //Write 0 to GPIOC port LL_GPIO_ResetOutputPin (GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL GPIO_PIN_11 | LL_GPIO_PIN_12 |
		    LL_GPIO_ResetOutputPin (GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); //Reset all segment
		    LL_GPIO_SetOutputPin (GPIOB, segF[i]);
		    LL_GPIO_SetOutputPin (GPIOC, digit[i]);
		    LL_mDelay(0); //USE for DEBUG increase delay to see what's happenning when 7-seg is lit
		  }
	  }else{
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_7);
      LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_4);
      LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_5);
			TIM_OC_Confignote(MUTE);
		}
  }
}

 void GPIO_Config(void){
	 
	  // Declare struct for GPIO
    LL_GPIO_InitTypeDef GPIO_InitStruct;
	 
	 // Enable GPIOB clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	 
	  // Config GPIOA using struct
    GPIO_InitStruct.Mode = LL_GPIO_MODE_INPUT;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_8;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    LL_GPIO_Init(GPIOA, &GPIO_InitStruct);

 }

void ltc4727js_config(void){
	LL_GPIO_InitTypeDef ltc4727_initstruct;
	//configure ltc4727js
	LL_AHB1_GRP1_EnableClock (LL_AHB1_GRP1_PERIPH_GPIOB);
	LL_AHB1_GRP1_EnableClock (LL_AHB1_GRP1_PERIPH_GPIOC);

	ltc4727_initstruct.Mode = LL_GPIO_MODE_OUTPUT;
	ltc4727_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	ltc4727_initstruct.Pull = LL_GPIO_PULL_NO;
	ltc4727_initstruct.Speed =LL_GPIO_SPEED_FREQ_VERY_HIGH;
	ltc4727_initstruct. Pin = LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15; //Enable PB
	LL_GPIO_Init(GPIOB, &ltc4727_initstruct);

	ltc4727_initstruct. Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2; //Enable PC
	LL_GPIO_Init(GPIOC, &ltc4727_initstruct);
	
}

//IC l293d Config
void l293d_Config(void)
{
  LL_GPIO_InitTypeDef l293d_init;
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  l293d_init.Mode = LL_GPIO_MODE_OUTPUT;
  l293d_init.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  l293d_init.Pull = LL_GPIO_PULL_NO;
  l293d_init.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  l293d_init.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_5 | LL_GPIO_PIN_7;
  LL_GPIO_Init(GPIOB, &l293d_init);
}
//TIM3_BASE_Config
void TIM_BASE_Config(void)
{
  LL_TIM_InitTypeDef timbase_initstructure;
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM3);
  //Time-base configure
  timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV2;
  timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
  timbase_initstructure.Autoreload = 100 - 1; //Set ARR to 100 for Compare
  timbase_initstructure.Prescaler = 32000 - 1;
  LL_TIM_Init(TIM3, &timbase_initstructure);
  LL_TIM_EnableCounter(TIM3);
}
//TIM_OC_Setup PIN4
void TIM_OC_GPIO_Config(void)
{
  LL_GPIO_InitTypeDef gpio_initstructure;
  LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
  gpio_initstructure.Mode = LL_GPIO_MODE_ALTERNATE;
  gpio_initstructure.Alternate = LL_GPIO_AF_2;
  gpio_initstructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
  gpio_initstructure.Pin = LL_GPIO_PIN_4;
  gpio_initstructure.Pull = LL_GPIO_PULL_NO;
  gpio_initstructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
  LL_GPIO_Init(GPIOB, &gpio_initstructure);
}
//TIM3_OC_Config
void TIM_OC_Config()
{
  LL_TIM_OC_InitTypeDef tim_oc_initstructure;
  TIM_OC_GPIO_Config();
  TIM_BASE_Config();
  tim_oc_initstructure.OCState = LL_TIM_OCSTATE_DISABLE;
  tim_oc_initstructure.OCMode = LL_TIM_OCMODE_PWM1;
  tim_oc_initstructure.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
  tim_oc_initstructure.CompareValue = 100; //Compare with ARR to change duty cycle 100%
  LL_TIM_OC_Init(TIM3, LL_TIM_CHANNEL_CH1, &tim_oc_initstructure);
  //Enable TIM3_CH1
  /*Start Output Compare in PWM Mode*/
  LL_TIM_CC_EnableChannel(TIM3, LL_TIM_CHANNEL_CH1);
  LL_TIM_EnableCounter(TIM3);
}

void TIM_BASE_Confignote(uint16_t ARR)
{
	LL_TIM_InitTypeDef timbase_initstructure;
	
	LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_TIM4);
	//Time-base configure
	timbase_initstructure.ClockDivision = LL_TIM_CLOCKDIVISION_DIV1;
	timbase_initstructure.CounterMode = LL_TIM_COUNTERMODE_UP;
	timbase_initstructure.Autoreload = ARR - 1;
	timbase_initstructure.Prescaler =  TIMx_PSC- 1;
	LL_TIM_Init(TIM4, &timbase_initstructure);
	
	LL_TIM_EnableCounter(TIM2); 
}


void TIM_OC_GPIO_Confignote(void)
{
	LL_GPIO_InitTypeDef gpio_initstructure;
	
	LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	
	gpio_initstructure.Mode = LL_GPIO_MODE_ALTERNATE;
	gpio_initstructure.Alternate = LL_GPIO_AF_2;
	gpio_initstructure.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
	gpio_initstructure.Pin = LL_GPIO_PIN_6;
	gpio_initstructure.Pull = LL_GPIO_PULL_NO;
	gpio_initstructure.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
	LL_GPIO_Init(GPIOB, &gpio_initstructure);
}

void TIM_OC_Confignote(uint16_t note)
{
	LL_TIM_OC_InitTypeDef tim_oc_initstructure;
	
	TIM_OC_GPIO_Confignote();
	TIM_BASE_Confignote(note);
	
	tim_oc_initstructure.OCState = LL_TIM_OCSTATE_DISABLE;
	tim_oc_initstructure.OCMode = LL_TIM_OCMODE_PWM1;
	tim_oc_initstructure.OCPolarity = LL_TIM_OCPOLARITY_HIGH;
	tim_oc_initstructure.CompareValue = LL_TIM_GetAutoReload(TIM4) / 2;
	LL_TIM_OC_Init(TIM4, LL_TIM_CHANNEL_CH1, &tim_oc_initstructure);
	/*Interrupt Configure*/
	NVIC_SetPriority(TIM4_IRQn, 1);
	NVIC_EnableIRQ(TIM4_IRQn);
	LL_TIM_EnableIT_CC1(TIM4);
	
	/*Start Output Compare in PWM Mode*/
	LL_TIM_CC_EnableChannel(TIM4, LL_TIM_CHANNEL_CH1);
	LL_TIM_EnableCounter(TIM4);
}

void TIM4_IRQHandler(void)
{
	if(LL_TIM_IsActiveFlag_CC1(TIM4) == SET)
	{
		LL_TIM_ClearFlag_CC1(TIM4);
	}
}


void SystemClock_Config(void)
{
  /* Enable ACC64 access and set FLASH latency */
  LL_FLASH_Enable64bitAccess();;
  LL_FLASH_SetLatency(LL_FLASH_LATENCY_1);

  /* Set Voltage scale1 as MCU will run at 32MHz */
  LL_APB1_GRP1_EnableClock(LL_APB1_GRP1_PERIPH_PWR);
  LL_PWR_SetRegulVoltageScaling(LL_PWR_REGU_VOLTAGE_SCALE1);
 
  /* Poll VOSF bit of in PWR_CSR. Wait until it is reset to 0 */
  while (LL_PWR_IsActiveFlag_VOSF() != 0)
  {
  };
 
  /* Enable HSI if not already activated*/
  if (LL_RCC_HSI_IsReady() == 0)
  {
    /* HSI configuration and activation */
    LL_RCC_HSI_Enable();
    while(LL_RCC_HSI_IsReady() != 1)
    {
    };
  }
 

  /* Main PLL configuration and activation */
  LL_RCC_PLL_ConfigDomain_SYS(LL_RCC_PLLSOURCE_HSI, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3);

  LL_RCC_PLL_Enable();
  while(LL_RCC_PLL_IsReady() != 1)
  {
  };
 
  /* Sysclk activation on the main PLL */
  LL_RCC_SetAHBPrescaler(LL_RCC_SYSCLK_DIV_1);
  LL_RCC_SetSysClkSource(LL_RCC_SYS_CLKSOURCE_PLL);
  while(LL_RCC_GetSysClkSource() != LL_RCC_SYS_CLKSOURCE_STATUS_PLL)
  {
  };
 
  /* Set APB1 & APB2 prescaler*/
  LL_RCC_SetAPB1Prescaler(LL_RCC_APB1_DIV_1);
  LL_RCC_SetAPB2Prescaler(LL_RCC_APB2_DIV_1);

  /* Set systick to 1ms in using frequency set to 32MHz                             */
  /* This frequency can be calculated through LL RCC macro                          */
  /* ex: __LL_RCC_CALC_PLLCLK_FREQ (HSI_VALUE, LL_RCC_PLL_MUL_6, LL_RCC_PLL_DIV_3); */
  LL_Init1msTick(32000000);
 
  /* Update CMSIS variable (which can be updated also through SystemCoreClockUpdate function) */
  LL_SetSystemCoreClock(32000000);
}
