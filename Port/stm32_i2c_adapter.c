#include "i2c_hal.h"
#include "stm32f1xx_hal.h" // The prebuilt ST library for the Blue Pill

// 1. The READ Wrapper
i2c_status_t stm32_i2c_read(void *bus_context, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    // Step A: Unpack the backpack. We know it holds the ST Hardware Handle.
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)bus_context;

    // Step B: The Classic STM32 Quirk. 
    // ST's HAL requires the 7-bit I2C address to be shifted left by 1 bit.
    uint16_t st_address = (uint16_t)(dev_addr << 1);

    // Step C: Call the prebuilt ST function
    // I2C_MEMADD_SIZE_8BIT tells it that our registers are 1 byte long (like 0x00)
    // 100 is a timeout in milliseconds.
    HAL_StatusTypeDef status = HAL_I2C_Mem_Read(hi2c, st_address, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100);

    // Step D: Translate ST's language into YOUR Universal HAL language
    if (status == HAL_OK)      return I2C_OK;
    if (status == HAL_BUSY)    return I2C_ERR_BUSY;
    if (status == HAL_TIMEOUT) return I2C_ERR_TIMEOUT;
    
    return I2C_ERR_NACK; 
}

// 2. The WRITE Wrapper
i2c_status_t stm32_i2c_write(void *bus_context, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len) {
    
    I2C_HandleTypeDef *hi2c = (I2C_HandleTypeDef *)bus_context;
    uint16_t st_address = (uint16_t)(dev_addr << 1);

    HAL_StatusTypeDef status = HAL_I2C_Mem_Write(hi2c, st_address, reg_addr, I2C_MEMADD_SIZE_8BIT, data, len, 100);

    if (status == HAL_OK)      return I2C_OK;
    if (status == HAL_BUSY)    return I2C_ERR_BUSY;
    if (status == HAL_TIMEOUT) return I2C_ERR_TIMEOUT;
    
    return I2C_ERR_NACK;
}

// 3. The "Factory" Setup Function
// You call this once in main() to wire everything together.
i2c_bus_t stm32_create_i2c_bus(I2C_HandleTypeDef *hardware_handle) {
    i2c_bus_t my_bus;
    
    // Put the hardware handle (e.g., &hi2c1) into the backpack
    my_bus.bus_context = hardware_handle;
    
    // Connect the buttons on your universal remote to the ST functions
    my_bus.read_reg  = stm32_i2c_read;
    my_bus.write_reg = stm32_i2c_write;

    return my_bus;
}