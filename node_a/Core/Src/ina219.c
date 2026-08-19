#include "ina219.h"

uint8_t INA219_Init(I2C_HandleTypeDef *hi2c, uint8_t addr)
{
    uint8_t config[2];
    uint16_t cfg = 0x399F;
    config[0] = (cfg >> 8) & 0xFF;
    config[1] = cfg & 0xFF;
    HAL_I2C_Mem_Write(hi2c, addr << 1, INA219_REG_CONFIG, 1, config, 2, 100);
    return 1;
}

float INA219_ReadCurrent(I2C_HandleTypeDef *hi2c, uint8_t addr)
{
    uint8_t data[2];
    HAL_I2C_Mem_Read(hi2c, addr << 1, INA219_REG_CURRENT, 1, data, 2, 100);
    int16_t raw = (int16_t)(data[0] << 8 | data[1]);
    return raw * 0.1f;
}
