#include "main.h"
#include "dashboard.h"

void finish();
void main_loop();
void menu();
void define_reference();

float TR = 0, TI = 0, TE = 0, control_output = 0;
int reference_potentiometer = 1;

int main(int argc, char *argv[])
{
    pthread_t tid[2];

    lcd_setup();
    gpio_setup();
    bme280_setup();
    csv_setup();
    init_uart();

    pthread_create(&tid[0], NULL, (void *)main_loop, (void *)NULL);
    pthread_create(&tid[1], NULL, (void *)dashboard, (void *)NULL);

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return (0);
}

void finish()
{
    deactivate_fan_and_resistor();
    ClrLcd();
    close_bme280();
    close_uart();
    refresh();
	endwin();
    exit(0);
}

void main_loop()
{
    float Kp = 5.0, Ki = 1.0, Kd = 5.0;
    float aux_TR;
    int time_to_write_csv = 0;

    signal(SIGINT, finish);
    pid_configure_constants(Kp, Ki, Kd);

    while (1)
    {

        if (reference_potentiometer)
        {
            aux_TR = pontentiometer_temperature(TR);
            if (aux_TR > TE && aux_TR < 100)
            {
                TR = aux_TR;
            }
        }

        TI = lm35_temperature(TI);
        TE = bme280_temperature();

        print_lcd(TR, TI, TE);

        pid_update_reference(TR);

        control_output = pid_control(TI);

        manage_gpio_devices(control_output);

        if (time_to_write_csv == 0)
            time_to_write_csv++;
        else if (time_to_write_csv == 1)
        {
            write_csv(TR, TE, TI, control_output);
            time_to_write_csv = 0;
        }

        show_infos(TR, TI, TE, control_output, reference_potentiometer);

        usleep(700000);
    }
}

void menu()
{
    int choose;
    do
    {
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
    printf("Temperatura referência: ");
    scanf("%f", &reference);
    if (reference <= TE || reference >= 100)
    {
        reference_potentiometer = 0;
    }
}

void set_reference_input(int use_potenciometer, float new_reference){
    printf("%4.2f", new_reference);
    reference_potentiometer = use_potenciometer;
    if(!use_potenciometer) {
        pid_update_reference(new_reference);
        TR = new_reference;
    }
}
