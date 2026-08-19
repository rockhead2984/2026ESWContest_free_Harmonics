#include "vl53l0x_i2c_platform.h"
#include "vl53l0x_platform.h"
#include "main.h"

extern I2C_HandleTypeDef hi2c1;

int32_t VL53L0X_write_multi(uint8_t address, uint8_t index, uint8_t *pdata, int32_t count)
{
    HAL_I2C_Mem_Write(&hi2c1, address, index, I2C_MEMADD_SIZE_8BIT, pdata, count, 100);
    return 0;
}

int32_t VL53L0X_read_multi(uint8_t address, uint8_t index, uint8_t *pdata, int32_t count)
{
    HAL_I2C_Mem_Read(&hi2c1, address, index, I2C_MEMADD_SIZE_8BIT, pdata, count, 100);
    return 0;
}

VL53L0X_Error VL53L0X_WrByte(VL53L0X_DEV Dev, uint8_t index, uint8_t data)
{
    VL53L0X_write_multi(Dev->I2cDevAddr, index, &data, 1);
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_WrWord(VL53L0X_DEV Dev, uint8_t index, uint16_t data)
{
    uint8_t buf[2];
    buf[0] = (data >> 8) & 0xFF;
    buf[1] = data & 0xFF;
    VL53L0X_write_multi(Dev->I2cDevAddr, index, buf, 2);
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_WrDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t data)
{
    uint8_t buf[4];
    buf[0] = (data >> 24) & 0xFF;
    buf[1] = (data >> 16) & 0xFF;
    buf[2] = (data >> 8) & 0xFF;
    buf[3] = data & 0xFF;
    VL53L0X_write_multi(Dev->I2cDevAddr, index, buf, 4);
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_RdByte(VL53L0X_DEV Dev, uint8_t index, uint8_t *data)
{
    VL53L0X_read_multi(Dev->I2cDevAddr, index, data, 1);
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_RdWord(VL53L0X_DEV Dev, uint8_t index, uint16_t *data)
{
    uint8_t buf[2];
    VL53L0X_read_multi(Dev->I2cDevAddr, index, buf, 2);
    *data = ((uint16_t)buf[0] << 8) | buf[1];
    return VL53L0X_ERROR_NONE;
}

VL53L0X_Error VL53L0X_RdDWord(VL53L0X_DEV Dev, uint8_t index, uint32_t *data)
{
    uint8_t buf[4];
    VL53L0X_read_multi(Dev->I2cDevAddr, index, buf, 4);
    *data = ((uint32_t)buf[0] << 24) | ((uint32_t)buf[1] << 16) | ((uint32_t)buf[2] << 8) | buf[3];
    return VL53L0X_ERROR_NONE;
}

int32_t VL53L0X_read_byte(uint8_t address, uint8_t index, uint8_t *data)
{
    return VL53L0X_read_multi(address, index, data, 1);
}

int32_t VL53L0X_write_byte(uint8_t address, uint8_t index, uint8_t data)
{
    return VL53L0X_write_multi(address, index, &data, 1);
}
