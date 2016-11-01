#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <mtd/mtd-user.h>

#define ERASESIZE 0x20000
#define WRITESIZE 0x800
#define BUF_SIZE (64 * 1024)

static int memerase (int fd, struct erase_info_user *erase)
{
	return ioctl (fd, MEMERASE, erase);
}

int NandErase (unsigned int DeviceId, unsigned int offset, unsigned int bytes)
{
	int err;
	int fd;
	char device[50];
	struct erase_info_user erase;
	erase.start = offset;
	erase.length = bytes;
	memset (device, 0, 50);
	sprintf (device, "/dev/mtd%d", DeviceId);

	if ((fd = open (device, O_SYNC | O_RDWR)) < 0)
	{
		perror ("OPEN DEVICE");
		return 1;
	}
	err = memerase (fd, &erase);
	if (err < 0)
	{
		perror ("MEMERASE");
		close (fd);
		return 1;
	}
	fprintf (stderr, "Erased %d bytes from address 0x%.8x in flash\n", bytes, offset);

	if (close (fd) < 0)
	{
		perror ("close()");
	}
	return 0;
}

int NandRead (unsigned int DeviceId, u_int32_t offset, size_t len, char *buffer)
{
	u_int8_t *buf = NULL;
	int err;
	int size = len * sizeof (u_int8_t);
	int n = len;
	char device[50];
	int fd;
	memset (device, 0, 50);
	sprintf (device, "/dev/mtd%d", DeviceId);

	if ((fd = open (device, O_SYNC | O_RDWR)) < 0)
	{
		perror ("OPEN DEVICE");
		return 1;
	}

	if (offset != lseek (fd, offset, SEEK_SET))
	{
		perror ("lseek()");
		goto err0;
	}
retry:
	if ((buf = (u_int8_t *) malloc (size)) == NULL)
	{
		fprintf (stderr, "%s: malloc(%#x)\n", __func__, size);
		if (size != BUF_SIZE)
		{
			size = BUF_SIZE;
			fprintf (stderr, "%s: trying buffer size %#x\n", __func__, size);
			goto retry;
		}
		perror ("malloc()");
		goto err0;
	}
	do
	{
		if (n <= size)
		{
			size = n;
		}
		err = read (fd, buf, size);
		if (err != size)
		{
			fprintf (stderr, "%s: read, size %#x, n %#x\n", __func__, size, n);
			perror ("read()");
			goto err1;
		}
		n -= size;
	} while (n > 0);
	memcpy ((void *) buffer, (void *) buf, size);
	if (buf != NULL)
	{
		free (buf);
	}
	close (fd);
	printf ("Read %08x bytes from address 0x%08x in flash\n", len, offset);
	return 0;
err1:
	close (fd);
	if (buf != NULL)
	{
		free (buf);
	}
	return 1;
err0:
	close (fd);
	return 1;
}

int NandWrite (unsigned int DeviceId, u_int32_t offset, u_int32_t len, char *buffer)
{
	u_int8_t *buf = NULL;
	int err;
	int size = len * sizeof (u_int8_t);
	int n = len;
	char device[50];
	int fd;
	memset (device, 0, 50);
	sprintf (device, "/dev/mtd%d", DeviceId);
	if ((fd = open (device, O_SYNC | O_RDWR)) < 0)
	{
		perror ("OPEN DEVICE");
		return 1;
	}
	if (offset != lseek (fd, offset, SEEK_SET))
	{
		perror ("lseek()");
		close (fd);
		return 1;
	}
retry:
	if ((buf = (u_int8_t *) malloc (size)) == NULL)
	{
		fprintf (stderr, "%s: malloc(%#x) failed\n", __func__, size);
		if (size != BUF_SIZE)
		{
			size = BUF_SIZE;
			fprintf (stderr, "%s: trying buffer size %#x\n", __func__, size);
			goto retry;
		}
		perror ("malloc()");
		close (fd);
		return 1;
	}
	do
	{
		if (n <= size)
		{
			size = n;
		}
		memcpy ((void *) buf, (void *) buffer, size);
		err = write (fd, buf, size);
		if (err < 0)
		{
			fprintf (stderr, "%s: write, size %#x, n %#x\n", __func__, size, n);
			perror ("write()");
			free (buf);
			close (fd);
			return 1;
		}
		n -= size;
	} while (n > 0);

	if (buf != NULL)
	{
		free (buf);
	}
	close (fd);
	printf ("Copied %d bytes to address 0x%.8x in flash successly\n", len, offset);
	return 0;
}

