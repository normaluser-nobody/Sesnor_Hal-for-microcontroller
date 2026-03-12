#include "type_define.h"
#include "i2c_hal.h"
#include "sensor_hal.h"
#include <stddef.h>
#include <stdio.h>

sensor_status_t tmp108_init(SensorDeviceMeta *self) {
    return SENSOR_OK;
}

// sensor_status_t tmp108_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
//     uint8_t raw_data[2] = {0};

//     i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

//     if (my_bus->read_reg(my_bus->bus_context, self->device_address, 0x00, raw_data, 2) != I2C_OK) {
//         return SENSOR_FAIL;
//     }

//     return SENSOR_OK;
// }
// sensor_status_t tmp108_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
//     uint8_t raw_data[2] = {0};
//     i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

//     // 1. Read 2 bytes from Pointer Register 0x00 (Temperature Register)
//     // The TMP108 returns MSB first, then LSB.
//     if (my_bus->read_reg(my_bus->bus_context, self->device_address, 0x00, raw_data, 2) != I2C_OK) {
//         return SENSOR_FAIL;
//     }

//     // 2. Combine the two bytes into a 16-bit signed integer
//     // The TMP108 stores the 12-bit value in the upper 12 bits of the 16-bit word.
//     int16_t combined = (raw_data[0] << 8) | raw_data[1];

//     // 3. Shift right by 4 to get the actual 12-bit count
//     // Because 'combined' is a signed int16_t, the >> operator will 
//     // perform an arithmetic shift, preserving the negative sign bit!
//     int16_t digital_val = combined >> 4;

//     // 4. Convert to Celsius
//     // 1 LSB = 0.0625 degrees C
//     float temp_c = (float)digital_val * 0.0625f;
//     // 5. Output to your framework
//     out->data.scalar = temp_c;
    
//     return SENSOR_OK;
// }


sensor_status_t tmp108_read(SensorDeviceMeta *self, SensorDevicePacket *out) {
    uint8_t raw_data[2] = {0};
    i2c_bus_t *my_bus = (i2c_bus_t *)self->io_interface;

    if (my_bus->read_reg(my_bus->bus_context, self->device_address, 0x00, raw_data, 2) != I2C_OK) {
        return SENSOR_FAIL;
    }

    // 1. Join bytes into 16-bit integer
    int16_t raw_combined = (raw_data[0] << 8) | raw_data[1];

    // 2. DO NOT shift yet! Convert the whole 16-bit value to a float first.
    // The TMP108 is left-justified, so the 12-bit value is (Value * 16).
    // To get the real value, we divide the float by 16.0 and then multiply by 0.0625.
    // Or simpler: divide by 256.0 total.
    
    float temp_c = (float)raw_combined / 256.0f; 

    out->data.scalar = temp_c;
    return SENSOR_OK;
}
void tmp108_set(BasicSensor *self, i2c_bus_t *bus, uint8_t device_address) {
    self->basic_meta.device_address = device_address;
    self->basic_meta.type = SENSOR_TYPE_SCALAR;
    self->basic_meta.io_interface = bus; 

    self->basic_meta.class_id = CLASS_BASIC_SENSOR; 
    self->basic_meta.channel = 0;                   
    self->basic_ops.init = tmp108_init;
    self->basic_ops.read = tmp108_read;
}