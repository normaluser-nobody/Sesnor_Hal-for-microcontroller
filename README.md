# Hardware-Agnostic Sensor HAL

A bare-metal C architecture built to seamlessly decouple application logic from physical hardware, ensuring effortless sensor replacement and strict code modularity. 

## 🧠 Architecture & Design Philosophy

This project solves those issues through:
1. **True C Polymorphism (vTables):** Every sensor is represented by a generic interface. Function routing is handled via Virtual Tables (vTables) locked in read-only Flash memory (`.rodata`). This reduces the RAM overhead to a single 4-byte pointer per sensor object.
2. **Static Buffer Injection:** To handle wildly different data types (like simple floats for temperature vs. complex 3D vectors for acceleration) without monolithic structs, the architecture uses `void *` payloads. The application layer statically allocates the memory buffers and injects them down into the driver, completely avoiding the risks of `malloc()` and the Singleton trap.
3. **Safe API Gatekeeping:** The global API layer intercepts all function calls to prevent fatal `NULL` pointer hardware faults (e.g., if a sensor does not support a `sleep()` function).

## 📂 Repository Guide: Where to Look

This repository contains standard STM32CubeMX boilerplate alongside the custom HAL architecture. Reviewers and engineering managers should focus on the following core files:

* **`sensor_hal.h`** * *The Core Interface:* Defines the polymorphic `SensorObject` struct and the function pointers that make up the vTable interface.
* **`sensor_api.c` & `sensor_api.h`** * *The Global API:* The strict boundary layer that the application loop uses to communicate with sensors safely. It prevents hard faults by checking vTable pointers before execution.
* **`drv_adxl345.c` & `drv_tmp108.c`** * *The Hardware Drivers:* Contains the hardware-specific logic. Notice how the vTables here are declared as `const` to force them into Flash memory (`.rodata`).
* **`i2c_hal.h` & `stm32_i2c_adapter.c`**
  * *The Hardware Bridge:* Maps the generic HAL I2C calls directly to the vendor-specific STM32 HAL functions.
* **`main.c`** * *The Application Loop:* While this file contains standard STM32 clock/peripheral setup boilerplate, look at the `main()` execution loop. Notice how it seamlessly manages an array of completely different sensors using a single, unified function call (`os_sensor_read()`), remaining entirely blind to the underlying silicon.

## 🚀 Key Engineering Benefits

* **Zero-Friction Hardware Swapping:** Physical I2C sensors can be swapped or upgraded without altering a single line of the core application logic.
* **Deterministic Execution:** Avoids pointer-chasing and bloated language features, respecting strict microcontroller time constraints.
* **Absolute Memory Safety:** 100% static memory allocation. No dynamic memory, preventing heap fragmentation and memory leaks in long-running embedded systems.
* **Unified Multi-Sensor Management:** The exact same Global API seamlessly controls completely
different categories of sensors simultaneously. A single application loop can call
os_sensor_read() on a scalar temperature sensor and a 3-axis accelerometer right next
to each other. The vTable dynamically routes the execution to the correct driver and handles
the distinct data types automatically.

## 🛠️ Toolchain & Validation

* **Language:** C (Bare-metal)
* **Target Hardware:** Generic Microcontrollers
* **Validation:** Verified hardware-independence and error propagation using the **Renode** emulator for software-in-the-loop testing.
