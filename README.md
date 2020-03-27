# MCP4141 based digital potentiometer

> MCP4141 and ATtiny13 based digital potentiometer to replace an
> existing 3 terminal analog potentiometers.

![Breadboard View](https://github.com/dilshan/mcp4141-digital-pot/raw/master/resources/digipot_breadboard.jpg)

The main objective of this project is to create a prototype version of a digital potentiometer using *Microchip*'s MCP4141 IC. MCP4141 is available with end-to-end resistances of 5KΩ, 10KΩ, 50kΩ, and 100KΩ. This potentiometer-module can drive MCP4141 with any of the above mention resistances.

To drive the MCP4141, this module use ATtiny13 MCU. This MCU control MCP4141's resistance, based on the rotary-encoder events. ATtiny13 is an 8-pin, low power 8-bit MCU with an internal oscillator. The key reason to pick this MCU is its availability and lower price. Due to a lack of hardware-based SPI, this system use bit-banging SPI implementation to drive the MCP4141 IC. 

![Schematic](https://raw.githubusercontent.com/dilshan/mcp4141-digital-pot/master/resources/schematic.svg?sanitize=true)

To reduce the board size, this module employs only 3 components. Which including ATtiny13, MCP4141, and rotary-encoder. The dimension of the PCB design given for this module is 26.16mm x 29.72mm. 

## Building and flash firmware

Firmware of this module is developed using AVR-GCC toolchain. To build the firmware execute the `make` command.

To flash the compiled firmware, use the following set of commands:

    make fuse
    make flash

The `make fuse` command can use to set up the fuse-bits of the MCU and it needs to execute only once.

## License

This is an open-source hardware project. Compiled firmware, PCB designs, schematic, and source codes related to this project are available at this repository. 

The firmware of this project is licensed under the terms of [MIT license](https://github.com/dilshan/mcp4141-digital-pot/blob/master/LICENSE). Hardware design and related content are licensed under [Creative Commons - Attribution 4.0 International](https://creativecommons.org/licenses/by/4.0/) ([CC BY 4.0](https://creativecommons.org/licenses/by/4.0/)) license.
