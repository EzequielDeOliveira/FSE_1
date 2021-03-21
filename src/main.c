#include <stdio.h>
#include <signal.h>
#include "uart.h"
#include "led.h"
#include "bme280temperature.h"
#include "pid.h"
#include "gpio.h"

void finish();

int main(int argc, char *argv[])
{
    float TR, TI, TE, control_output;

    signal(SIGINT, finish);
    pid_configure_constants(5.0, 1.0, 5.0);

    while (1)
    {

        TR = pontentiometer_temperature();
        TI = lm35_temperature();
        TE = bme280_temperature();

        print_lcd(TR, TI, TE);

        pid_update_reference(TR);

        control_output = pid_control(TI);

        manage_gpio_devices(control_output);

        usleep(1000000);
    }

    return (0);
}

void finish(int uart)
{
    deactivate_fan_and_resistor();
    exit(0);
}