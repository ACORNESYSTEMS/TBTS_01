/*
File Name :adc1.c
Description :Intialze the controller ADC and Interrupt service routine,store data in ADC buffer
*/

#include <functions.h>

#define ADC_RESOLUTION	        0.00003662109375  //16 bit resolution formulae 
#define ADC_12BIT_RESOLUTION	0.0005859375
#define INT_DEC      			256               // integrate and decimate ratio

void ADCInit (void);
void TIMER3_Init (int counts);
void ADC_ISR (void);

unsigned int xdata ADCCh[ADC_MAX_BUFF_SIZE];
//   unsigned int idata Adc_Write_Index;
unsigned int idata Adc_Read_Index;
unsigned int idata NoOfElementsToWrite;
unsigned long Length=0;

sfr16 RCAP3    = 0xCA;                 // Timer3 reload value
sfr16 TMR3     = 0xCC;                 // Timer3 counter
sfr16 ADC0     = 0xbe;                 // ADC0 data

			  
void ADCInit (void)
{
	char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = ADC0_PAGE;
 
	ADC0CN = 0x04;                      // ADC disabled; normal tracking
                                       // mode; ADC conversions are initiated 
                                       // on overflow of Timer3; ADC data is
                                       // right-justified
	REF0CN = 0x03;                      // on-chip VREF,
	AD0EN =1;                                   // and VREF output buffer
	AMX0SL = 0x00;                      // Select ch0 ADC mux output

#ifdef EXTERNAL_CLK
	ADC0CF = (SYSTEMCLOCKK/2500000) << 3;     // ADC conversion clock = 2.5MHz
#else
	ADC0CF    = 0x88;
#endif

// ADC0CF = (SYSTEMCLOCKK/2500000) << 3;     // ADC conversion clock = 2.5MHz
   EIE2 |= 0x02;                       // enable ADC interrupts
   SFRPAGE = SFRPAGE_SAVE;
}


//-----------------------------------------------------------------------------
// Timer3_Init
//-----------------------------------------------------------------------------
// Configure Timer3 to auto-reload at interval specified by <counts> (no
// interrupt generated) using SYSCLK as its time base.


void Timer3_Init (int counts)
{
   char SFRPAGE_SAVE = SFRPAGE;        // Save Current SFR page
   SFRPAGE = TMR3_PAGE;
   TMR3CN = 0x00;                      // Stop Timer3; Clear TF3;
   TMR3CF = 0x08;                      // use SYSCLK as timebase
   RCAP3   = -counts;                  // Init reload values
   TMR3    = RCAP3;                    // set to reload immediately
   EIE2   &= ~0x01;                    // disable Timer3 interrupts
   SFRPAGE = SFRPAGE_SAVE;             // Restore SFR page
}

//Adc isr routine

#ifdef INTERNAL_ADC

void ADC0_ISR (void) interrupt 15
{
	static unsigned int_dec=INT_DEC;    // integrate/decimate counter
                                       // we post a new result when
                                       // int_dec = 0
   	static long accumulator=0L;         // here's where we integrate the
                                       // ADC samples

   	AD0INT = 0;          // clear ADC conversion complete indicator
   	accumulator += ADC0;            		// read ADC value and add to running total
	int_dec--;                          // update decimation counter
	if (int_dec == 0)
   	{                 					// if zero, then post result
		int_dec = INT_DEC;               // reset counter
      	ADCCh[Adc_Write_Index] = accumulator >> 4;
	  	AMX0SL++;
		if(AMX0SL == 4)
			AMX0SL = 0;
		NoOfElementsToWrite++;
//		if(NoOfElementsToWrite >=ADC_MAX_BUFF_SIZE)
//			LCDWriteLine(2,"ADC BUFFER OVERFLOW",2);
      	accumulator = 0L; 
	  	Adc_Write_Index++;               // reset accumulator
	  	if(Adc_Write_Index == ADC_MAX_BUFF_SIZE)
	  		Adc_Write_Index = 0;
   }
}


#endif