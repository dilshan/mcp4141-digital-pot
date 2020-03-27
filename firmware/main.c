// -------------------------------------------------------------------------------
// MIT License
//
// Copyright (c) 2020 Dilshan R Jayakody
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// -------------------------------------------------------------------------------

#include <avr/io.h>
#include <util/delay.h>

#include "main.h"

// ---------------------------------------------------
// - ATTiny 13 Pin     | Connection                  -
// ---------------------------------------------------
// - Pin 1 (RESET)     | +5V (VCC)                   -
// - Pin 2 (IN)        | Rotary Encorder Terminal A  -
// - Pin 3 (IN)        | Rotary Encorder Terminal B  -
// - Pin 4 (GND)       | GND                         -
// - Pin 5 (MOSI)      | MCP4141 SDI/SDO             -
// - Pin 6 (CS)        | MCP4141 CS                  -
// - Pin 7 (SCK)       | MCP4141 SCK                 -
// - Pin 8 (VCC)       | +5V (VCC)                   -
// ---------------------------------------------------

int main()
{
    // Initialize MCU ports and associated registers.
    DDRB = 0x07;
    PORTB = 0xFF;
    
    unsigned char state, lastState;
    unsigned char secState, potLevel;

    // Load last potentiometer position from EEPROM. 
    _delay_ms(5);
    potLevel = loadPotValue();
    updatePot(potLevel);
    _delay_ms(10);
    updatePot(potLevel);

    // Get status of the rotary encorder.
    lastState = (PINB & 0x08) ? 1 : 0;
    state = lastState;

    while(1)
    {
        state = (PINB & 0x08) ? 1 : 0;        

        // Check for rotary encorder events.
        if(lastState != state)
        {
            secState = (PINB & 0x10) ? 1 : 0;
            if(secState != state)
            {
                // Rotary encorder is in increment direction.
                if(potLevel < 127)
                {
                    potLevel++;
                }
            }
            else 
            {
                // Rotary encorder is in decrement direction.
                if(potLevel > 0)
                {
                    potLevel--;
                }
            }

            // Update potentiometer with new value and save it in EEPROM.
            updatePot(potLevel);
            savePotValue(potLevel);
        }

        lastState = state;
    }
    
    return 0;
}

void updatePot(unsigned char value)
{
    unsigned char pos;

    // Select MCP4141 chip and set MOSI to high.
    PORTB = PORTB | 0x01;
    _delay_us(1);
    PORTB = PORTB & 0xFD;
    _delay_us(1);

    PORTB = PORTB & 0xFE;

    // Send address value as 0x00.
    for (pos = 0x80; pos; pos >>= 1)
    {
        _delay_us(1);
        PORTB = PORTB | 0x04;
        _delay_us(1);
        PORTB = PORTB & 0xFB;
    }

    PORTB = PORTB | 0x01;
    _delay_us(5);

    // Send potentiometer position data.
    for (pos = 0x80; pos; pos >>= 1)
    {
        if(value & pos)
        {
            PORTB = PORTB | 0x01;
        }
        else
        {
            PORTB = PORTB & 0xFE;
        }

        _delay_us(1);
        PORTB = PORTB | 0x04;
        _delay_us(1);
        PORTB = PORTB & 0xFB;
    }

    // Deselect MCP4141 chip and reset MOSI.
    PORTB = PORTB | 0x01;
    _delay_us(5);
    PORTB = PORTB | 0x02;
}

void savePotValue(unsigned char potVal)
{
    // Waiting for last EEPROM update.
    while(EECR & (1<<EEPE));

    // Save specified potentiometer position into address 0x00.
    EECR = (0<<EEPM1)|(0>>EEPM0);
    EEARL = 0x00;
    EEDR = potVal;

    EECR |= (1<<EEMPE);
    EECR |= (1<<EEPE);
}

unsigned char loadPotValue()
{
    unsigned char eepromVal;
    
    while(EECR & (1<<EEPE));

    // Try to load potentiometer position from address 0x00.
    EEARL = 0x00;
    EECR |= (1<<EERE);

    // Trim received value to avoid overflows.
    eepromVal = EEDR;
    return ((eepromVal > 127) ? 0 : eepromVal);
}
