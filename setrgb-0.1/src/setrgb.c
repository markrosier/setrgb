// I2C test program for a PCA9555

#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <linux/i2c-dev.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/ioctl.h>
#include <fcntl.h>

// I2C Linux device handle
int g_i2cFile;

// open the Linux device
void i2cOpen()
{
	g_i2cFile = open("/dev/i2c-0", O_RDWR);
	if (g_i2cFile < 0) {
		perror("i2cOpen");
		exit(1);
	}
}

// close the Linux device
void i2cClose()
{
	close(g_i2cFile);
}

// set the I2C slave address for all subsequent I2C device transfers
void i2cSetAddress(int address)
{
	if (ioctl(g_i2cFile, I2C_SLAVE, address) < 0) {
		perror("i2cSetAddress");
		exit(1);
	}
}

// write a 24 bit value 
void writeValue(uint8_t red, uint8_t green, uint8_t blue)
{
	uint8_t data[3];
	data[0] = red;
	data[1] = green;
	data[2] = blue;
	if (write(g_i2cFile, data, 3) != 3) {
		perror("writeValue");
	}
}

// read a 16 bit value from a register pair
uint16_t readValue(uint8_t reg)
{
	uint8_t data[3];
	data[0] = reg;
	if (write(g_i2cFile, data, 1) != 1) {
		perror("readValue set register");
	}
	if (read(g_i2cFile, data, 2) != 2) {
		perror(" read value");
	}
	return data[0] | (data[1] << 8);
}


int main(int argc, char** argv)
{
	int count;
	uint8_t red = 0;
	uint8_t green = 0;
	uint8_t blue = 0;
 	
	if (argc > 1)
    	{
      		for (count = 1; count < argc; count++)
		{
	  		printf("argv[%d] = %s\n", count, argv[count]);

			if (count == 1) red = atoi(argv[1]);
			if (count == 2) green = atoi(argv[2]);
			if (count == 3) blue = atoi(argv[3]);
		}
    	}
	else
    	{
      		printf("please set colour eg 255 0 15\n");
		return 0;
    	}

	// open Linux I2C device
	i2cOpen();

	// set address of the controller
	i2cSetAddress(0x53);

	writeValue(red++, green++, blue++);

	// close Linux I2C device
	i2cClose();

	return 0;
}
