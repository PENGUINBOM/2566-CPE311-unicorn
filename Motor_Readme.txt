#include "stm32l1xx_ll_bus.h"
#include "stm32l1xx_ll_gpio.h"
#include "stm32l1xx_ll_system.h"

#define USER_BUTTON_PIN LL_GPIO_PIN_0
#define MOTOR_PIN LL_GPIO_PIN_4

void SystemClock_Config(void);
// การหน่วงเวลา Delay(100); รอจนกว่าตัวแปร nCount จะลดลงเป็นศูนย์
//  เพื่อให้การกดปุ่ม USER ไม่ถูกตรวจพบซ้ำซ้อนหลายครั้ง
void Delay(__IO uint32_t nCount)
{
    while(nCount--)
    {
    }
}

int main(void)
{
    SystemClock_Config();

    LL_GPIO_InitTypeDef gpio_initstruct;  
    LL_GPIO_InitTypeDef gpio_button_initstruct;  

    // เปิดใช้งานนาฬิกา GPIOB
    LL_AHB1_GRP1_EnableClock(LL_AHB1_GRP1_PERIPH_GPIOB);

    // การตั้งค่าพินควบคุมมอเตอร์
    gpio_initstruct.Mode = LL_GPIO_MODE_OUTPUT;
    gpio_initstruct.OutputType = LL_GPIO_OUTPUT_PUSHPULL;
    gpio_initstruct.Pull = LL_GPIO_PULL_NO;  
    gpio_initstruct.Speed = LL_GPIO_SPEED_FREQ_VERY_HIGH;
    gpio_initstruct.Pin = MOTOR_PIN;
    LL_GPIO_Init(GPIOB, &gpio_initstruct);

    // การตั้งค่าปุ่มผู้ใช้
    gpio_button_initstruct.Mode = LL_GPIO_MODE_INPUT;
    gpio_button_initstruct.Pull = LL_GPIO_PULL_NO;
    gpio_button_initstruct.Pin = USER_BUTTON_PIN;

    LL_GPIO_Init(GPIOB, &gpio_button_initstruct);

    uint8_t motor_running = 0;  // เพิ่มตัวแปรเพื่อติดตามสถานะการทำงานของมอเตอร์

    while (1)
    {
        // ตรวจสอบสถานะของปุ่มผู้ใช้
        if (LL_GPIO_IsInputPinSet(GPIOB, USER_BUTTON_PIN))
        {
            // เมื่อกดปุ่ม
            if (!motor_running) {
               // มอเตอร์ไม่ทำงานจะสั่งให้ทำงาน
                LL_GPIO_SetOutputPin(GPIOB, MOTOR_PIN);
                motor_running = 1;  // ปรับค่าตัวแปรเพื่อระบุว่ามอเตอร์กำลังทำงาน
            } else {
                // หากมอเตอร์กำลังทำงานอยู่แล้วให้หยุดการทำงาน
                LL_GPIO_ResetOutputPin(GPIOB, MOTOR_PIN);
                motor_running = 0;  // ปรับค่าตัวแปรเพื่อระบุว่ามอเตอร์หยุดทำงาน
            }

            // เพิ่มความล่าช้าในการกดปุ่ม(ปุ่มไม่ดี)
            Delay(100);
        }
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

