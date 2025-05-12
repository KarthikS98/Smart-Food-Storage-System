# Smart Food Storage Monitoring System

A microcontroller-based system for monitoring and maintaining optimal storage conditions for fruits and vegetables that don't require refrigeration. The system uses temperature and humidity sensors to provide real-time monitoring and control of storage conditions.

## Features

- Real-time temperature and humidity monitoring using DHT11 sensor
- Support for multiple fruits and vegetables with preset optimal conditions
- LCD display for real-time status and instructions
- Audio feedback through MP3 module
- Automatic fan control for maintaining optimal conditions
- User-friendly interface with push-button controls

## Hardware Requirements

- STM32F103C8T6 (BluePill) Microcontroller
- DHT11 Temperature and Humidity Sensor
- 16x2 LCD Display
- 2-Channel Relay Module
- MP3 Player Module
- 5V Speaker (8ohm)
- Push Buttons (x4)
- DC Fan
- Power Supply (5V)

## Pin Configuration

### LCD Connections
- RS: PB3
- EN: PB4
- D4: PB5
- D5: PB6
- D6: PB7
- D7: PB8

### DHT11 Sensor
- Data Pin: PA0

### Push Buttons
- Vegetable Switch: PA3
- Fruit Switch: PA2
- Reset Switch: PA0
- Show Temperature/Humidity Switch: PA1

### Relay Control
- Fan Relay: PB0
- Heater Relay: PB1

### MP3 Module
- RX: PA10
- TX: PA9

## Installation

1. Clone this repository
2. Open the project in your preferred STM32 IDE
3. Configure the pins according to the pin configuration section
4. Build and flash the code to your STM32F103C8T6

## Usage

1. Power on the system
2. Use the push buttons to select between fruits and vegetables
3. Choose the specific item you want to store
4. The system will display the current conditions and ideal conditions
5. The system will automatically maintain optimal conditions using the fan

## Supported Items

### Vegetables
- Carrot
- Potato
- Tomato
- Onion
- Lettuce

### Fruits
- Apple
- Banana
- Orange
- Grapes
- Mango

## Contributing

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the [LICENSE](LICENSE) file for details.

## Authors

- Karthik S

## Acknowledgments

- Department of Electronics and Communication, RV College of Engineering
