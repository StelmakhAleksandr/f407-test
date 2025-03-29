Import("env")

def generate_pins():
    ports = ['A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J', 'K']
    pins = range(16)
    with open('src/stmcmp/periph/Pins.h', 'w') as file:
        file.write('#pragma once \n\n#include "Gpio.h"\n\n')
        for i, port in enumerate(ports):
            for pin in pins:
                file.write(f'#define P{port}{pin} GpioBuilder(GPIO{port}_BASE, {pin})\n')
        file.write('\n\n')

generate_pins()