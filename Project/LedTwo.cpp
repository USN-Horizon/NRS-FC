#include "LedTwo.hpp"
#include <main.h>
#include <tx_port.h>
#include <tx_api.h>
#include <Gpio.hpp>
#include "NRS-FC-BoardDefinitions.hpp"
void LedTwo(ULONG input)
{
    namespace NRS = NotRocketScienceFlightComputer;
    GPIO_InitTypeDef config = NRS::outputConfig;
    HAM::Gpio pin = HAM::Gpio{NRS::Servo2, config};
    // HAM::Gpio pin = HAM::Gpio{GPIOE, GPIO_PIN_9};

    while (1)
    {
      pin.Toggle();
      tx_thread_sleep(10);
      /* code */
    }
}