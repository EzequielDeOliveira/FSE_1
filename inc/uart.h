#define SERVER_CODE 0x01
#define REQUEST_CODE 0x23
#define REQUEST_LM35_TEMPERATURE 0xC1
#define REQUEST_POTENTIOMETER_TEMPERATURE 0xC2
#define LM35 1
#define POTENTIOMETER 2

#ifndef UART_H_
#define UART_H_

int init_uart();
void write_uart_message(int code);
float read_uart_message();
float pontentiometer_temperature();
float lm35_temperature();
void close_uart();

#endif