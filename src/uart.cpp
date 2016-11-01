#include "uart.h"
#include <termios.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdio.h>
#include <string.h>
#include <sys/ioctl.h>
#include <unistd.h>
#include <fcntl.h>

#define MAX_READ 8
#define MULTI 2
int fd[2];
const char *uart_dev[2] = {"/dev/ttyS0", "/dev/ttyS1"};

int UART_Init (enum UART tty)
{
	fd[tty] = open (uart_dev[tty], O_RDWR);
	if (fd[tty] < 0)
	{
		perror ("Open failed!\n");
		return -1;
	}

	return 0;
}

void UART_Term (enum UART tty)
{
	close (fd[tty]);
}

int UART_Write (enum UART tty,  char *buf, unsigned int num_to_write,
																	unsigned int *num_writen, unsigned int time_out)
{
	time_out = 0;
	*num_writen = write (fd[tty], buf, num_to_write);
	return 0;
}

int UART_Read (enum UART tty,  char *buf, unsigned int num_to_read,
																	unsigned int *num_read, unsigned int time_out)
{
	int ret;
	struct timeval tv;
	int cnt = 0;
	int cnt_max = 0x01 << MULTI;
	fd_set fds;

	*num_read = 0;
	time_out = time_out >> MULTI;
	FD_ZERO (&fds);
	FD_SET (fd[tty], &fds);

	tv.tv_sec = 0;
	tv.tv_usec = time_out;

	for (cnt = 0; cnt < cnt_max && num_to_read > 0; cnt++)
	{
		if (select (fd[tty] + 1, &fds, NULL, NULL, &tv) <= 0)
		{
			continue;
		}
		else
		{
			ret = read (fd[tty], buf + *num_read, num_to_read);
			if (ret < 0)
			{
				printf ("Read error!\n");
				continue;
			}
			else
			{
				cnt = 0;
				*num_read = *num_read + ret;
				num_to_read = num_to_read - ret;
			}
		}
	}

	return ret;
}

int UART_Flush (enum UART tty)
{
	return tcflush (fd[tty], TCIOFLUSH);
}

int UART_SetParams (enum UART tty, int data_bits, int stop_bits, int flow_control, int baud_rate)
{
	struct termios newtio, oldtio;
	flow_control = 0;

	if (tcgetattr (fd[tty], &oldtio) != 0)
	{
		perror ("SetupSerial 1");
		return -1;
	}
	bzero (&newtio, sizeof (newtio));

	newtio.c_cflag |= CLOCAL | CREAD;
	newtio.c_cflag &= ~CSIZE;
	switch (data_bits)
	{
		case 7:
			newtio.c_cflag |= CS7;
			break;
		case 8:
			newtio.c_cflag |= CS8;
		break;
	}

	switch (baud_rate)
	{
		case 2400:
			cfsetispeed (&newtio, B2400);
			cfsetospeed (&newtio, B2400);
			break;
		case 4800:
			cfsetispeed (&newtio, B4800);
			cfsetospeed (&newtio, B4800);
			break;
		case 9600:
			cfsetispeed (&newtio, B9600);
			cfsetospeed (&newtio, B9600);
			break;
		case 115200:
			cfsetispeed (&newtio, B115200);
			cfsetospeed (&newtio, B115200);
			break;
		default:
			cfsetispeed (&newtio, B9600);
			cfsetospeed (&newtio, B9600);
			break;
	}

	if (stop_bits == 1)
	{
		newtio.c_cflag &= ~CSTOPB;
	}
	else
	{
		if (stop_bits == 2)
		{
			newtio.c_cflag |= CSTOPB ;
		}
	}
	newtio.c_cc[VTIME] = 10;
	newtio.c_cc[VMIN] = 0;
	tcflush (fd[tty], TCIFLUSH);

	if ((tcsetattr (fd[tty], TCSANOW, &newtio)) != 0)
	{
		perror ("com set error");
		return -1;
	}
	if (tcgetattr (fd[tty], &oldtio) != 0)
	{
		perror ("SetupSerial 1");
		return -1;
	}
	printf ("set done!\n");

	return 0;
}


