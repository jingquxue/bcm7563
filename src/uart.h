#ifndef KWT_UART_H
#define KWT_UART_H

enum UART
{
    UART0,
    UART1
};

int UART_Init (enum UART tty);
void UART_Term (enum UART tty);
int UART_Write (enum UART tty,  char *buf, unsigned int num_to_write,
                                                                unsigned int *num_writen, unsigned int time_out);
int UART_Read (enum UART tty,  char *buf, unsigned int num_to_read,
                                                                unsigned int *num_read, unsigned int time_out);
int UART_Flush (enum UART tty);
int UART_SetParams (enum UART tty, int data_bits, int stop_bits, int flow_control, int baud_rate);

#endif

