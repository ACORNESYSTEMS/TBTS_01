/*==============================================================================================*/
/* RIGHTS      : Acorn Embedded Systems India (Pvt) Ltd.
			       www.acornindia.com       
/* File Name   :Keypad.c
/* Description:Key pad routines                                                    */
/* Developed by: Nagarjuna Reddy.G,
				 nagarjuna@sorokasoft.com
/* HISTORY     : V1.00    AA    10-DEC-2005
/*================================================================================================= */


#include "functions.h"


#ifdef EXTERNAL_CLK
#define DEBOUNCE_DELAY 35
#else
#define DEBOUNCE_DELAY 150
#endif


volatile unsigned char RowSelect=0;

void Keydelay(unsigned int count)
{
	while(count--);
}
void Delayms(unsigned char count)
{
	while(count--)
		Keydelay(0x7a0);
}

void NOPS()
{
	_nop_();
	_nop_();
	_nop_();
	_nop_();
}

//Scan key pad which key pressed
//return - key value  
//return - -1 when key is not pressed
char ScanKeyPad()
{
	char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = CONFIG_PAGE;

	ROW1 = 0;  //Check Row 1
	ROW2 = 1;
	ROW3 = 1;
	ROW4 = 1;
#ifdef EXTERNAL_CLK
	NOPS();
#else
	NOPS();
	NOPS();
	NOPS();
#endif

	if(COLUMN1 == 0)  //check column1
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN1 != 1);
		return FIRST_KEY;
	}
	else if(COLUMN2 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN2 != 1);
		return SECOND_KEY;
	}
	else if(COLUMN3 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN3 != 1);
		return THIRD_KEY;
	}
	else if(COLUMN4 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN4 != 1);
		return RIGHT_ARROW_KEY;
	}

	SFRPAGE = CONFIG_PAGE;

	ROW1 = 1; //Check Row 1
	ROW2 = 0;
	ROW3 = 1;
	ROW4 = 1;
#ifdef EXTERNAL_CLK
	NOPS();
#else
	NOPS();
	NOPS();
	NOPS();
#endif

	if(COLUMN1 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN1 != 1);
		return FOURTH_KEY;
	}
	else if(COLUMN2 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN2 != 1);
		return FIFTH_KEY;
	}
	else if(COLUMN3 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN3 != 1);
		return SIXTH_KEY;
	}
	else if(COLUMN4 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN4 != 1);
		return LEFT_ARROW_KEY;
	}

	SFRPAGE = CONFIG_PAGE;

	ROW1 = 1;  //check ROW2
	ROW2 = 1;
	ROW3 = 0;
	ROW4 = 1;

#ifdef EXTERNAL_CLK
	NOPS();
#else
	NOPS();
	NOPS();
	NOPS();
#endif

	if(COLUMN1 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN1 != 1);
		return SEVENTH_KEY;
	}

	else if(COLUMN2 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN2 != 1);
		return EIGHTH_KEY;
	}
	else if(COLUMN3 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN3 != 1);
		return NINTH_KEY;
	}
	else if(COLUMN4 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN4 != 1);
		return DECIMAL_POINT_KEY;
	}

	SFRPAGE = CONFIG_PAGE;

	ROW1 = 1;		//Check Row3
	ROW2 = 1;
	ROW3 = 1;
	ROW4 = 0;
  #ifdef EXTERNAL_CLK
	NOPS();
#else
	NOPS();
	NOPS();
	NOPS();
#endif

	if(COLUMN1 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN1 != 1);
		return ENTER_KEY;
	}
	else if(COLUMN2 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN2 != 1);
		return ZERO_KEY;
	}
	else if(COLUMN3 == 0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN3 != 1);
		return CLEAR_KEY;
	}
	else if(COLUMN4 ==0)
	{
		Delayms(DEBOUNCE_DELAY);
		while(COLUMN4 != 1);
		return RESET_KEY;
	}
	SFRPAGE = SFRPAGE_SAVE;
	return -1;
}
		