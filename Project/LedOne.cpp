#include "LedOne.hpp"
#include <main.h>
#include <tx_port.h>
#include <tx_api.h>
#include <Gpio.hpp>
#include "NRS-FC-BoardDefinitions.hpp"

void LedOne(ULONG input)
{
    namespace NRS = NotRocketScienceFlightComputer;
    GPIO_InitTypeDef config = NRS::outputConfig;
    HAM::Gpio pin = HAM::Gpio{NRS::Servo1, config};
    while (1)
    {
      pin.Toggle();
      // HAL_GPIO_TogglePin(GPIOE, GPIO_PIN_11);
      tx_thread_sleep(10);
      /* code */
    }
}