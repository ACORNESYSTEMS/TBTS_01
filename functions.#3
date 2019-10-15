//FUNCTIONS.H

#ifndef _FUNCTIONS__H
#define _FUNCTIONS__H

#include <c8051f120.h>
#include <stdio.h>
#include <string.h>
#include <intrins.h>

#define EXTERNAL_CLK     //TESTADC
#define EXTERNAL_ADC     

//#define INTERNAL_ADC            










#define NO_OF_SAMPLES_FOR_AVERAGING      8
#define NACK			"$000001NN"
#define ACK				"$000001AA"
#define TESTOK	    	"$000002TC\x17"
#define TESTSTARTED 	"$000002TS\x07"


#define ENABLE_INT0_INTERRUPT()		{\
										IT0		=	1;\
										EX0		=	1;\
						    		}


#define DISABLE_INT0_INTERRUPT()	{\
										IT0 = 0;\
										EX0 = 0;\
									}	


#define ADC_MAX_BUFF_SIZE		1536
#define ADC_NO_OF_CHANNELS		4
#define EACH_CHANNEL_BYTES		2
#define FLASH_PAGE_SIZE			256

#define PAGE_SIZE_WRITE			((FLASH_PAGE_SIZE)/(ADC_NO_OF_CHANNELS*EACH_CHANNEL_BYTES))

#define CHANNEL_POSITIVE_SCALE			4.33//4.2886   //4.3886  	// Modified on 11-01-06 for +/- analog values 
#define CHANNEL_NEGATIVE_SCALE			4.32296   	// Modified on 11-01-06 for +/- analog values    

#define CHANNEL_NEGATIVE_OFFSET         2.8543      // Modified on 11-01-06 for +/- analog values 
#define CHANNEL_POSITIVE_OFFSET        0.54 //0.54 Modified on 05-10-2019 by kss      // Modified on 11-01-06 for +/- analog values 


//#define CHANNEL_0_POSITIVE_SCALE			4.33//4.2886   //4.3886  	// Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_0_NEGATIVE_SCALE			4.32296   	// Modified on 11-01-06 for +/- analog values    

//#define CHANNEL_0_NEGATIVE_OFFSET         2.8543      // Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_0_POSITIVE_OFFSET        0.40 //0.54 Modified on 05-10-2019 by kss      // Modified on 11-01-06 for +/- analog values 



//#define CHANNEL_1_POSITIVE_SCALE			4.33//4.2886   //4.3886  	// Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_1_NEGATIVE_SCALE			4.32296   	// Modified on 11-01-06 for +/- analog values    

//#define CHANNEL_1_NEGATIVE_OFFSET         2.8543      // Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_1_POSITIVE_OFFSET        0.40 //0.54 Modified on 05-10-2019 by kss      // Modified on 11-01-06 for +/- analog values 


//#define CHANNEL_2_POSITIVE_SCALE			4.33//4.2886   //4.3886  	// Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_2_NEGATIVE_SCALE			4.32296   	// Modified on 11-01-06 for +/- analog values    

//#define CHANNEL_2_NEGATIVE_OFFSET         2.8543      // Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_2_POSITIVE_OFFSET        0.40 //0.54 Modified on 05-10-2019 by kss      // Modified on 11-01-06 for +/- analog values 


//#define CHANNEL_3_POSITIVE_SCALE			4.33//4.2886   //4.3886  	// Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_3_NEGATIVE_SCALE			4.32296   	// Modified on 11-01-06 for +/- analog values    

//#define CHANNEL_3_NEGATIVE_OFFSET         2.8543      // Modified on 11-01-06 for +/- analog values 
//#define CHANNEL_3_POSITIVE_OFFSET        0.40 //0.54 Modified on 05-10-2019 by kss      // Modified on 11-01-06 for +/- analog values 




#define RELAY_TOLERENCE			10

//#define FILE_START_ADDRESS 0x000000L
#define FILE_NUMBER(filename) ((filename-1)* 0x1FFFFL+(filename-1))

#define DATA_RECORD_TARGET_FLASH_MEMORY		0x10000L //bank 2
#define DATA_RECORD_ADDRESS(testnumber)		( (testnumber-1)*1024 + DATA_RECORD_TARGET_FLASH_MEMORY )

//*************************************************************************

#define RELAY_DATA_FLASH_MEMORY		0xC000L //
#define RELAY_DATA_ADDRESS(relaysetnumber)		( (relaysetnumber-1)*1024 + RELAY_DATA_FLASH_MEMORY )


//**************************************************************************

#define ACTIVE_BATTERIES_INFO		0xEC00
#define ACTIVE_BATTERY1_INFO		ACTIVE_BATTERIES_INFO
#define ACTIVE_BATTERY2_INFO		ACTIVE_BATTERIES_INFO+9

	


#define SYSTEMCLOCKK 				22118400

#define SAMPLE_RATE  				100000        // Sample frequency in Hz
//#define SAMPLE_RATE  				50000        // Sample frequency in Hz


#define ADC_RESOLUTION	      		0.00003662109375  //16 bit resolution formulae 
#define ADC_12BIT_RESOLUTION		0.0005859375
#define EXT_ADC_TEN_VOLTS 			0.00030517578125  // 16 bit resolution



#define ADC_RES_TEN_VOLTS 			0.00030517578125    	//TESTADC


#define BYTE(x) (1<<(x))						//TESTADC
#define SETBIT(p,x) (p |= BYTE(x))				//TESTADC
#define CLEARBIT(p,x) (p &= ~BYTE(x))			//TESTADC




#define ENABLE_DISABLE_DEBUGGING	0   //1 FOR ENABLE 0- DISBLE
#define MAX_COLUMNS   				20

#define K1_RELAY					31
#define K2_RELAY					32
#define K3_RELAY					33

#define IGN_DURATION				34

#define START_DURATION				35
#define DURATION					36
#define COUNT						37
#define INTERVAL					38
#define TIME_SETTINGS				39
#define DATE_SETTINGS				40
#define RELAY_SETTINGS				41
#define IGNITION					42
#define K2_RELAY_STOP				43
#define K3_RELAY_STOP				44
#define TEST_DURATION				45
#define K1_RELAY_STOP				46
#define BATTERY_SERIAL				47
#define SECTION_SETTINGS    		48

/*
#define	LCD_CLEAR_DISPLAY			0x01
#define	LCD_CURSOR_ON				0x0E
#define	LCD_CURSOR_OFF				0x0C
#define	LCD_BLINKING_ON				0x0F
#define	LCD_BLINKING_OFF			0x0C
#define	LCD_CURSOR_HOME				0x02
#define	LCD_CURSOR_LEFT				0x10
#define	LCD_CURSOR_RIGHT			0x14
*/

#define LCDSOM      				0xfe
#define LCDCUR      				0x47
#define LCD_BLINK_OFF       		0x54

#define	LCD_CLEAR_DISPLAY			0x58
#define	LCD_CURSOR_ON				0x4A
#define	LCD_CURSOR_OFF				0x4B
#define	LCD_BLINKING_ON				0x53
#define	LCD_BLINKING_OFF			0x54
#define	LCD_CURSOR_HOME				0x48
#define	LCD_CURSOR_LEFT				0x4C
#define	LCD_CURSOR_RIGHT			0x4D
#define LCD_BACK_LIGHT_ON           0x42








#define TYPE_307					1
#define TYPE_417					0

#define FIRST_KEY					1
#define SECOND_KEY					2
#define THIRD_KEY					3
#define FOURTH_KEY					4
#define FIFTH_KEY					5
#define SIXTH_KEY					6
#define SEVENTH_KEY					7
#define EIGHTH_KEY					8
#define NINTH_KEY					9
#define ZERO_KEY					10
#define ENTER_KEY					11
#define	LEFT_ARROW_KEY				12
#define	RIGHT_ARROW_KEY				13
#define CLEAR_KEY					14
#define DECIMAL_POINT_KEY 			15
#define RESET_KEY					16	


#define MAX_RECORDS					50


#define MIN_K1_START_TIME			0
#define MAX_K1_START_TIME			120000
#define MIN_K1_STOP_TIME			00
#define MAX_K1_STOP_TIME			120000

#define MIN_K2_START_TIME			0
#define MAX_K2_START_TIME			120000
#define MIN_K2_STOP_TIME			00
#define MAX_K2_STOP_TIME			120000

#define MIN_K3_START_TIME			0
#define MAX_K3_START_TIME			120000
#define MIN_K3_STOP_TIME			00
#define MAX_K3_STOP_TIME			120000

#define MIN_PR_START_TIME			1
#define MAX_PR_START_TIME			120000
#define MIN_PR_INTERVAL_TIME 		00
#define MAX_PR_INTERVAL_TIME		120000

#define MIN_PR_DURATION				1
#define MAX_PR_DURATION				12000
#define MIN_PR_COUNT_TIME			00
#define MAX_PR_COUNT_TIME			10


#define OUTPUT_DATA 		P2
#define INPUT_DATA_LSB 		P4
#define INPUT_DATA_MSB 		P5
#define KEYPORT 			P6
#define LCD_DATA   			P7

//sbit STROBE   			= P1^2;
//sbit PRINTER_BUSY     	= P1^3;

sbit START_SWITCH			= P1^2;  	//pull up mode
sbit OUTPUT_LATCH_ENABLE    = P1^6;	
sbit CE 					= P1^7;

sbit IGNITER_ENABLE			= P1^4;


sbit READY_LED				= P2^0;  	//open drain mode
sbit DAQ_LED				= P2^1;  	//open drain mode
sbit SOLID_STATE_RELAY		= P2^2; 	//Igniter
sbit PULSE_RELAY			= P2^3; 	//SPDT
sbit K1RELAY				= P2^4; 	//DPDTsbit PULSE_RELAY			= P2^3; //SPDT
sbit K2RELAY				= P2^5; 	//DPDT
sbit K3RELAY				= P2^6; 	//DPDT

sbit SPI_WP_FLASH1   		= P3^0;
sbit SPI_HOLD_FLASH1 		= P3^1;
sbit SPI_WP_FLASH2   		= P3^2;	
sbit SPI_HOLD_FLASH2 		= P3^3;
sbit RS_485_CTRL     		= P3^4;
sbit LCD_E 	         		= P3^5;
sbit LCD_RS          		= P3^6;
sbit LCD_RW          		= P3^7;

sbit READ_CONVERT 			= P4^0; 	//Adc start of conversion
sbit DATACLK	  			= P4^1; 	//Data clock reading the adc data
sbit DATA		  			= P4^2; 	//Adc data outputted on this pin with respect to clock
sbit ADC_BUSY	  			= P4^3; 	//Adc end of conversion 
sbit A0			  			= P4^4; 	//channel selection 1
sbit A1			  			= P4^5; 	//channel selection 2
	
sbit ROW1 					= P6^0;
sbit ROW2 					= P6^1;
sbit ROW3 					= P6^2;
sbit ROW4 					= P6^3;
sbit COLUMN1 				= P6^4;
sbit COLUMN2 				= P6^5;
sbit COLUMN3 				= P6^6;
sbit COLUMN4 				= P6^7;

sbit LCD_D0 				= P7^0;
sbit LCD_D1 				= P7^1;
sbit LCD_D2 				= P7^2;
sbit LCD_D3 				= P7^3;
sbit LCD_D4 				= P7^4;
sbit LCD_D5 				= P7^5;
sbit LCD_D6 				= P7^6;
sbit LCD_D7 				= P7^7;

sbit CHIP3_ENABLE 			= P2^5;




//extern void Enable_ADC();
//extern void ADCInit (void);
//extern void TIMER3_Init (int counts);

extern void SignOnMessage();
extern void LCDPutString(unsigned char address,unsigned char *string);
extern void LCDPutChar(unsigned char byte);
extern void WaitLCDBusy();
extern void LCD_Delay(unsigned int count);
extern void LCDInit(void);
extern void LCDDelayms(unsigned char count);
extern void LCDCmdWrite(unsigned char cmd);
extern void LCDGoToXY(unsigned char Colum,unsigned char Row);

extern void ConfigWatchDogTimer();
extern void SYSCLKInit(void);
extern void PortIOInit (void);

extern void SendByteToRTC(unsigned char ByteAddress,unsigned char OutByte);
extern unsigned char RecvByteFromRTC(unsigned char ByteAddress);
extern void SMBInit(void);
extern void GetTime_Date(struct time *Time);

extern void Init_SPI( void );
extern void UART1_Init(void);
extern void UART0_Init();

extern char Read_Device_Id_Status(unsigned char Chip_Number);
extern bit EnableDisableChipSelect(unsigned char ChipSelect,bit SetValue);
extern bit ReadStatusRegister(unsigned char);
extern void Timer0_us (unsigned us);
extern char FlashBulkErase(unsigned char Chip_Number);
extern void MainMenu();

extern void PutStringOnRS232(unsigned char *String);
extern void TransmitCharOnRS232(unsigned char value);
extern void TransmitCharRS485(unsigned char value);
void Put_String_UART1(unsigned char *String);
extern char ScanKeyPad();
extern unsigned int ReadInputPort();
extern void SendOutPutPort(unsigned char Data);

extern void Delayms(unsigned char count);
extern void Delay(unsigned int count);
void delay(unsigned int);
void Timer4neMsecInterruptInit();

bit ReadFileFromFlash(unsigned char FileName,unsigned long Length);
bit ConfigureRTCTime();
unsigned long WriteFiletoFlash(unsigned long,unsigned char);
char ClearRecords(unsigned char FileName,unsigned char Count);
char EraseRecords(unsigned char FileName,unsigned char Count);

/**************** Internal Flash Routines*********************/
void FLASH_ByteWrite (unsigned long dest, char dat);
void FLASH_Write (unsigned long dest, char* src, unsigned int numbytes);
void FLASH_Read (unsigned long dest, char* src, unsigned int numbytes);
void FLASH_PageErase (unsigned long addr);
unsigned char FLASH_ByteRead (unsigned long addr);
/*******************************************************************/

char ReadDirectory();
void CygnalInit();

//void LcdInit();
void LCDInstWrite(unsigned char instruction);
//void LCDDataWrite(unsigned char character);
void LCDGotoXY(unsigned char X, unsigned char Y);
void LCDPutText(char *datastr);
void LCDPutChar(char datachar);
void LCDWriteLine(unsigned char LineNo, char *datastr, unsigned char Alignment);
void LCDClearLine(unsigned char LineNo);
void LCDCursorON();
void LCDCursorOFF();

void WelcomeScreen();
void RecordsInformation();
void TestConfigurations();
void StartStopTime();
void IntervalDuration();
void ConnectBatteryTerminals();
void ConnectLoads();
void ResetToAbort();
void TestProcessing();
void TestCompleted();
void RelaySelection();
void BatterySerailNumber();
void TestConfigurations();
void RelaySettings();
void IgniterSettings();
void ConnectBatteryTerminals();
void MainMenu();
void DateAndTimeScreen();
void RelaySelections();

void Delayms(unsigned char count);
void Keydelay(unsigned int count);

char ScanKeyPad();

void ClearAllLines();
void NewRelayK1();
void NewRelayK2();
void NewRelayK3();
void IntervalDuration();
void delaylong(unsigned long milliseconds);
void DisplayTime();
unsigned int IntervalSettings(unsigned char Key);

//void BatterySerialSetting(unsigned char Key);
//char KeyReadingInfo(unsigned char Key,unsigned char CountMax);

/*****************************************************************************************/
bit DecodeCommands();
unsigned char Checksum(unsigned char *cbuffer); 
void PutCharOnRS232(unsigned char value);
void SendDataToRS232(unsigned char *TxBuffer);
void ReadRecordLists();
//*******************************************************************************************/

void FLASH_Read ( unsigned long src, char* dest, unsigned int numbytes);
void IntializeVariables();
void Settings();

void TestCondition_50p();
void TestCondition_50n();
void TestCondition_30n();
void TestCondition_55p();
void TestCondition_Room_Temp();
void TestConditoion_Menu();
void SectionSettings();
void GetActiveBatteries();

void ReadInput(unsigned char KeyValue,unsigned char CountMax);

// added on 18-12-2006

void Delay(unsigned int Count);
unsigned int ReadADCData();

#endif
