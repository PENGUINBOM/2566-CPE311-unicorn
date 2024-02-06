#include "stm32l1xx.h"
#include "stm32l1xx_ll_system.h"
#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_pwr.h"
#include "stm32l1xx_ll_rcc.h"
#include "stm32l1xx_ll_utils.h"
#include "stm32l1xx_ll_tim.h"
#include "stm32l1xx_ll_exti.h"

void SystemClock_Config(void);
void GPIO_Config(void);
void ltc4727js_config(void);

int main(void)
{
	SystemClock_Config();
	GPIO_Config();
  ltc4727js_config();

	uint8_t i;
	uint32_t segF [4] = {LL_GPIO_PIN_15 | LL_GPIO_PIN_2 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14,
	LL_GPIO_PIN_10 | LL_GPIO_PIN_11,
	LL_GPIO_PIN_2 | LL_GPIO_PIN_11 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15,
	LL_GPIO_PIN_2 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13  | LL_GPIO_PIN_14 | LL_GPIO_PIN_15}; //FIRE

	uint32_t digit [4] ={LL_GPIO_PIN_0, LL_GPIO_PIN_1, LL_GPIO_PIN_2, LL_GPIO_PIN_3};

	while (1){
  int button_pressed = LL_GPIO_IsInputPinSet(GPIOA, LL_GPIO_PIN_8);
   
	if (button_pressed == 0) {
		  LL_GPIO_SetOutputPin(GPIOB, LL_GPIO_PIN_6);
	   for (i= 0; i < 4; ++i){
		    LL_GPIO_ResetOutputPin (GPIOC, LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3); //Write 0 to GPIOC port LL_GPIO_ResetOutputPin (GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL GPIO_PIN_11 | LL_GPIO_PIN_12 |
		    LL_GPIO_ResetOutputPin (GPIOB, LL_GPIO_PIN_2 | LL_GPIO_PIN_10 | LL_GPIO_PIN_11 | LL_GPIO_PIN_12 | LL_GPIO_PIN_13 | LL_GPIO_PIN_14 | LL_GPIO_PIN_15); //Reset all segment
		    LL_GPIO_SetOutputPin (GPIOB, segF[i]);
		    LL_GPIO_SetOutputPin (GPIOC, digit[i]);
		    LL_mDelay(0); //USE for DEBUG increase delay to see what's happenning when 7-seg is lit
		  }
	  }else{
			LL_GPIO_ResetOutputPin(GPIOB, LL_GPIO_PIN_6);
		}
  }
}

 void GPIO_Config(void){
	 
	  // Declare struct for GPIO
    LL_GPIO_InitTypeDef GPIO_InitStruct;

	  // Enable GPIOB clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);
	 
	 // Enable GPIOB clock
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOA);
	 
	  // Config GPIOB using struct
    GPIO_InitStruct.Mode = LL_GPIO_MODE_OUTPUT;
    GPIO_InitStruct.Pin = LL_GPIO_PIN_4 | LL_GPIO_PIN_6 ;
    GPIO_InitStruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    GPIO_InitStruct.Pull = LL_GPIO_PULL_NO;
    GPIO_InitStruct.Speed = LL_GPIO_SPEED_FREQ_HIGH;
    LL_GPIO_Init(GPIOB, &GPIO_InitStruct);
	 
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

	ltc4727_initstruct. Pin = LL_GPIO_PIN_0 | LL_GPIO_PIN_1 | LL_GPIO_PIN_2 | LL_GPIO_PIN_3; //Enable PC
	LL_GPIO_Init(GPIOC, &ltc4727_initstruct);
	
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
