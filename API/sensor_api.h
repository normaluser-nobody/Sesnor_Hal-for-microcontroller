#ifndef SENSOR_API_H
#define SENSOR_API_H

#include "type_define.h"

// UNIVERSAL FUNCTIONS

uint8_t os_sensor_init(void *generic_sensor_ptr);
uint8_t os_sensor_read(void *generic_sensor_ptr);

// ADVANCE FUNCTIONS

uint8_t os_sensor_trigger(void *generic_sensor_ptr);
uint8_t os_sensor_sleep(void *generic_sensor_ptr);


#endif