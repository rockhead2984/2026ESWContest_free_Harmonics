#ifndef VL53L0X_H
#define VL53L0X_H

#include "stm32f4xx_hal.h"

#define VL53L0X_ADDR 0x29

uint8_t VL53L0X_Init(I2C_HandleTypeDef *hi2c);
uint16_t VL53L0X_ReadDistance(I2C_HandleTypeDef *hi2c);

#endif
