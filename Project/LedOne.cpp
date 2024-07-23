#include "LedOne.hpp"
#include "cmsis_os.h"
#include <Gpio.hpp>
#include "NRS-FC-BoardDefinitions.hpp"
#include "tim.h"

// void LedOneEntry(void* argument)
// {
//     namespace NRS = NotRocketScienceFlightComputer;
//     GPIO_InitTypeDef config = NRS::outputConfig;
//     HAM::Gpio pin = HAM::Gpio{NRS::Servo1, NRS::outputConfig};
//     while (1)
//     {
//       pin.Toggle();
//       // HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
//       osDelay(200);
//       /* code */
//     }
// }

//TIM_HandleTypeDef htim1;

//void SystemClock_Config(void);
//static void MX_GPIO_Init(void);
//static void MX_TIM1_Init(void);


void LedOneEntry(void* argument)
{
  int32_t CH1_DC=0;

  //HAL_Init();
  //SystemClock_Config();
  //MX_GPIO_Init();
  //MX_TIM1_Init();
  HAL_TIM_PWM_Start(&htim1, TIM_CHANNEL_2);

  while (1)
  {
    while (CH1_DC<65535)
    {
      TIM1->CCR2 = CH1_DC;
      CH1_DC += 70;
      osDelay(10);
    }
    while (CH1_DC > 0)
    {
      TIM1->CCR2 = CH1_DC;
      CH1_DC -= 70;
      osDelay(10);
    }
    
    
  }
  

}