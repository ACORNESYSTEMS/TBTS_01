//	ad974 test 

/*
	CS pin ties to ground.
	WR1 and Wr2 pins are tied to ground.
	EXT/INT pinis connected to tied to +5V

	Input Voltage Range 	Connect VxA to	Connect VxB to  Input Impedence
	-/+ 10 v				    BIP				VIn				13.7 kohms
	0 to +5V					VIn				Gnd				6.0 kohms
	0 to 4v						VIn				VIn				6.4 kohms

*/

#include "functions.h"



#define ADC_RES_TEN_VOLTS 0.00030517578125  

#define BYTE(x) (1<<(x))
#define SETBIT(p,x) (p |= BYTE(x))
#define CLEARBIT(p,x) (p &= ~BYTE(x))

//unsigned int Adcvoltage=0;






unsigned int ReadADCData()
{
	unsigned char idata Counter=16;
	unsigned int idata Adcvoltage1;
	unsigned char idata Count;

	SFRPAGE   = CONFIG_PAGE;/* Set SFR Page to 00 */ 	

	READ_CONVERT =0;
	_nop_();
	READ_CONVERT =1;
	delay(0x2);
	DATACLK = 1;
	_nop_();
	DATACLK = 0;
	_nop_();
	DATACLK = 1;

	DATACLK = 0;
	if(DATA)
		SETBIT(Adcvoltage1,Counter--);
	else
		CLEARBIT(Adcvoltage1,Counter--);
	DATACLK = 1;
	_nop_();

	SFRPAGE   = CONFIG_PAGE;/* Set SFR Page to 00 */ 	

	for(Count =0;Count<16;Count++)
	{
	
		DATACLK = 0;
		if(DATA)
			SETBIT(Adcvoltage1,Counter--);
		else
			CLEARBIT(Adcvoltage1,Counter--);
		DATACLK = 1;
			_nop_();
			
	}
	DATACLK =0;
	while(!ADC_BUSY);

	if(Adcvoltage1 & 0x8000) 
		Adcvoltage1 &= 0x7FFF;
	else
		Adcvoltage1 = ~Adcvoltage1;

	return Adcvoltage1;
} 