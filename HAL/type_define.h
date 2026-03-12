#ifndef TYPE_DEFINE_H
#define TYPE_DEFINE_H

#include "sensor_hal.h"

typedef struct {
    SensorDeviceMeta basic_meta;
    SensorDeviceOpsBase basic_ops;
    SensorDevicePacket basic_packet;
} BasicSensor;

typedef struct {
    BasicSensor base;
    SensorDeviceOps_plus extra_ops;
} AdvanceSensor;


#endif