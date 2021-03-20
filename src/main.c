#include <stdio.h>
#include "uart.h"
#include "led.h"

void finish();

int main(int argc, char *argv[]) {
    
    int uart = init_uart();

    write_uart_message(uart, 1);

    read_uart_message(uart);

    finish(uart);

    print_lcd(12.0,12.0,12.0);

    return 0;
}

void finish(int uart){
    close(uart);
}