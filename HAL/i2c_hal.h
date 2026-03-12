#ifndef I2C_HAL_H
#define I2C_HAL_H

#include <stdint.h>

typedef enum {
    I2C_OK = 0,
    I2C_ERR_NACK,      
    I2C_ERR_TIMEOUT,   
    I2C_ERR_BUSY       
} i2c_status_t;

typedef struct {
    void *bus_context; 
    i2c_status_t (*write_reg)(void *bus_context, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);
    i2c_status_t (*read_reg)(void *bus_context, uint8_t dev_addr, uint8_t reg_addr, uint8_t *data, uint16_t len);

} i2c_bus_t;

#endif