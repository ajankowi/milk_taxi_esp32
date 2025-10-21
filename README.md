# Milk Taxi ESP32

## Overview

Milk Taxi is an ESP32-based smart device designed to monitor and manage milk temperature and feeding schedules. This project combines real-time clock functionality, temperature monitoring, and an intuitive LCD menu system to help parents ensure optimal milk temperature and timing for feeding.

## Features

- **Real-Time Clock (RTC)**: DS3231 RTC module for accurate timekeeping
- **Temperature Monitoring**: ADC-based temperature sensor for milk temperature measurement
- **LCD Display**: HD44780-compatible 16x2 LCD for user interface
- **Menu Navigation**: Button-based menu system with debouncing
- **Alarm System**: Configurable alarm times for feeding schedules
- **FreeRTOS Integration**: Multi-task architecture for responsive operation

## Hardware Components

- ESP32 microcontroller
- DS3231 RTC module
- HD44780 LCD display (16x2)
- Temperature sensor (ADC-based)
- Navigation buttons (Up, Down, Select, Set)
- Power supply and supporting circuitry

## Software Architecture

The project is built using ESP-IDF framework with FreeRTOS for task management:

### Main Components

- **Menu System** (`menu.c`): Handles user interface and navigation
- **Keyboard Input** (`keyboard.h`): Manages button input with debouncing
- **RTC Interface** (`ds3231.h`): Real-time clock functionality
- **Temperature Sensor** (`temperature.h`): Temperature measurement and conversion
- **LCD Driver** (`HD44780.h`): Display control and text rendering

### Key Features

1. **Multi-task Operation**:
   - GPIO task for button handling with debouncing
   - Display task for continuous LCD updates
   - Background temperature monitoring

2. **Menu Navigation**:
   - Current Time display and setting
   - Alarm Time configuration
   - Temperature monitoring
   - Intuitive button-based navigation

3. **Temperature Management**:
   - Real-time temperature measurement
   - Display in Celsius with degree symbol
   - Optimal feeding temperature monitoring


## Usage

1. **Navigation**:
   - Use UP/DOWN buttons to navigate between menu items
   - Press SELECT to enter edit mode
   - Press SET to confirm changes

2. **Menu Options**:
   - **Current Time**: View and set current time
   - **Alarm Time**: Configure feeding alarm
   - **Temperature**: Monitor milk temperature

3. **Temperature Monitoring**:
   - Real-time temperature display
   - Automatic updates every few seconds
   - Temperature displayed in Celsius

## Configuration

The system can be configured through various parameters:
- Debounce time for button inputs
- Display refresh rate
- Temperature measurement intervals
- Alarm settings

## License

This project is open source and available under the [MIT License](LICENSE).

## Contact

For questions or support, please open an issue in the repository.