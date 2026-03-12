#ifndef SENSOR_API_H
#define SENSOR_API_H
#include <stdint.h>
#include "sensor_hal.h"


uint8_t os_sensor_init(SensorObject *self);
uint8_t os_sensor_read(SensorObject *self);
uint8_t os_sensor_trigger(SensorObject *self);
uint8_t os_sensor_sleep(SensorObject *self);


#endif