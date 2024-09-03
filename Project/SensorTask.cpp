#include "SensorTask.hpp"
#define DEBUG 1
#include <I2C.hpp>
#include <BMI088/Accelerometer.hpp>
#include <Barometer.hpp>
#include <BMI088/Temperature.hpp>
#include <queue>
#include <Gpio.hpp>
#include <Printer.hpp>
#include <string>
#include <optional>
#include "cmsis_os.h"
#include "NRS-FC-BoardDefinitions.hpp"


namespace NRS = NotRocketScienceFlightComputer;
static HAM::byte a[] = {0x7D, 0x04};

static std::queue<ISensorAsync*> i2cRequestQueue;
static HAM::I2C i2c{NRS::I2CDefinition};
static HAM::Uart uart{NRS::USBUartDefinition};
static HAM::Printer printer {uart};

void initiateNextReading()
{
    // printer << "New reading initiated\n";
    //Handle I2C queue
    if(!i2c.Busy() && !i2cRequestQueue.empty())
    {
        i2cRequestQueue.front()->startReading();
        i2cRequestQueue.pop();
    }
}

HAM::Barometer barometer;
HAM::BMI088::Accelerometer accelerometer{i2c, 0x18, NRS::AccelerometerConfig, [](){
    //New reading available here
    //initiateNextReading();
}};
static HAM::BMI088::Temperature imuTemperature{i2c, NRS::BMIAccelerometerAddress, [](){}};

/**
 * @brief Callback function for GPIO EXTI interrupts.
 * 
 * This function is called when an EXTI interrupt is triggered on a GPIO pin.
 * It handles the specific actions to be taken based on the pin that triggered the interrupt.
 * 
 * @param pin The pin number that triggered the interrupt.
 */
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if(pin == Barometer_Interrupt_Pin)
    {
       // barometer.getPressure();
    }
    else if (pin == Accelerometer_Interrupt_1_Pin)
    {
        i2cRequestQueue.push(&accelerometer);
    }
}
void HAL_I2C_MemTxCpltCallback(I2C_HandleTypeDef* handle)
{
    // HAL_I2C_Mem_Write_DMA(&hi2c1, 0x08 << 1,0x01, 1, a, 1);
}
 void HAL_I2C_MasterTxCpltCallback(I2C_HandleTypeDef* handle)
{
    //HAL_I2C_Master_Transmit_DMA(&hi2c1, 0x18 << 1, a, 2);
    // if (handle == i2c.GetHandle())
    // {
      //  i2c.OnWriteAsyncComplete();
    // }
}
void HAL_I2C_MasterRxCpltCallback(I2C_HandleTypeDef* handle)
{
    // printer << "Read complete\n";
    // osDelay(1000);

    if (handle == i2c.GetHandle())
    {
        // i2c.OnReadAsyncComplete();
    }
}



void SensorTask(void* argument)
{
    unsigned long long lastTemperatureUpdate {0};
    printer % HAM::PrintType::Log << "Sensor task booting up\n";
    GPIO_InitTypeDef config = NRS::outputConfig;
    HAM::Gpio pin = HAM::Gpio{NRS::Servo1, NRS::outputConfig};
    pin.Set(1);
    if (&NRS::I2CDefinition == i2c.GetHandle())
    {
        printer << "Same handle\n";
    }
    // printer <<"Barometer begin: " << barometer.begin() << "\n";
    // if (barometer.begin() == 0)
    // {
    //     printer << "Barometer initiated\n";
    // }
    // else
    // {
    //     printer << "Barometer failed to initiate\n";
    // }
    // i2c.WriteAsync(0x18 << 1, a, 2);
    accelerometer.softReset();
    osDelay(100);
    accelerometer.initiate();
    osDelay(100);

    accelerometer.setRange(HAM::BMI088::AccelerometerRegisters::ACC_RANGE::Range::g3);
    osDelay(100);

    accelerometer.setOutputDataRate(HAM::BMI088::AccelerometerRegisters::ACC_CONF::OutputDataRate::Hz25);
    int counter = 0;
    while (1)
    {       
        printer << "Counter: " << counter++ << "\n";
        std::optional<float> measurement = barometer.getPressure();
        if(measurement.has_value())
        {
            printer << "Pressure: "<< int(measurement.value()) << "\n";
        }
        accelerometer.BlockingRead();
        printer << "X: " << int(accelerometer.X()) << "\n";
        printer << "Y: " << int(accelerometer.Y()) << "\n";
        printer << "Z: " << int(accelerometer.Z()) << "\n";
        osDelay(1000);
    }
    
}