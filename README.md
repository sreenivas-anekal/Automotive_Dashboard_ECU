# Automotive Instrument Cluster ECU

A modular Embedded C simulation of an automotive instrument-cluster ECU, implementing sensor processing, vehicle-state management, diagnostic logging, persistent storage, task scheduling, and communication interfaces.

## Overview

This project implements a modular automotive instrument-cluster ECU simulation using Embedded C.

The system models core ECU software responsibilities, including:

- ADC-based sensor processing
- Vehicle state-machine management
- UART-based diagnostic logging
- EEPROM-based data storage
- Periodic task scheduling
- CAN communication interface
- Module-level testing
- ECU-level integration testing

The project uses a host-based simulation environment for module and integration testing, along with STM32/Wokwi-based implementations for embedded-oriented validation.

## System Architecture

```text
                         AUTOMOTIVE ECU
                              |
              +---------------+---------------+
              |               |               |
        Sensor Processing  Vehicle FSM   Task Scheduler
              |               |               |
              +---------------+---------------+
                              |
              +---------------+---------------+
              |               |               |
             UART           EEPROM            CAN
         Diagnostics       Storage       Communication
```

## ECU Data Flow

```text
Simulated Sensor Input
          |
          v
     ADC Processing
          |
          v
   Sensor Value Conversion
          |
          v
    Vehicle ECU Runtime
          |
    +-----+-----+----------------+
    |           |                |
    v           v                v
Scheduler     Vehicle FSM     Diagnostics
    |           |                |
    +-----------+----------------+
                |
        +-------+-------+
        |               |
        v               v
      EEPROM           CAN
      Storage      Communication
```

## Features

### Sensor Processing

The ADC sensor module converts a raw ADC value into a corresponding voltage and vehicle parameter representation.

The current implementation includes fuel-level processing using a 12-bit ADC model with a 3.3 V reference.

Example:

```text
ADC = 2048
      |
      v
Voltage ≈ 1.65 V
      |
      v
Fuel ≈ 50%
```

### Vehicle State Machine

The ECU implements a finite state machine for vehicle operating states.

```text
OFF
 |
 | IGNITION_ON
 v
IGNITION
 |
 | SELF_TEST_OK
 v
IDLE
 |
 | ENGINE_START
 v
RUNNING
```

Fault handling and shutdown transitions are also represented in the state machine.

### UART Diagnostics

A lightweight diagnostic logging interface provides categorized runtime messages:

```text
INFO
WARN
ERROR
```

Messages are tagged by subsystem to make ECU runtime behavior easier to trace during simulation and debugging.

### EEPROM Storage

The EEPROM module provides simulated non-volatile storage using an in-memory representation.

The integration flow stores the final calculated fuel value to a specified EEPROM address.

### Task Scheduler

A lightweight periodic task scheduler is implemented to model time-triggered ECU activities.

The integration environment currently demonstrates tasks running at different periodic intervals:

```text
Sensor Task          100 ms
Communication Task   250 ms
```

### CAN Communication

The project includes a CAN message abstraction supporting:

- Standard CAN identifiers
- Data length validation
- Up to 8 bytes of payload data
- Message construction and transmission through the simulation layer

The current implementation represents the communication interface at the software/simulation level and does not claim to implement a physical CAN transceiver or STM32 CAN peripheral.

## Project Structure

```text
Automotive_Dashboard_ECU/
|
+-- app/
|   |
|   +-- inc/
|   |   +-- adc_sensor.h
|   |   +-- can_driver.h
|   |   +-- eeprom_storage.h
|   |   +-- task_scheduler.h
|   |   +-- uart_debug.h
|   |   +-- vehicle_state.h
|   |
|   +-- src/
|       +-- adc_sensor.c
|       +-- can_driver.c
|       +-- eeprom_storage.c
|       +-- task_scheduler.c
|       +-- uart_debug.c
|       +-- vehicle_state.c
|
+-- tests/
|   |
|   +-- gdb/
|       +-- adc_sensor_test.c
|       +-- can_driver_test.c
|       +-- eeprom_storage_test.c
|       +-- task_scheduler_test.c
|       +-- uart_debug_test.c
|       +-- vehicle_state_test.c
|       +-- ecu_integration.c
|
+-- wokwi/
|   +-- adc_sensor.c
|   +-- can_driver.c
|   +-- eeprom_storage.c
|   +-- task_scheduler.c
|   +-- uart_debug.c
|   +-- vehicle_state.c
|
+-- LICENSE
+-- README.md
```

## Software Architecture

The application is divided into independent modules with defined responsibilities.

| Module | Responsibility |
|---|---|
| `adc_sensor` | ADC value conversion and sensor processing |
| `vehicle_state` | Vehicle operating-state management |
| `uart_debug` | Diagnostic logging |
| `eeprom_storage` | Persistent data storage abstraction |
| `task_scheduler` | Periodic task execution |
| `can_driver` | CAN message abstraction and transmission |
| `ecu_integration` | End-to-end ECU runtime simulation |

This modular structure allows individual components to be tested independently before being exercised as part of the complete ECU flow.

## Testing

The project includes module-level tests and an ECU integration environment.

### Module Tests

Individual tests are provided for:

- ADC sensor processing
- CAN communication
- EEPROM storage
- Task scheduling
- UART diagnostics
- Vehicle state management

### ECU Integration Test

The integration environment initializes the ECU modules, executes the vehicle startup sequence, registers periodic tasks, runs a simulated ECU timeline, processes sensor data, performs communication, and stores the resulting fuel value.

The high-level flow is:

```text
Initialize ECU Modules
        |
        v
Vehicle Startup Sequence
        |
        v
OFF -> IGNITION -> IDLE -> RUNNING
        |
        v
Register Periodic Tasks
        |
        v
Run Simulated ECU Runtime
        |
        +----> Sensor Processing
        |
        +----> Communication
        |
        v
Store Final Fuel Value
        |
        v
Report ECU Runtime Results
```

## Simulation Environments

### Host-Based Simulation

The host-based environment is used for:

- Module-level testing
- ECU integration testing
- Runtime observation
- Debugging with GDB
- Verification of module interactions

### STM32 / Wokwi

The `wokwi/` directory contains embedded-oriented implementations of the individual ECU modules for STM32/Wokwi simulation.

This provides a path from host-side software validation toward microcontroller-oriented testing.

## Tools and Technologies

- Embedded C
- STM32
- Wokwi
- GDB
- GCC
- Git

## Design Approach

The project follows a modular embedded-software approach rather than implementing the ECU as a single monolithic application.

Each subsystem is isolated behind its own interface, allowing:

- Independent testing
- Easier debugging
- Clear module responsibilities
- Reusable components
- Incremental integration

The host simulation is used to validate software behavior before considering deployment on physical MCU hardware.

## Current Scope

The current project focuses on ECU software architecture, module behavior, simulation, and testing.

It does not represent a production automotive ECU and does not implement automotive safety certification, AUTOSAR, production-grade CAN hardware drivers, or hardware-specific STM32 peripheral drivers across the complete system.

## Future Hardware Implementation

A physical STM32 implementation could replace the simulation layers with hardware-specific drivers for:

- ADC peripherals
- UART peripherals
- CAN peripherals
- Timers
- GPIO
- External or internal non-volatile memory

The higher-level application architecture can then be retained while replacing the underlying hardware abstraction and peripheral interfaces.

## Project Goals

The project was developed to demonstrate practical understanding of:

- Embedded C
- Modular firmware architecture
- ADC and sensor processing
- Finite state machines
- Periodic task scheduling
- UART diagnostics
- Persistent storage
- CAN communication concepts
- Embedded software testing
- GDB-based debugging
- STM32/Wokwi simulation

## License

This project is licensed under the terms provided in the repository's `LICENSE` file.
