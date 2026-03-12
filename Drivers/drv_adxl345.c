#include "i2c_hal.h"
#include "sensor_hal.h"
#include <stddef.h>
#include <stdint.h>

// ADXL345 Register Map (Standard)
#define ADXL345_DEVID          0x00  // Device ID register
#define ADXL345_POWER_CTL      0x2D  // Power-saving features control
#define ADXL345_DATA_FORMAT    0x31  // Data format control
#define ADXL345_DATAX0         0x32  // X-Axis Data 0 (Start of 6-byte block)

// 1. Initialization: ADXL345 needs to be put into "Measure" mode
sensor_status_t adxl345_init(SensorDeviceMeta *self) {
    i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;
    uint8_t data;

    // A. Verify Chip ID (ADXL345 always returns 0xE5)
    if (my_bus->read_reg(my_bus->bus_context, self->device_address, ADXL345_DEVID, &data, 1) != I2C_OK) {
        return SENSOR_FAIL;
    }
    if (data != 0xE5) return SENSOR_FAIL;

    // B. Power Control: Set bit 3 to 1 to enable Measurement Mode
    data = 0x08; 
    if (my_bus->write_reg(my_bus->bus_context, self->device_address, ADXL345_POWER_CTL, &data, 1) != I2C_OK) {
        return SENSOR_FAIL;
    }

    // C. Data Format: Set +/- 2g range (0x00)
    data = 0x00;
    if (my_bus->write_reg(my_bus->bus_context, self->device_address, ADXL345_DATA_FORMAT, &data, 1) != I2C_OK) {
        return SENSOR_FAIL;
    }

    return SENSOR_OK;
}

// 2. Read: ADXL345 gives 10-bit data (in 2g mode) across 6 bytes
// sensor_status_t adxl345_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
//     uint8_t raw[6];
//     i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

//     // Read 6 bytes starting from DATAX0
//     if (my_bus->read_reg(my_bus->bus_context, self->device_address, ADXL345_DATAX0, raw, 6) != I2C_OK) {
//         return SENSOR_FAIL;
//     }

//     /* ADXL345 Data Format (Little Endian):
//        X_LSB = raw[0], X_MSB = raw[1]
//     */
//     int16_t x_raw = (int16_t)((raw[1] << 8) | raw[0]);
//     int16_t y_raw = (int16_t)((raw[3] << 8) | raw[2]);
//     int16_t z_raw = (int16_t)((raw[5] << 8) | raw[4]);

//     // Convert to G's (In 10-bit mode +/- 2g, 1 LSB = 3.9mg = 0.0039g)
//     out->data.vec3.x = (float)x_raw * 0.0039f;
//     out->data.vec3.y = (float)y_raw * 0.0039f;
//     out->data.vec3.z = (float)z_raw * 0.0039f;

//     return SENSOR_OK;
// }
// 2. Read: ADXL345 gives 10-bit data (in 2g mode) across 6 bytes
sensor_status_t adxl345_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
    uint8_t raw[6] = {0};
    i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

    // Renode Emulator Fix: Read 1 byte at a time instead of a 6-byte burst
    for (int i = 0; i < 6; i++) {
        if (my_bus->read_reg(my_bus->bus_context, self->device_address, ADXL345_DATAX0 + i, &raw[i], 1) != I2C_OK) {
            return SENSOR_FAIL;
        }
    }

    /* ADXL345 Data Format (Little Endian):
       X_LSB = raw[0], X_MSB = raw[1]
    */
    int16_t x_raw = (int16_t)((raw[1] << 8) | raw[0]);
    int16_t y_raw = (int16_t)((raw[3] << 8) | raw[2]);
    int16_t z_raw = (int16_t)((raw[5] << 8) | raw[4]);

    // Convert to G's
    out->data.vec3.x = (float)x_raw * 0.0039f;
    out->data.vec3.y = (float)y_raw * 0.0039f;
    out->data.vec3.z = (float)z_raw * 0.0039f;

    return SENSOR_OK;
}
// 3. Updated vTable for ADXL345
static const SensorVTable ADXL345_VTable = {
    .init    = adxl345_init,
    .read    = adxl345_read,
    .trigger = NULL,
    .sleep   = NULL 
};

// 4. Set Function: Updated to match ADXL naming
void adxl345_set(SensorObject *self, i2c_bus_t *bus, uint8_t device_address) {
    self->sensor_meta.device_address = device_address;
    self->sensor_meta.type = SENSOR_TYPE_VEC3; 
    self->sensor_meta.io_interface = bus; 
    self->sensor_meta.channel = 0;                   
    self->vtable = &ADXL345_VTable;
}