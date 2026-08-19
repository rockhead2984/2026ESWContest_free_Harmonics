#ifndef INA219_H
#define INA219_H

#include "stm32f4xx_hal.h"

#define INA219_ADDR_L 0x40
#define INA219_ADDR_R 0x41

#define INA219_REG_CONFIG  0x00
#define INA219_REG_CURRENT 0x04
#define INA219_REG_POWER   0x03

uint8_t INA219_Init(I2C_HandleTypeDef *hi2c, uint8_t addr);
float INA219_ReadCurrent(I2C_HandleTypeDef *hi2c, uint8_t addr);

#endif
