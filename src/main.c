#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <pthread.h>

#include "uart.h"
#include "lcd.h"
#include "bme280temperature.h"
#include "pid.h"
#include "gpio.h"
#include "csv.h"

void finish();
void main_loop();
void menu();
void define_reference();
void clearscr();

float TR = 0, TI = 0, TE = 0, control_output = 0;
int reference_potentiometer = 1;

int main(int argc, char *argv[])
{
    pthread_t tid[2];

    lcd_setup();
    gpio_setup();
    bme280_setup();
    csv_setup();

    pthread_create(&tid[0], NULL, (void *)main_loop, (void *)NULL);
    pthread_create(&tid[1], NULL, (void *)menu, (void *)NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return (0);
}

void finish()
{
    deactivate_fan_and_resistor();
    ClrLcd();
    close_bme280();
    exit(0);
}

void main_loop()
{
    float Kp = 5.0, Ki = 1.0, Kd = 5.0;

    signal(SIGINT, finish);
    pid_configure_constants(Kp, Ki, Kd);

    while (1)
    {

        if (reference_potentiometer)
            TR = pontentiometer_temperature(TR);

        TI = lm35_temperature(TI);
        TE = bme280_temperature();

        print_lcd(TR, TI, TE);

        pid_update_reference(TR);

        control_output = pid_control(TI);

        write_csv(TR, TE, TI, control_output);

        manage_gpio_devices(control_output);

        usleep(1000000);
    }
}

void menu()
{
    int choose;
    do
    {
        clearscr();
        printf("-----------------------------------------\n");
        printf("Temperatura Referência: %4.2f\n", 12.8);
        printf("Temperatura Interna: %4.2f\n", 12.8);
        printf("Temperatura Externa: %4.2f\n", 12.8);
        printf("-----------------------------------------\n");
        printf("Resistor: \n");
        printf("Ventoinha: \n");
        printf("-----------------------------------------\n");
        printf("1. Definir uma temperatura referência\n");
        printf("2. Usar temperatura do potenciômetro como referência\n");
        scanf("%d", &choose);
        if (choose == 1)
        {
            define_reference();
        }
        else if (choose == 2)
        {
            reference_potentiometer = 1;
        }
    } while (1);
}

void define_reference()
{
    float reference;
    clearscr();
    printf("Temperatura referência: ");
    scanf("%f", &reference);
    pid_update_reference(reference);
    reference_potentiometer = 0;
    clearscr();
}

void clearscr(void)
{
    for (int i = 0; i < 50; i++)
        printf("\n");
}