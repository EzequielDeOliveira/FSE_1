#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <termios.h>
#include "crc16.h"

#define SERVER_CODE 0x01
#define REQUEST_CODE 0x23
#define REQUEST_LM35_TEMPERATURE 0xC1
#define REQUEST_POTENTIOMETER_TEMPERATURE 0xC2
#define LM35 1
#define POTENTIOMETER 2

int init_uart()
{
    int uart0_filestream = -1;
    uart0_filestream = open("/dev/serial0", O_RDWR | O_NOCTTY | O_NDELAY);
    if (uart0_filestream == -1)
    {
        printf("Failed to init UART\n");
    }
    else
    {
        printf("UART initialized\n");
    }

    struct termios options;
    tcgetattr(uart0_filestream, &options);
    options.c_cflag = B9600 | CS8 | CLOCAL | CREAD;
    options.c_iflag = IGNPAR;
    options.c_oflag = 0;
    options.c_lflag = 0;
    tcflush(uart0_filestream, TCIFLUSH);
    tcsetattr(uart0_filestream, TCSANOW, &options);
    return uart0_filestream;
}

void write_uart_message(int uart, int code)
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

    if (uart != -1)
    {
        printf("Writing on UART ...\n");
        int count = write(uart, &tx_buffer[0], (p_tx_buffer - &tx_buffer[0]));
        if (count < 0)
        {
            printf("UART TX error\n");
        }
        else
        {
            printf("Writed.\n");
        }
    }

    sleep(1);
}

void read_uart_message(int uart)
{
    if (uart != -1)
    {
        unsigned char rx_buffer[256];
        int rx_length = read(uart, (void *)rx_buffer, 255);
        if (rx_length < 0)
        {
            printf("Read Error %d", rx_length);
        }
        else if (rx_length == 0)
        {
            printf("Nothing \n");
        }
        else
        {
            rx_buffer[rx_length] = '\0';
            float f;
            memcpy(&f, &rx_buffer[3], 4);
            printf("float: %f\n", f);
        }
    }
}