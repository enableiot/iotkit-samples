extern int setGPIO_Out(int);
extern int GPIO_Write(int, int);
extern int unsetGPIO(int);

/* The following PIN# maps to sysfs gpio pin numbers */
//currently only pin4 thru pin8 have been validated 
#define PIN4 28
#define PIN5 17
#define PIN6 24
#define PIN7 27
#define PIN8 26

