/* Program to turn RBG sensor to change its color to Red, Blue and Green */

#include "gpio.h"
#include <unistd.h>


int main()
{
	int pins[3]={PIN4, PIN5, PIN6};
	int i;
	int c;
	int j;

	for (i=0;i<(sizeof(pins) / sizeof(int));i++)
		if (setGPIO_Out(pins[i]))
			return -1;
	for(i=0;i<100;i++) {
		GPIO_Write(PIN4, (i+0)%3);
		GPIO_Write(PIN5, (i+1)%3);
		GPIO_Write(PIN6, (i+2)%3);
	//	sleep(1);
	}
	

	return 0;
}

