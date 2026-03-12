#include "sensor_hal.h"
#include <stdio.h>
#include <stdint.h>

uint8_t os_sensor_init(SensorObject *self) {
    if (self == NULL) return 0; 

    if (self->vtable->init != NULL) {
        if (self->vtable->init(&self->sensor_meta) == SENSOR_OK) {
            return 1; 
        }
    }
    return 0;
}

uint8_t os_sensor_read(SensorObject *self) {
    if (self == NULL || self->vtable == NULL) return 0;

    if (self->vtable->read != NULL) {
        sensor_status_t status = self->vtable->read(&self->sensor_meta, &self->sensor_packet);
        
        if (status == SENSOR_OK) {
            // Check type and print accordingly
            if (self->sensor_meta.type == SENSOR_TYPE_SCALAR) {
                printf("[Addr: 0x%02X] Scalar Value: %.3f\n", 
                        self->sensor_meta.device_address, 
                        self->sensor_packet.data.scalar);
            }
            else if (self->sensor_meta.type == SENSOR_TYPE_VEC3) {
                printf("[Addr: 0x%02X] Vec3 X: %.2f, Y: %.2f, Z: %.2f\n", 
                        self->sensor_meta.device_address,
                        self->sensor_packet.data.vec3.x, 
                        self->sensor_packet.data.vec3.y, 
                        self->sensor_packet.data.vec3.z);
            }
            return 1; // Moved OUTSIDE the type-check blocks so it returns 1 for both!
        }
    }
    return 0; 
}

uint8_t os_sensor_trigger(SensorObject *self) {
    if (self == NULL) return 0; 

    if (self->vtable->trigger != NULL) {
        if (self->vtable->trigger(&self->sensor_meta) == SENSOR_OK) {
            return 1; 
        }
    }
    return 0; 
}

uint8_t os_sensor_sleep(SensorObject *self) {
    if (self == NULL) return 0;

    if (self->vtable->sleep != NULL) {
        if (self->vtable->sleep(&self->sensor_meta) == SENSOR_OK) {
            return 1; 
        }
    }
    return 0; 
}