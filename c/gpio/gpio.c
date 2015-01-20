/*
Copyright (c) 2013 - 2015, Intel Corporation

Redistribution and use in source and binary forms, with or without modification,
are permitted provided that the following conditions are met:

    * Redistributions of source code must retain the above copyright notice,
      this list of conditions and the following disclaimer.
    * Redistributions in binary form must reproduce the above copyright notice,
      this list of conditions and the following disclaimer in the documentation
      and/or other materials provided with the distribution.
    * Neither the name of Intel Corporation nor the names of its contributors
      may be used to endorse or promote products derived from this software
      without specific prior written permission.

THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS" AND
ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED
WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT OWNER OR CONTRIBUTORS BE LIABLE FOR
ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES
(INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES;
LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON
ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
(INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF THIS
SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
*/

#define MAXSTR 64

#include <stdio.h>
#include "gpio.h"

#define VALID_PINS PIN4, PIN5, PIN6, PIN7, PIN8

int setGPIO_Out(int pin)
{	
	/* This is a function to export a GPIO pin - and set it's direction to output... */

	/* Start with a test to see if we built in debug...
	   ...if we did, then show some more verbose descriptions of what we're doing. */
	#ifdef debug
		printf("Exporting pin %d for output\n", pin);
	#endif

	int valid_pins[]={VALID_PINS};
	int c;
	int valid = 0;

	/* Before we do anyting else, we need to check to see if the pin number passed in
	 * is a valid pin or not - we'll compare it against the pins defined in the list of
	 * valid pins */
	for (c=0;c<(sizeof(valid_pins) / sizeof(int));c++)
	{
		if(pin == valid_pins[c])
			valid = 1;
	}

	if(!valid)
	{
		fprintf(stderr, "ERROR: Invalid pin!\nPin %d is not a GPIO pin...\n", pin);
		return -1;
	}

	FILE *sysfs_handle = NULL;

	/* Now we define a file handle, for the 'gpio/export' sysfs file & try to open it
	 * Note that we open for writing, and as a binary file*/
		if ((sysfs_handle = fopen("/sys/class/gpio/export", "w")) == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open GPIO export...\n(Is this program running as root?)\n");
		return 1;
	}

	/* Next we need to convert our int pin value to a string... The safest way is to use snprintf
	 * Note that the length is n+1 as C strings are nul-terminated - so for a two-digit value - we
	 * need to specify 3 chars to be used. */
	char str_pin[3];
	snprintf(str_pin, (3*sizeof(char)), "%d", pin);	
	
	/* To actually export the pin - we simply write the string value of the pin number to the
	 * sysfs gpio/export file */
	if (fwrite(&str_pin, sizeof(char), 3, sysfs_handle)!=3)
	{
		fprintf(stderr, "ERROR: Unable to export GPIO pin %d\n", pin);
		return 2;
	}
	fclose(sysfs_handle);
	
	/* If we got to here, then we've been able to export the pin - so now we need to set the direction */
	/* We open the direction file for the pin...*/
	char str_direction_file[MAXSTR];
	snprintf(str_direction_file, (MAXSTR*sizeof(char)), "/sys/class/gpio/gpio%d/direction", pin);
	if ((sysfs_handle = fopen(str_direction_file, "w")) == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open direction file...\n");
		return 3;
	}
	
	/* ...and then we'll write "out" to the direction file.*/
	if (fwrite("out", sizeof(char), 4, sysfs_handle) != 4)
	{
	
		fprintf(stderr, "ERROR: Unable to write direction for GPIO%d\n", pin);
		return 4;
	}
	fclose(sysfs_handle);
	
	// If everything worked, we'll return 0 - an non-zero return value signifies something went wrong
	return 0;
}

int GPIO_Write(int pin, int value)
{
	/* This function will write a value (0 or 1) to the selected GPIO pin */

	/* Once again, if we built in debug mode - then show verbose output*/
	#ifdef debug
		printf("Writing value %d, to GPIO pin %d\n", value, pin);
	#endif
	
	/* The first thing that we need to do is check to see if we were passed a valid value */

	if ((value!=0)&&(value!=1))
	{
		fprintf(stderr, "ERROR: Invalid value!\nValue must be 0 or 1\n");
		return -1;
	}

	/* Now we're sure we have a god value, we'll try to open the sysfs value file for the pin
	 * We'll start by building the path - using snprintf... */
	FILE *sysfs_handle = NULL;
	char str_value_file[MAXSTR];

	snprintf (str_value_file, (MAXSTR*sizeof(char)), "/sys/class/gpio/gpio%d/value", pin);

	if ((sysfs_handle = fopen(str_value_file, "w")) == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open value file for pin %d...\n(Has the pin been exported?)\n", pin);
		return 1;
	}

	/* If the file is good - then we write the string value "0" or "1" to it. */
	char str_val[2];
	snprintf (str_val, (2*sizeof(char)), "%d", value);

	if(fwrite(str_val, sizeof(char), 2, sysfs_handle) != 2)
	{
		fprintf(stderr, "ERROR: Cannot write value %d to GPIO pin %d\n", value, pin);
		return 2;
	}
	fclose(sysfs_handle);

	return 0;
}

int unsetGPIO(int pin)
{
	/* This function "turns off" the specified pin - and unexports it. */

	/* If we built in debug - then produce verbose output */
	#ifdef debug
		printf("Unxporting pin %d\n", pin);
	#endif

	/* We start by building the string for the sysfs value file for the pin - to set it to 0*/
	FILE *sysfs_handle = NULL;
	char str_pin[3];
	char str_value_file[MAXSTR];

	snprintf (str_pin, (3*sizeof(char)), "%d", pin);
	snprintf (str_value_file, (MAXSTR*sizeof(char)), "/sys/class/gpio/gpio%d/value", pin);

	if ((sysfs_handle = fopen(str_value_file, "w")) == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open value file for pin %d...\n", pin);
		return 1;
	}

	if(fwrite("0", sizeof(char), 2, sysfs_handle) != 2)
	{
		fprintf(stderr, "ERROR: Cannot write to GPIO pin %d\n", pin);
		return 2;
	}
	fclose(sysfs_handle);
	
	/* Once we've done that - the last step is to open the gpio/unexport file - and write the pin
	 * number to it - to unexport the pin. */
	if ((sysfs_handle = fopen("/sys/class/gpio/unexport", "w")) == NULL)
	{
		fprintf(stderr, "ERROR: Cannot open GPIO unexport...\n");
		return 1;
	}

	if (fwrite(&str_pin, sizeof(char), 3, sysfs_handle)!=3)
	{
		fprintf(stderr, "ERROR: Unable to unexport GPIO pin %d\n", pin);
		return 2;
	}
	fclose(sysfs_handle);
	return 0;
}
