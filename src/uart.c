#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "crc16.h"
#include "uart.h"

int init_uart()
{
    int uart0 = -1;
    uart0 = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0 == -1)
    {
        printf("Failed to init UART\n");
    }
    else
    {
        printf("UART initialized\n");
    }

    struct termios options;
    tcgetattr(uart0, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0, TCIFLUSH);
    tcsetattr(uart0, TCSANOW, &options);
    return uart0;
}

void write_uart_message(int uart0, int code)
{
    unsigned char tx_buffer[20];
    unsigned char *p_tx_buffer;
    p_tx_buffer = &tx_buffer[0];

    *p_tx_buffer++ = SERVER_CODE;
    *p_tx_buffer++ = REQUEST_CODE;

    if (code == LM35)
    {
        *p_tx_buffer++ = REQUEST_LM35_TEMPERATURE;
    }
    else if (code == POTENTIOMETER)
    {
        *p_tx_buffer++ = REQUEST_POTENTIOMETER_TEMPERATURE;
    }

    *p_tx_buffer++ = 9;
    *p_tx_buffer++ = 3;
    *p_tx_buffer++ = 1;
    *p_tx_buffer++ = 6;

    short crc = calcula_CRC(tx_buffer, (p_tx_buffer - &tx_buffer[0]));
    memcpy(&tx_buffer[(p_tx_buffer - &tx_buffer[0])], &crc, 2);
    p_tx_buffer += 2; // CRC

    if (uart0 != -1)
    {
        int count = write(uart0, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART error on write\n");
            exit(1);
        }
    }
    usleep(1000000);
}

float read_uart_message(int uart0)
{
    float result;
    if (uart0 != -1)
    {
        unsigned char rx_buffer[256];
        int rx_length = read(uart0, (void *)rx_buffer, 255);
        if (rx_length < 0)
        {
            printf("Error on read UART.\n");
            result = -1;
        }
        else if (rx_length == 0)
        {
            printf("Nothing on UART\n");
            result = -1;
        }
        else
        {
            rx_buffer[rx_length] = '\0';
            float f;
            memcpy(&f, &rx_buffer[3], 4);
            result = f;
        }
    }
    return result;
}

float pontentiometer_temperature()
{
    int uart = init_uart();
    write_uart_message(uart, 2);
    float temp = read_uart_message(uart);
    close(uart);
    return temp;
}

float lm35_temperature()
{
    int uart = init_uart();
    write_uart_message(uart, 1);
    float temp = read_uart_message(uart);
    close(uart);
    return temp;
}