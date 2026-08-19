#include "vl53l0x.h"

uint8_t VL53L0X_Init(I2C_HandleTypeDef *hi2c)
{
    uint8_t data = 0x01;
    HAL_I2C_Mem_Write(hi2c, VL53L0X_ADDR << 1, 0x00, 1, &data, 1, 100);
    HAL_Delay(10);
    return 1;
}

uint16_t VL53L0X_ReadDistance(I2C_HandleTypeDef *hi2c)
{
    uint8_t data[2];
    uint8_t cmd = 0x01;
    HAL_I2C_Mem_Write(hi2c, VL53L0X_ADDR << 1, 0x80, 1, &cmd, 1, 100);
    HAL_Delay(30);
    HAL_I2C_Mem_Read(hi2c, VL53L0X_ADDR << 1, 0x1E, 1, data, 2, 100);
    return (uint16_t)(data[0] << 8 | data[1]);
}
