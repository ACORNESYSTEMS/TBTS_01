//ds1307.h
// 

#ifndef _DS1307__H
#define _DS1307__H

struct time 
{
	unsigned char Seconds;
	unsigned char Minutes;
	unsigned char Hours;
	unsigned char Day;
	unsigned char Date;
	unsigned char Month;
	unsigned int Year;
};

#define SEC_ADD 	0x00
#define MIN_ADD 	0x01
#define HOUR_ADD 	0x02
#define DAY_ADD 	0x03
#define DATE_ADD 	0x04
#define MONTH_ADD 	0x05
#define YEAR_ADD 	0x06
#define CONTROL_ADD	0x07

#endif