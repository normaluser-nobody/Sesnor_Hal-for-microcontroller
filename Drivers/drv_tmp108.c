#include "i2c_hal.h"
#include "sensor_hal.h"
#include <stddef.h>
#include <stdio.h>

sensor_status_t tmp108_init(SensorDeviceMeta *self) {
    return SENSOR_OK;
}

sensor_status_t tmp108_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
    uint8_t raw_data[2] = {0};
    i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

    if (my_bus->read_reg(my_bus->bus_context, self->device_address, 0x00, raw_data, 2) != I2C_OK) {
        return SENSOR_FAIL;
    }

    // 1. Join bytes into 16-bit integer
    int16_t raw_combined = (raw_data[0] << 8) | raw_data[1];

    
    float temp_c = (float)raw_combined / 256.0f; 

    out->data.scalar = temp_c;
    return SENSOR_OK;
}

static const SensorVTable TMP108_VTable = {
    .init    = tmp108_init,
    .read    = tmp108_read,
    .trigger = NULL, // TMP108 doesn't need a trigger
    .sleep   = NULL  // Or add a sleep function if you want!
};

void tmp108_set(SensorObject *self, i2c_bus_t *bus, uint8_t device_address) {
    self->sensor_meta.device_address = device_address;
    self->sensor_meta.type = SENSOR_TYPE_SCALAR;
    self->sensor_meta.io_interface = bus; 

    self->sensor_meta.channel = 0;                   
    self->vtable=&TMP108_VTable;
}