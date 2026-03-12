#include "type_define.h"
#include <stdio.h>
#include <stdint.h>

// ---------------------------------------------------------
// UNIVERSAL FUNCTIONS
// ---------------------------------------------------------

uint8_t os_sensor_init(void *generic_sensor_ptr) {
    if (generic_sensor_ptr == NULL) return 0; // Fail: Bad pointer

    BasicSensor *current = (BasicSensor *)generic_sensor_ptr;

    if (current->basic_ops.init != NULL) {
        if (current->basic_ops.init(&current->basic_meta) == SENSOR_OK) {
            return 1; 
        }
    }
    return 0;
}

uint8_t os_sensor_read(void *generic_sensor_ptr) {
    if (generic_sensor_ptr == NULL) return 0; // Fail: Bad pointer

    BasicSensor *current = (BasicSensor *)generic_sensor_ptr;

    if (current->basic_ops.read != NULL) {
        sensor_status_t status = current->basic_ops.read(&current->basic_meta, &current->basic_packet);
        
        if (status == SENSOR_OK) {
            // It worked! Print the data based on type
            if (current->basic_meta.type == SENSOR_TYPE_SCALAR) {
                printf("[Addr: 0x%02X] Value: %.3f\n", 
                        current->basic_meta.device_address, 
                        current->basic_packet.data.scalar);
            }
            return 1; // Success!
        }
    }
    return 0; // Fail: I2C error, disconnected wire, or missing function
}

// ---------------------------------------------------------
// ADVANCED FUNCTIONS
// ---------------------------------------------------------

uint8_t os_sensor_trigger(void *generic_sensor_ptr) {
    if (generic_sensor_ptr == NULL) return 0; 

    BasicSensor *base_ptr = (BasicSensor *)generic_sensor_ptr;

    // Gatekeeper: If it's a Basic Sensor, it CANNOT trigger. 
    if (base_ptr->basic_meta.class_id == CLASS_BASIC_SENSOR) {
        return 0; // Fail: Property not supported!
    }

    AdvanceSensor *adv_ptr = (AdvanceSensor *)generic_sensor_ptr;

    // Safely fire the trigger
    if (adv_ptr->extra_ops.trigger != NULL) {
        if (adv_ptr->extra_ops.trigger(&adv_ptr->base.basic_meta) == SENSOR_OK) {
            return 1; // Success!
        }
    }
    return 0; // Fail: Hardware error or missing function
}

uint8_t os_sensor_sleep(void *generic_sensor_ptr) {
    if (generic_sensor_ptr == NULL) return 0;

    BasicSensor *base_ptr = (BasicSensor *)generic_sensor_ptr;

    // Gatekeeper: If it's a Basic Sensor, it CANNOT sleep.
    if (base_ptr->basic_meta.class_id == CLASS_BASIC_SENSOR) {
        return 0; // Fail: Property not supported!
    }

    AdvanceSensor *adv_ptr = (AdvanceSensor *)generic_sensor_ptr;

    // Safely put hardware to sleep
    if (adv_ptr->extra_ops.sleep != NULL) {
        if (adv_ptr->extra_ops.sleep(&adv_ptr->base.basic_meta) == SENSOR_OK) {
            return 1; // Success!
        }
    }
    return 0; // Fail: Hardware error or missing function
}