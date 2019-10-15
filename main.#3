
/*
	Project		: TBTS(Thermal Battery Test System), ECIL
	File Name	: main.c
	Description	: Thermal Batteries are primary reserve batteries that are solid state at normal 
				  temperature.For many years,they have been the first choice of power supply for
				  guided missiles and nuclear weapons.They use an electrolyte, such as a mixture of
				  lithium chloride and potassium chloride.
	Advantages	: 
					1.Able withstand severe mechanical stresses of acceleration,shock,vibration and spin.
					2.Rugged
					3.safe
					4.High power and energy densities
					5.Instantaneous activation
					6.No maintenance
	Disadvantage:
					1.Expensive
	Applications :
					1.Military applications
					2.Missiles
					3.Weapons systems.
	Measurements: Connect the Battery Terminals to the DAQ module and follow the operation procedures.
	Interfaces	: 
					1.24x2 LCD Interface
					2.4x4 Keys Interface
					3.12Bit-ADC0 4-channel Interfaces
					4.RS-232 Interfaces
					5.8 Digital Relay Outputs
					6.16 OptoIsolator Inputs
					7.Flash Memory Interface (M25p32 4MB flash).
	Created On	: 23-June-2006
	Modified On	: 30-August-2006
	Version		: 3.3		
		Copyright © 2006 www.sorokasoft.com
 
*/

#include <functions.h>
#include <stdlib.h>
#include "Record.h"
#include "ds1307.h"
#include "lcd.h"



#define ADC_RES_TEN_VOLTS 0.00030517578125  

extern enum Screens MMI_screen;
extern struct Relay xdata WriteRelayValues;
extern struct Relay xdata ReadRelayValues;
extern struct time xdata stTime;
extern struct Record xdata stRecordInfo;
extern struct Record xdata stRecordReadInfo;
extern unsigned char xdata KeyBuffer[14];
extern	unsigned char xdata TestType;
extern unsigned char idata KeyIndex;
//extern unsigned char BatType;

extern unsigned char BatteryIndex;
extern unsigned char idata FreeRecords;
extern unsigned char xdata RecordsUsed;
extern unsigned char TransmitChksum;
extern unsigned int xdata ADCCh[ADC_MAX_BUFF_SIZE];
extern unsigned char xdata	LCD_CurrentX;
extern unsigned char xdata	LCD_CurrentY;

extern unsigned char xdata ActiveBat1Info[12];
extern unsigned char xdata ActiveBat2Info[12];



extern bit Remote;
extern bit ChksumErr;
//extern bit DecimalPoint;
extern bit completedPacket;
extern bit gbDebugPort;
// Contains Sections to Channels mapping
unsigned char SectionsMapping[4];

//unsigned int Adcvoltage;

extern void SetTime_Date(struct time stTime);
extern bit ChksumErr;
unsigned char CursorIndex=0;




void main(void)
{
	unsigned char idata KeyPressed;
	unsigned char i;
	unsigned char testbuf[10];

	CygnalInit();
	EA =1;					//Enable global Interrupt
	SFRPAGE = CONFIG_PAGE;	
	//EraseRecords(1,50);
	IntializeVariables();
	sprintf(testbuf,"%c%c%c",LCDSOM,LCD_BACK_LIGHT_ON,0);
	LCDPutText(testbuf);
	LCDInstWrite(LCD_BLINKING_OFF);
	WelcomeScreen(); //sign on message
	if(gbDebugPort)
	{		
		PutStringOnRS232("Welcome To Thermal Battery Test System00\r\n");
	}
	for(i=0;i<=15;i++)
	{
  		delay(0xffff);
  		delay(0xffff);
  		delay(0xffff);
		delay(0xffff);
	}
	// Assign the ADC channels to sections
	// First section to Channel#1
	SectionsMapping[0] = 1;
	// Second section to Channel#2
	SectionsMapping[1] = 2;
	// Third section to Channel#4
	SectionsMapping[2] = 4;
	// Igniter to Channel#3
	SectionsMapping[3] = 3;
	
	i=0;
  	MainMenu();  //main menu
	SFRPAGE = CONFIG_PAGE;		/* Set SFR Page to 15 for initializing Parallel Ports */
	DATACLK =0;



	while(1)
	{

		if(ChksumErr) //checksum error occurs
		{
			ChksumErr=0;
			PutStringOnRS232(NACK);
		}
		if(MMI_screen !=TBT_MMI_DATE_TIME_SETTINGS && MMI_screen !=TBT_MMI_BATTERY_NUMERIC_KEY && MMI_screen !=TBT_MMI_NEW_RELAY_KEY1 && MMI_screen !=TBT_MMI_NEW_RELAY_KEY1_K2 && MMI_screen !=TBT_MMI_NEW_RELAY_KEY1_K3 && MMI_screen !=TBT_MMI_NEW_RELAY_KEY1_PR && MMI_screen != TBT_MMI_IGNITE_KEY && MMI_screen != SETTINGS &&  MMI_screen != TBT_MMI_CONDITION_50_1 &&  MMI_screen != TBT_MMI_CONDITION_50_2 &&  MMI_screen != TBT_MMI_CONDITION_50_3 &&  MMI_screen != TBT_MMI_CONDITION_50_4 && MMI_screen !=TBT_MMI_CONDITION_50_5 && MMI_screen !=TBT_MMI_SECTION_SETTINGS)
			DisplayTime();
		if(completedPacket)  //complete packet is received from serial port 
		{
			completedPacket =0;
			DecodeCommands();   //decoding serial commands
		}
		KeyPressed = ScanKeyPad();  //scan keypad
		switch(KeyPressed)
		{
			case FIRST_KEY :   //first key
				switch(MMI_screen)
				{
					case TBT_MMI_MAIN_MENU_KEYS:  //record information
				      RecordsInformation();
					break;
					case TBT_MMI_TEST_SELECTION:  //display battery serial number
					   	 BatterySerailNumber();
					  	 LCDInstWrite(LCD_BLINKING_ON);
					  	 LCDGotoXY(10,3);
					break;
					case TBT_MMI_TEST_CONFIGURATION:	//test configuration
						strcpy(stRecordInfo.Battery1Type,ActiveBat1Info);
						for(BatteryIndex =1;BatteryIndex <=10;BatteryIndex++)
						{
							
							FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
							if((strncmp(ReadRelayValues.BatteryName,ActiveBat1Info,7)) == 0)
								break;
						}
						if(TestType !=RELAY_SETTINGS && TestType != SECTION_SETTINGS)
							BatterySerailNumber();
						else
						if(TestType == RELAY_SETTINGS)
						{
							TestType  = 0xFF;	
							RelaySelections();
						}
						else
						if(TestType == SECTION_SETTINGS)
						{
							TestType  = 0xFF;	
							SectionSettings();
						}
					
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('1',10);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('1',9);
						else
							ReadInput('1',7);
						
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('1',3);
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('1',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('1',1);
						else if(TestType == DURATION)
							ReadInput('1',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('1',4);

					break;
					case TBT_MMI_RELAY_SELECTION_KEYS:
						FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
						IntervalDuration();
					break;
					case SETTINGS:
						DateAndTimeScreen();
					break;
					case TBT_MMI_CONDITION_MENU:
						TestCondition_50p();
					break;
					case TBT_MMI_SECTION_SETTINGS:
						if(KeyIndex >=0 && KeyIndex <2)
						{
							LCDPutChar('1');
							KeyBuffer[KeyIndex++]= '1';
						}
					break;
				}
		   	break;
			case SECOND_KEY :	//second key
				switch(MMI_screen)
				{
					case SETTINGS:
						TestType = RELAY_SETTINGS;
						TestConfigurations();
					break;
					case TBT_MMI_MAIN_MENU_KEYS:
						Settings();
					break;
					case TBT_MMI_TEST_CONFIGURATION:
						strcpy(stRecordInfo.Battery1Type,ActiveBat2Info);
						for(BatteryIndex =1;BatteryIndex <=10;BatteryIndex++)
						{
							
							FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
							if((strncmp(ReadRelayValues.BatteryName,ActiveBat2Info,7)) == 0)
								break;
						}
					
						if(TestType !=RELAY_SETTINGS && TestType != SECTION_SETTINGS)
							BatterySerailNumber();
						else
						if(TestType == RELAY_SETTINGS)
						{
							TestType  = 0xFF;	
							RelaySelections();
						}
						else
						if(TestType == SECTION_SETTINGS)
						{
							TestType  = 0xFF;	
							SectionSettings();
						}
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('2',10);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('2',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('2',9);
						else
							ReadInput('2',7);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('2',1);
						else if(TestType == DURATION)
							ReadInput('2',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('2',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('2',4);
					break;
					case TBT_MMI_RELAY_SELECTION_KEYS:
						FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
						NewRelayK1();
					break;
					case TBT_MMI_CONDITION_MENU:
						TestCondition_50n();
					break; 
					case TBT_MMI_SECTION_SETTINGS:
						if(KeyIndex >=0 && KeyIndex <2)
						{
							LCDPutChar('2');
							KeyBuffer[KeyIndex++]= '2';
						}
					break;
				}
	  	    break;
			case THIRD_KEY :    //third key
				switch(MMI_screen)
				{
					case TBT_MMI_MAIN_MENU_KEYS:
						IntializeVariables();  // Added on 21st Jan 2007	
						TestConfigurations();
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('3',10);
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('3',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('3',1);
						else if(TestType == DURATION)
							ReadInput('3',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('3',4);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('3',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('3',9);
						else
							ReadInput('3',7);
					break;
					case TBT_MMI_RELAY_SELECTION_KEYS:
						FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
						NewRelayK2();
					break;
					case TBT_MMI_CONDITION_MENU:
						TestCondition_30n();
					break;
					case SETTINGS:
						//SectionSettings();
						TestType = SECTION_SETTINGS;
						TestConfigurations();

					break;
					case TBT_MMI_SECTION_SETTINGS:
						if(KeyIndex >=0 && KeyIndex <2)
						{
							LCDPutChar('3');
							KeyBuffer[KeyIndex++]= '3';
						}
					break;
				}
			break;
			case FOURTH_KEY : 	//fourth key
				switch(MMI_screen)
				{
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('4',10);
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('4',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('4',1);
						else if(TestType == DURATION)
							ReadInput('4',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('4',4);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('4',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('4',9);
						else
							ReadInput('4',7);

					break;
					case TBT_MMI_RELAY_SELECTION_KEYS:
						FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
						NewRelayK3();
					break;

					case TBT_MMI_CONDITION_MENU:
						TestCondition_55p();
					break;
					
				}
			break;
			case FIFTH_KEY :   //fifth key
				switch(MMI_screen)
				{
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('5',4);
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('5',10);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('5',1);
						else if(TestType == DURATION)
							ReadInput('5',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('5',4);

					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('5',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('5',9);
						else
							ReadInput('5',7);
					break;
	
					case TBT_MMI_CONDITION_MENU:
						TestCondition_Room_Temp();
					break;
				}
			break;
			case SIXTH_KEY : 	//sixth key
				switch(MMI_screen)
				{
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('6',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('6',1);
						else if(TestType == DURATION)
							ReadInput('6',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('6',4);
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('6',10);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('6',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('6',9);
						else
							ReadInput('6',7);
					break;
				}
			break;
			case SEVENTH_KEY : 	//seventh key
				switch(MMI_screen)
				{
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('7',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('7',1);
						else if(TestType == DURATION)
							ReadInput('7',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('7',4);

					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('7',10);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('7',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('7',9);
						else
							ReadInput('7',7);
					break;
				}
			break;
			case EIGHTH_KEY : 		//eighth key
				switch(MMI_screen)
				{
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('8',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('8',1);
						else if(TestType == DURATION)
							ReadInput('8',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('8',4);
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('8',10);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('8',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('8',9);
						else
							ReadInput('8',9);
					break;
				}
			break;
			case NINTH_KEY : 	//ninth key
				switch(MMI_screen)
				{
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('9',4);
					break;
					//8 replaced with 9 for three read values
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('9',1);
						else if(TestType == DURATION)
							ReadInput('9',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('9',4);

					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('9',10);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('9',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('9',9);
						else
							ReadInput('9',7);
					break;
				}
			break;
			case ZERO_KEY : 	//zero key
				switch(MMI_screen)
				{
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						ReadInput('0',4);
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType ==COUNT)
							ReadInput('0',1);
						else if(TestType == DURATION)
							ReadInput('0',2);
						else if(TestType == START_DURATION || TestType == INTERVAL)
							ReadInput('0',4);
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						ReadInput('0',10);
					break;
					case TBT_MMI_IGNITE_KEY:
							ReadInput('0',3);
					break;
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
							ReadInput('0',9);
						else
							ReadInput('0',7);

					break;
					case TBT_MMI_SECTION_SETTINGS:
						if(KeyIndex >=0 && KeyIndex <2)
						{
							LCDPutChar('0');
							KeyBuffer[KeyIndex++]= '0';
						}
					break;	
				}
			break;
			case LEFT_ARROW_KEY: //left_arrow (B)
				switch(MMI_screen)
				{
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
						{
							if(KeyIndex <=9 && KeyIndex >0)
							{
								if(KeyIndex == 3 || KeyIndex == 6)
								{
									KeyIndex--;
									LCDInstWrite(LCD_CURSOR_LEFT);
								}
								LCDInstWrite(LCD_BLINKING_ON);
								LCDInstWrite(LCD_CURSOR_LEFT);
								KeyIndex--;
							}
						}
						else
						{
							if(KeyIndex <=7 && KeyIndex >0)
							{
								if(KeyIndex == 3 || KeyIndex == 6)
								{
									KeyIndex--;
									LCDInstWrite(LCD_CURSOR_LEFT);
								}
								LCDInstWrite(LCD_BLINKING_ON);
								LCDInstWrite(LCD_CURSOR_LEFT);
								KeyIndex--;
							}

						}
					
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						if(KeyIndex <=11 && KeyIndex >0)
						{
							KeyIndex--;
							LCDInstWrite(LCD_BLINKING_ON);
							LCDInstWrite(LCD_CURSOR_LEFT);
						}
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType == DURATION)
						{
							if(KeyIndex > 0)                            //Modified from "== 2" to ">0"
							{
								KeyIndex--;
								LCDInstWrite(LCD_CURSOR_LEFT);
								//LCDInstWrite(LCD_CURSOR_LEFT);        // Modified on 7-11-06
								//KeyIndex--;                           // Modified on 7-11-06
							}
						}
						else if(TestType == START_DURATION || TestType == INTERVAL)
						{
							if(KeyIndex <=4 && KeyIndex >=1)
							{
								if(KeyIndex == 4)
								{
									KeyIndex--;
							  		LCDInstWrite(LCD_CURSOR_LEFT);
								}
								LCDInstWrite(LCD_BLINKING_ON);
						  		LCDInstWrite(LCD_CURSOR_LEFT);
								KeyIndex--;	
							}
						}
						else if(TestType == COUNT)
						{
							if(KeyIndex >0 && KeyIndex <2)
							{
								KeyIndex--;
								LCDInstWrite(LCD_CURSOR_LEFT);
							}
						}
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						if(KeyIndex <=4 && KeyIndex >=1)
						{
							if(KeyIndex == 4)
							{
								KeyIndex--;
							  	LCDInstWrite(LCD_CURSOR_LEFT);
							}
							LCDInstWrite(LCD_BLINKING_ON);
						  	LCDInstWrite(LCD_CURSOR_LEFT);
							KeyIndex--;	
			
						}
					break;
					
					case TBT_MMI_IGNITE_KEY:
						if(KeyIndex <=4 && KeyIndex >0)
						{
							KeyIndex--;	
							if(KeyIndex == 3)
							  	LCDInstWrite(LCD_CURSOR_LEFT);
							LCDInstWrite(LCD_BLINKING_ON);
						  	LCDInstWrite(LCD_CURSOR_LEFT);
			
						}
  					break;
					case TBT_MMI_CONDITION_50_1:
					case TBT_MMI_CONDITION_50_2:
					case TBT_MMI_CONDITION_50_3:
					case TBT_MMI_CONDITION_50_4:
//					case TBT_MMI_CONDITION_50_5:
						if(LCD_CurrentY == 2)
							LCDGotoXY(1,3);
						else if(LCD_CurrentY == 3)
							LCDGotoXY(1,4);
						else if(LCD_CurrentY == 4)
							LCDGotoXY(1,1);
						else if(LCD_CurrentY == 1)
							LCDGotoXY(1,2);
					break;	

					case TBT_MMI_CONDITION_50_5:
						if(LCD_CurrentY == 2)
							LCDGotoXY(1,3);
						else if(LCD_CurrentY == 3)
							LCDGotoXY(1,4);
						else if(LCD_CurrentY == 4)
						{
							if(!i)
							{
								i =1;
								LCDGotoXY(1,1);
							}
							else
							{
								i =0;
								LCDGotoXY(15,4);
							}

//							LCDGotoXY(1,1);
						}
						else if(LCD_CurrentY == 1)
							LCDGotoXY(1,2);
					break;				
					case TBT_MMI_SECTION_SETTINGS:				
						if(KeyIndex >0 && KeyIndex <=2)
						{
							KeyIndex--;
							LCDInstWrite(LCD_CURSOR_LEFT);
						}
					break;

				}
			break;
			case RIGHT_ARROW_KEY : //right _arrow (A on keypad)
				switch(MMI_screen)
				{
					case TBT_MMI_DATE_TIME_SETTINGS:
						if(TestType == TIME_SETTINGS)
						{
							if(KeyIndex <9 && KeyIndex >=0)
							{
								if(KeyIndex == 1 || KeyIndex == 4)
								{
									KeyIndex++;
									LCDInstWrite(LCD_CURSOR_RIGHT);
								}
								LCDInstWrite(LCD_BLINKING_ON);
								LCDInstWrite(LCD_CURSOR_RIGHT);
								KeyIndex++;
							}
						}
						else
						{
							if(KeyIndex <7 && KeyIndex >=0)
							{
								if(KeyIndex == 1 || KeyIndex == 4)
								{
									KeyIndex++;
									LCDInstWrite(LCD_CURSOR_RIGHT);
								}
								LCDInstWrite(LCD_BLINKING_ON);
								LCDInstWrite(LCD_CURSOR_RIGHT);
								KeyIndex++;
							}

						}
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
						if(KeyIndex <10 && KeyIndex >=0)
						{
							KeyIndex++;
							LCDInstWrite(LCD_BLINKING_ON);
							LCDInstWrite(LCD_CURSOR_RIGHT);
						}
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:
						if(TestType == DURATION)
						{
								if(KeyIndex < 2 )                      // Modified from "==0" to "<2"
							{
								KeyIndex++;
								LCDInstWrite(LCD_CURSOR_RIGHT);
								//LCDInstWrite(LCD_CURSOR_RIGHT);  // Modified on 7-11-06
								//KeyIndex++;                      // Modified on 7-11-06
							}
						}
						else if(TestType == START_DURATION || TestType == INTERVAL)
						{
							if(KeyIndex <3 && KeyIndex >=0)
							{
								if(KeyIndex == 2)
								{
									KeyIndex++;
							  		LCDInstWrite(LCD_CURSOR_RIGHT);
								}
								LCDInstWrite(LCD_BLINKING_ON);
							  	LCDInstWrite(LCD_CURSOR_RIGHT);
								KeyIndex++;
							}
						}
						else if(TestType == COUNT)
						{
							if(KeyIndex >=0 && KeyIndex <1)
							{
								KeyIndex++;
								LCDInstWrite(LCD_CURSOR_RIGHT);
							}
						}
					
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
						if(KeyIndex <3 && KeyIndex >=0)
						{
								if(KeyIndex == 2)
								{
									KeyIndex++;
							  		LCDInstWrite(LCD_CURSOR_RIGHT);
								}
								LCDInstWrite(LCD_BLINKING_ON);
							  	LCDInstWrite(LCD_CURSOR_RIGHT);
								KeyIndex++;	
						}
					break;
					case TBT_MMI_IGNITE_KEY:
						if(KeyIndex <3)
						{
							KeyIndex++;	
  							LCDInstWrite(LCD_BLINKING_ON);
					  		LCDInstWrite(LCD_CURSOR_RIGHT);
						}
				  	break;
					case TBT_MMI_CONDITION_50_1:
					case TBT_MMI_CONDITION_50_2:
					case TBT_MMI_CONDITION_50_3:
					case TBT_MMI_CONDITION_50_4:

						if(LCD_CurrentY == 2)
							LCDGotoXY(1,1);
						else if(LCD_CurrentY == 3)
							LCDGotoXY(1,2);
						else if(LCD_CurrentY == 4)
							LCDGotoXY(1,3);
						else if(LCD_CurrentY == 1)
							LCDGotoXY(1,4);
					break;	
					case TBT_MMI_CONDITION_50_5:
						if(LCD_CurrentY == 2)
							LCDGotoXY(1,1);
						else if(LCD_CurrentY == 3)
							LCDGotoXY(1,2);
						else if(LCD_CurrentY == 4)
						{
							if(!i)
							{
								i =1;
								LCDGotoXY(1,3);
							}
							else
							{
								i =0;
								LCDGotoXY(15,4);
							}
						}
						else if(LCD_CurrentY == 1)
							LCDGotoXY(1,4);
					break;
					case TBT_MMI_SECTION_SETTINGS:				
						if(KeyIndex >=0 && KeyIndex <2)
						{
							KeyIndex++;
							LCDInstWrite(LCD_CURSOR_RIGHT);
						}
					break;

				}
  			break;
			case ENTER_KEY :  //enter key(* on keypad)
				switch(MMI_screen)
				{
					case TBT_MMI_DATE_TIME_SETTINGS:
						KeyIndex =0;
						if(TestType == TIME_SETTINGS)
						{
							stTime.Date  = ((KeyBuffer[0] -'0')<<4)| (KeyBuffer[1] -'0');
							stTime.Month = ((KeyBuffer[3] -'0')<<4)| (KeyBuffer[4] -'0');
							stTime.Year  = ((KeyBuffer[6] -'0')<<12)|((KeyBuffer[7] -'0')<<8) | ((KeyBuffer[8]-'0')<<4) | (KeyBuffer[9]-'0');
							if(stTime.Date >=1 && stTime.Date <= 0x31)
							{
								if(stTime.Month >=1 && stTime.Month <=0x12)
								{
									if(stTime.Year >=0x2006)
									{
										TestType = DATE_SETTINGS;
										LCDGotoXY(6,4);
										sprintf(KeyBuffer,"%02x:%02x:%02x",(unsigned int)stTime.Hours&0x3f,(unsigned int)stTime.Minutes,(unsigned int)stTime.Seconds); 
									}
									else
									{
										TestType = TIME_SETTINGS;
										LCDGotoXY(6,3);
									}
								}
								else
								{
									TestType = TIME_SETTINGS;
									LCDGotoXY(6,3);
								}
							}
							else
							{
								TestType = TIME_SETTINGS;
								LCDGotoXY(6,3);
							}
								
						}
				 		else if(TestType == DATE_SETTINGS)
						{
							stTime.Hours  = ((KeyBuffer[0] -'0')<<4)| (KeyBuffer[1] -'0');
							stTime.Minutes= ((KeyBuffer[3] -'0')<<4)| (KeyBuffer[4] -'0');
							stTime.Seconds= (((KeyBuffer[6]-'0')<<4) | (KeyBuffer[7]-'0'));
							if(stTime.Hours >=0 && stTime.Hours <=0x24)
							{
								if(stTime.Minutes >=0 && stTime.Minutes <=0x59)
								{
									if(stTime.Seconds >=0 && stTime.Seconds <=0x59)
									{
										SetTime_Date(stTime);
										LCDInstWrite(LCD_BLINKING_OFF);
										MainMenu();
									}
								}
							}
							else
							{
								TestType = DATE_SETTINGS;
								LCDGotoXY(6,4);
							}
						}
					break;
					case TBT_MMI_RECORD_INFO:  
					case TBT_MMI_TEST_COMPLETED:
					case TBT_MMI_TEST_STATUS:
						MainMenu();
					break;
					case TBT_MMI_BATTERY_NUMERIC_KEY:
//						if(!KeyIndex == 0)
//							strcpy(stRecordInfo.SerialNumber,"00000000000");
//						else
							KeyBuffer[11]= '\0';
							strcpy(stRecordInfo.SerialNumber,KeyBuffer);
							KeyIndex =0;
							LCDInstWrite(LCD_BLINKING_OFF);
//MODIFIED ON 07 AUGUST 2006 BY NAGARJUNA
							TestConditoion_Menu();
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
						if(TestType ==K1_RELAY)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
								LCDGotoXY(15,4);
								sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.K1StopTime/1000.0));
								TestType =K1_RELAY_STOP;
//								TestType = 0xFF;
							}
							else
							{
								WriteRelayValues.K1StartTime = atof(KeyBuffer)*1000;
								if(WriteRelayValues.K1StartTime >=MIN_K1_START_TIME && WriteRelayValues.K1StartTime <=MAX_K1_START_TIME)
								{
									LCDGotoXY(15,4);
									sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.K1StopTime/1000.0));
									TestType =K1_RELAY_STOP;
	//								TestType = 0xFF;
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID K1 RELAY START TIME\r\n");
									LCDGotoXY(15,3);
									KeyIndex =0;
								}
							}
						}
						else
						{
							if(!KeyIndex)
							{
								FLASH_Read  (RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
								WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
								WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;
								WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
								WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
								WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
								WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
								WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
								WriteRelayValues.PRStart = ReadRelayValues.PRStart;
								WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
								WriteRelayValues.PRCount = ReadRelayValues.PRCount;
								WriteRelayValues.Interval = ReadRelayValues.Interval;
								WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
								WriteRelayValues.Testduration = ReadRelayValues.Testduration;
								//added
								strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
							
								FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
								FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
								
								LCDInstWrite(LCD_BLINKING_OFF);
								LCDWriteLine(2,"                    ",2);
								TestType = 0xFF;
								RelaySelections();
							}
							else
							{
								WriteRelayValues.K1StopTime = atof(KeyBuffer)*1000;
								if(WriteRelayValues.K1StopTime >=MIN_K1_STOP_TIME && WriteRelayValues.K1StopTime <=MAX_K1_STOP_TIME)
								{
									LCDInstWrite(LCD_BLINKING_OFF);
									FLASH_Read  (RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
									WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
									WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

									WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
									WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
									WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
									WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
									WriteRelayValues.PRStart = ReadRelayValues.PRStart;
									WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
									WriteRelayValues.PRCount = ReadRelayValues.PRCount;
									WriteRelayValues.Interval = ReadRelayValues.Interval;
									WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
									WriteRelayValues.Testduration = ReadRelayValues.Testduration;

									strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
									FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
									FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
									LCDInstWrite(LCD_BLINKING_OFF);
									LCDWriteLine(2,"                    ",2);
									RelaySelections();
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID K1 RELAY STOP TIME\r\n");
									LCDGotoXY(15,4);
									KeyIndex =0;
								}
							}
						}
						KeyIndex =0;
//						DecimalPoint =0;
					break;			
					case TBT_MMI_NEW_RELAY_KEY1_K2:
					//	KeyBuffer[KeyIndex] ='\0';
						if(TestType ==K2_RELAY)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
								LCDGotoXY(15,4);
								sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.K2StopTime/1000.0));
								TestType = K2_RELAY_STOP; //0xFF;
							}
							else
							{
								WriteRelayValues.K2StartTime = atof(KeyBuffer)*1000;
								if(WriteRelayValues.K2StartTime >=MIN_K2_START_TIME && WriteRelayValues.K2StartTime <=MAX_K2_START_TIME)
								{
									LCDGotoXY(15,4);
									sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.K2StopTime/1000.0));
									TestType = K2_RELAY_STOP;
									//TestType = 0xFF;
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID K2 RELAY START TIME\r\n");
									WriteRelayValues.K2StartTime =0;
									LCDGotoXY(15,3);
									KeyIndex =0;
								}
							}
						}
						else
						{
							if(!KeyIndex)
							{
								WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
								FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
								WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
								WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

								WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
								WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
								WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
								WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
								WriteRelayValues.PRStart = ReadRelayValues.PRStart;
								WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
								WriteRelayValues.PRCount = ReadRelayValues.PRCount;
								WriteRelayValues.Interval = ReadRelayValues.Interval;

								strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
								WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
								WriteRelayValues.Testduration = ReadRelayValues.Testduration;
								FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
								FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
								
								LCDInstWrite(LCD_BLINKING_OFF);
								LCDWriteLine(2,"                    ",2);
								TestType = 0xff; //K2_RELAY_STOP;
								RelaySelections();
							}
							else
							{
								WriteRelayValues.K2StopTime = atof(KeyBuffer)*1000;
								if(WriteRelayValues.K2StopTime >=MIN_K2_STOP_TIME && WriteRelayValues.K2StopTime <=MAX_K2_STOP_TIME)
								{
									TestType = K2_RELAY;
									LCDInstWrite(LCD_BLINKING_OFF);
									FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
									WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
									WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

									WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
									WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
									WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
									WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
									WriteRelayValues.PRStart = ReadRelayValues.PRStart;
									WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
									WriteRelayValues.PRCount = ReadRelayValues.PRCount;
									WriteRelayValues.Interval = ReadRelayValues.Interval;
									WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
									WriteRelayValues.Testduration = ReadRelayValues.Testduration;
									strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
	
									FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
									FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
									LCDInstWrite(LCD_BLINKING_OFF);
									LCDWriteLine(2,"                    ",2);
									RelaySelections();
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID K2 RELAY STOP TIME\r\n");
									LCDGotoXY(15,4);
									KeyIndex =0;
								}
							}
						}
						KeyIndex =0;
//						DecimalPoint =0;
					break;
					case TBT_MMI_CONDITION_50_1:
						if(LCD_CurrentY == 1)
							stRecordInfo.TestCondition =4;
						else if(LCD_CurrentY == 2)
							stRecordInfo.TestCondition =5;
						else if(LCD_CurrentY == 3)
							stRecordInfo.TestCondition =6;
						else if(LCD_CurrentY == 4)
							stRecordInfo.TestCondition =7;
						else
						{
							//Invalid Entry
						}
						ConnectBatteryTerminals();
					break;

					case TBT_MMI_CONDITION_50_2:
						if(LCD_CurrentY == 1)
							stRecordInfo.TestCondition =1;
						else if(LCD_CurrentY == 2)
							stRecordInfo.TestCondition =2;
						else if(LCD_CurrentY == 3)
							stRecordInfo.TestCondition =3;
						else
						{
							//Invalid Entry
						}
						ConnectBatteryTerminals();
					break;					

					case TBT_MMI_CONDITION_50_3:
						if(LCD_CurrentY == 1)
							stRecordInfo.TestCondition =13;
						else if(LCD_CurrentY == 2)
							stRecordInfo.TestCondition =14;
						else if(LCD_CurrentY == 3)
							stRecordInfo.TestCondition =15;
						else if(LCD_CurrentY == 4)
							stRecordInfo.TestCondition =16;
						else
						{
							//Invalid Entry
						}
						ConnectBatteryTerminals();
					break;					

					case TBT_MMI_CONDITION_50_4:
						if(LCD_CurrentY == 1)
							stRecordInfo.TestCondition =17;
						else if(LCD_CurrentY == 2)
							stRecordInfo.TestCondition =18;
						else if(LCD_CurrentY == 3)
							stRecordInfo.TestCondition =19;
						else if(LCD_CurrentY == 4)
							stRecordInfo.TestCondition =20;
						else
						{
							//Invalid Entry
						}
						ConnectBatteryTerminals();
					break;					

					case TBT_MMI_CONDITION_50_5:
						if(LCD_CurrentY == 1)
							stRecordInfo.TestCondition =8;
						else if(LCD_CurrentY == 2)
							stRecordInfo.TestCondition =9;
						else if(LCD_CurrentY == 3)
							stRecordInfo.TestCondition =10;
						else if(LCD_CurrentY == 4 && LCD_CurrentX == 1)
							stRecordInfo.TestCondition =11;
						else if(LCD_CurrentY == 4 && LCD_CurrentX == 15)
							stRecordInfo.TestCondition =12;

						else
						{
							//Invalid Entry
						}
						ConnectBatteryTerminals();
					break;					
					case TBT_MMI_NEW_RELAY_KEY1_K3:
					//	KeyBuffer[KeyIndex] ='\0';
						if(TestType ==K3_RELAY)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
								LCDGotoXY(15,4);
								sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.K3StopTime/1000.0));
//								TestType = 0xFF;
								TestType = K3_RELAY_STOP;

							}
							else
							{
								WriteRelayValues.K3StartTime = atof(KeyBuffer)*1000;
								if(WriteRelayValues.K3StartTime >=MIN_K3_START_TIME && WriteRelayValues.K3StartTime <=MAX_K3_START_TIME)
								{
									LCDGotoXY(15,4);
									sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.K3StopTime/1000.0));
									TestType = K3_RELAY_STOP;
	//								TestType = 0xFF;
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID K3 RELAY START TIME\r\n");
									WriteRelayValues.K3StartTime =0;
									LCDGotoXY(15,3);
									KeyIndex =0;
								}
							}
						}
						else
						{
							if(!KeyIndex)
							{
								WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
								FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
								WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
								WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

								WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
								WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
								WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
								WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
								WriteRelayValues.PRStart = ReadRelayValues.PRStart;
								WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
								WriteRelayValues.PRCount = ReadRelayValues.PRCount;
								WriteRelayValues.Interval = ReadRelayValues.Interval;
								WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
								WriteRelayValues.Testduration = ReadRelayValues.Testduration;

								strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
							//	memcpy(&WriteRelayValues,&ReadRelayValues,sizeof(struct Relay));
								FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
								FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
								
								LCDInstWrite(LCD_BLINKING_OFF);
								LCDWriteLine(2,"                    ",2);

								TestType = 0xFF;
								RelaySelections();
							}
							else
							{
								WriteRelayValues.K3StopTime = atof(KeyBuffer)*1000;
								if(WriteRelayValues.K3StopTime >=MIN_K3_STOP_TIME && WriteRelayValues.K2StartTime <=MAX_K2_STOP_TIME)
								{
									TestType = K3_RELAY;
									LCDInstWrite(LCD_BLINKING_OFF);
									FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
									WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
									WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

									WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
									WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
									WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
									WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
									WriteRelayValues.PRStart = ReadRelayValues.PRStart;
									WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
									WriteRelayValues.PRCount = ReadRelayValues.PRCount;
									WriteRelayValues.Interval = ReadRelayValues.Interval;
									WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
									WriteRelayValues.Testduration = ReadRelayValues.Testduration;
	
									strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
			
									FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
									FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
									LCDInstWrite(LCD_BLINKING_OFF);
									LCDWriteLine(2,"                    ",2);
									RelaySelections();
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID K3 RELAY STOP TIME\r\n");
									WriteRelayValues.K3StopTime =0;
									LCDGotoXY(15,4);
									KeyIndex =0;
								}
							}
						}
						KeyIndex =0;
//						DecimalPoint =0;
					break;
					case TBT_MMI_NEW_RELAY_KEY1_PR:   //pulse relay settings
						if(TestType == START_DURATION)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.PRStart = ReadRelayValues.PRStart;
								LCDGotoXY(9,3);
								sprintf(KeyBuffer,"%03ld",(ReadRelayValues.PRDuration));			//Modified from 1.1 float to  3 decimals
								TestType =DURATION;
							}

							else
							{
								WriteRelayValues.PRStart = atof(KeyBuffer)*1000;
								if(WriteRelayValues.PRStart >=MIN_PR_START_TIME && WriteRelayValues.PRStart <=MAX_PR_START_TIME)
								{
									LCDGotoXY(9,3);
									sprintf(KeyBuffer,"%03ld",(ReadRelayValues.PRDuration));		//Modified from 1.1 float to  3 decimals
									TestType =DURATION;
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID PR START  TIME\r\n");
									LCDGotoXY(10,2);
									KeyIndex =0;
									LCDInstWrite(LCD_BLINKING_ON);
									TestType = START_DURATION;
								}
							}
						}
						else if(TestType == DURATION)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
								LCDGotoXY(19,3);
								sprintf(KeyBuffer,"%02d",ReadRelayValues.PRCount);
								TestType =COUNT;
							}
							else
							{
								WriteRelayValues.PRDuration = atoi(KeyBuffer);	// Changed the PRDuration from Sec to msec by removing *1000
								if(WriteRelayValues.PRDuration >=MIN_PR_DURATION && WriteRelayValues.PRDuration <= MAX_PR_DURATION)
								{
									LCDGotoXY(19,3);
									sprintf(KeyBuffer,"%02d",ReadRelayValues.PRCount);
									TestType =COUNT;
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID PR DURATION  TIME\r\n");
									LCDGotoXY(9,3);
									KeyIndex =0;
									TestType =DURATION;
								}
							}
						}
						else if(TestType ==COUNT)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.PRCount = ReadRelayValues.PRCount;
								LCDGotoXY(13,4);
								sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.Interval/1000.0));
								LCDInstWrite(LCD_BLINKING_ON);
								TestType =INTERVAL;
							}
							else
							{
								WriteRelayValues.PRCount = atoi(KeyBuffer);
								if(WriteRelayValues.PRCount >= MIN_PR_COUNT_TIME && WriteRelayValues.PRCount <=MAX_PR_COUNT_TIME)
								{
									LCDGotoXY(13,4);
									sprintf(KeyBuffer,"%05.1f",((float)ReadRelayValues.Interval/1000.0));
									LCDInstWrite(LCD_BLINKING_ON);
									TestType =INTERVAL;
								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID PR COUNT  TIME\r\n");
									LCDGotoXY(19,3);
									KeyIndex =0;
//									DecimalPoint =0;
									TestType =COUNT;
								}
							}
						}
						else if(TestType ==INTERVAL)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.Interval = ReadRelayValues.Interval;
								FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
								WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
								WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

								WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
								WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
								WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
								WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
								WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
								WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
								strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
								WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
								WriteRelayValues.Testduration = ReadRelayValues.Testduration;
								FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
								FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
									
								LCDInstWrite(LCD_BLINKING_OFF);
								LCDGotoXY(12,1);
								LCDPutText("  ");
								LCDWriteLine(2,"                    ",2);

								RelaySelections();
							}
							else
							{
								WriteRelayValues.Interval=atof(KeyBuffer)*1000;
								if(WriteRelayValues.Interval >=MIN_PR_INTERVAL_TIME && WriteRelayValues.Interval <=MAX_PR_INTERVAL_TIME)
								{
									if((WriteRelayValues.PRStart+(WriteRelayValues.PRCount*WriteRelayValues.Interval) - WriteRelayValues.PRDuration) <120000)
									{
										LCDInstWrite(LCD_BLINKING_OFF);
										FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
										WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
										WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

										WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
										WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
										WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
										WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
										WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
										WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
										WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
										WriteRelayValues.Testduration = ReadRelayValues.Testduration;
		
										strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
										FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
										FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
										LCDInstWrite(LCD_BLINKING_OFF);
										LCDGotoXY(12,1);
										LCDPutText("  ");
										LCDWriteLine(2,"                    ",2);
										RelaySelections();
									}
									else
									{
										LCDWriteLine(2,"INVALID SETTINGS(PR)",2);
										LCDInstWrite(LCD_BLINKING_OFF);
										RelaySelections();
									}

								}
								else
								{
									if(gbDebugPort)
										Put_String_UART1("\r\n INVALID PR INTERVAL  TIME\r\n");
									LCDGotoXY(13,4);
									KeyIndex =0;
//									DecimalPoint =0;
									TestType =INTERVAL;
								}
							}
						}
						KeyIndex = 0;
//						DecimalPoint =0;
					break;
					case TBT_MMI_RELAY_SELECTION_KEYS:  //added 07 july06
						FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
					    IgniterSettings();  
				  	break;	
					case TBT_MMI_IGNITE_KEY:
						if(TestType ==IGN_DURATION)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
								MMI_screen = TBT_MMI_IGNITE_KEY;
								sprintf(KeyBuffer,"%03ld",ReadRelayValues.Testduration);
								LCDGotoXY(15,4);
//								TestType = 0xff;
								TestType = TEST_DURATION;
							}
							else
							{
								WriteRelayValues.IgnitionDuration = atoi(KeyBuffer);
								MMI_screen = TBT_MMI_IGNITE_KEY;
								LCDGotoXY(15,4);
								TestType = TEST_DURATION;
//								TestType = 0xff;
							}
						}
						else
						{
							if(!KeyIndex)
							{
								WriteRelayValues.Testduration = ReadRelayValues.Testduration;
								FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
								WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
								WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

								WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
								WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
								WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
								WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;										WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
								WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
								WriteRelayValues.PRStart = ReadRelayValues.PRStart;
								WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
								WriteRelayValues.PRCount = ReadRelayValues.PRCount;
								WriteRelayValues.Interval = ReadRelayValues.Interval;

								strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
								FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
								FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
									
								LCDInstWrite(LCD_BLINKING_OFF);
								TestType = 0xff;
								MainMenu();   //added on 07 July
							}
							else
							{
//								WriteRelayValues.Testduration = atof(KeyBuffer)*1000;
								WriteRelayValues.Testduration = atoi(KeyBuffer); //changed from atof
								if(WriteRelayValues.Testduration*1000 <=120000)
								{
									if((WriteRelayValues.PRStart+(WriteRelayValues.PRCount*WriteRelayValues.Interval) - WriteRelayValues.PRDuration) <(WriteRelayValues.Testduration*1000))
									{
										LCDInstWrite(LCD_BLINKING_OFF);
										FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
										WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
										WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

										WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
										WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
										WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
										WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
										WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
										WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
										WriteRelayValues.PRStart = ReadRelayValues.PRStart;
										WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
										WriteRelayValues.PRCount = ReadRelayValues.PRCount;
										WriteRelayValues.Interval = ReadRelayValues.Interval;
										strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);								
										FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
										FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
										TestType = 0xff;
										MainMenu();   //added on 07 July
									}
									else
									{
										LCDWriteLine(2,"INVALID SETTINGS(TD)",2);
										RelaySelections();
									}
								}
								else
								{
									LCDGotoXY(15,4);
									TestType = 0xff;
								}
							}
						}
						KeyIndex = 0;
//						DecimalPoint =0;
					break;
					case TBT_MMI_CONNECT_BATTERY:
						ConnectLoads();
					break;
					case TBT_MMI_CONNECT_LOAD:
						ResetToAbort();
					break;
					case TBT_MMI_TEST_PROCESSING:
						TestProcessing();
					break;
					case TBT_MMI_SECTION_SETTINGS:
						
						if(TestType ==TBT_MMI_SECTION1_SETTINGS)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
								sprintf(KeyBuffer,"%02d",((unsigned int)ReadRelayValues.SecondSection));
								LCDGotoXY(14,4);
								TestType = TBT_MMI_SECTION2_SETTINGS;
							}
							else
							{
								KeyIndex =0;
								WriteRelayValues.FirstSection = atoi(KeyBuffer);
								if(WriteRelayValues.FirstSection <=0x03)
								{
									LCDGotoXY(14,4);
									
									TestType = TBT_MMI_SECTION2_SETTINGS;
								}
								else
								{
									LCDGotoXY(14,3);
									sprintf(KeyBuffer,"%02d",((unsigned int)ReadRelayValues.FirstSection));
									TestType = TBT_MMI_SECTION1_SETTINGS;
								}
							}
						}
						else if(TestType ==TBT_MMI_SECTION2_SETTINGS)
						{
							if(!KeyIndex)
							{
								WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;
								TestType = 0xff;
								LCDInstWrite(LCD_BLINKING_OFF);
								FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
								WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
								WriteRelayValues.K1StopTime = ReadRelayValues.K1StopTime;
								WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
								WriteRelayValues.K2StopTime = ReadRelayValues.K2StopTime;
								WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
								WriteRelayValues.K3StopTime = ReadRelayValues.K3StopTime;
								WriteRelayValues.PRStart = ReadRelayValues.PRStart;
								WriteRelayValues.PRDuration = ReadRelayValues.PRDuration;
								WriteRelayValues.PRCount = ReadRelayValues.PRCount;
								WriteRelayValues.Interval = ReadRelayValues.Interval;
								strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
								WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
								WriteRelayValues.Testduration = ReadRelayValues.Testduration;
								FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
								FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
								MainMenu();
							}
							else
							{
								KeyIndex =0;
								WriteRelayValues.SecondSection = atoi(KeyBuffer);
								if(WriteRelayValues.SecondSection <=0x03)
								{
									TestType = 0xff;
									LCDInstWrite(LCD_BLINKING_OFF);
									FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
									WriteRelayValues.K1StartTime = ReadRelayValues.K1StartTime;
									WriteRelayValues.K1StopTime  = ReadRelayValues.K1StopTime;
									WriteRelayValues.K2StartTime = ReadRelayValues.K2StartTime;
									WriteRelayValues.K2StopTime  = ReadRelayValues.K2StopTime;
									WriteRelayValues.K3StartTime = ReadRelayValues.K3StartTime;
									WriteRelayValues.K3StopTime  = ReadRelayValues.K3StopTime;
									WriteRelayValues.PRStart     = ReadRelayValues.PRStart;
									WriteRelayValues.PRDuration  = ReadRelayValues.PRDuration;
									WriteRelayValues.PRCount     = ReadRelayValues.PRCount;
									WriteRelayValues.Interval    = ReadRelayValues.Interval;
									strcpy(WriteRelayValues.BatteryName,ReadRelayValues.BatteryName);
									WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
									WriteRelayValues.Testduration = ReadRelayValues.Testduration;
									FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
									FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
									MainMenu();
									}
								else
								{
									LCDGotoXY(11,4);
									sprintf(KeyBuffer,"%02d",((unsigned int)ReadRelayValues.SecondSection));
									TestType = TBT_MMI_SECTION2_SETTINGS;
								}
							}
						}
		
					
					break;

				}
			break;
			case CLEAR_KEY : //clear key(# key on keypad)
				switch(MMI_screen)
				{
					case TBT_MMI_BATTERY_NUMERIC_KEY:
							LCDInstWrite(LCD_BLINKING_OFF);
							TestConfigurations();
					break;
					case TBT_MMI_TEST_SELECTION:
					case TBT_MMI_IGNITE_KEY:
					case TBT_MMI_RESET_ABORT:
					case TBT_MMI_TEST_CONFIGURATION:
					case TBT_MMI_DATE_TIME_SETTINGS:
					case TBT_MMI_CONNECT_BATTERY:  //added on july 07
					case TBT_MMI_CONNECT_LOAD:
					case TBT_MMI_RELAY_SELECTION_KEYS:  //added on july 70
					case TBT_MMI_TEST_PROCESSING:
					case TBT_MMI_TEST_COMPLETED:  //for testing only
					case SETTINGS:
					case TBT_MMI_SECTION_SETTINGS:
					case TBT_MMI_RECORD_INFO:  
					case TBT_MMI_CONDITION_MENU:
						LCDInstWrite(LCD_BLINKING_OFF);
						MainMenu();
					break;
					case TBT_MMI_NEW_RELAY_KEY1:
					case TBT_MMI_NEW_RELAY_KEY1_PR:
					case TBT_MMI_NEW_RELAY_KEY1_K3:
					case TBT_MMI_NEW_RELAY_KEY1_K2:
						LCDWriteLine(2,"                    ",2);
						LCDInstWrite(LCD_BLINKING_OFF);
						LCDGotoXY(12,1);
						LCDPutText("  ");
							
						RelaySelections();
					break;	
					case TBT_MMI_CONDITION_50_1:
					case TBT_MMI_CONDITION_50_2:
					case TBT_MMI_CONDITION_50_3:
					case TBT_MMI_CONDITION_50_4:
					case TBT_MMI_CONDITION_50_5:
						LCDInstWrite(LCD_BLINKING_OFF);
						TestConditoion_Menu();
					break;
				}
			break;
			case DECIMAL_POINT_KEY: //decimal key('C' key on keypad)


			break;
			case RESET_KEY:  //reset key('D' key on Keypad)
				SFRPAGE = 0x00;
				RSTSRC |= 0x10;  //reset the controller
			break;
						
			default :
				//		  RecordsInformation();					   					  		
			break;
		}//switch

	}//while		

} //main

//scan the input from the keypad and strores in a buffer.
void ReadInput(unsigned char KeyValue,unsigned char CountMax)
{
	switch(TestType)
	{						
		case IGN_DURATION:
		case TEST_DURATION:
			if(KeyIndex <CountMax)
			{
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex]=KeyValue;
				if(KeyIndex <CountMax-1)
					KeyIndex++;
				else
					LCDInstWrite(LCD_CURSOR_LEFT);
			}
		break;
		case K1_RELAY:
		case K2_RELAY:
		case K3_RELAY:
		case K3_RELAY_STOP:
		case K2_RELAY_STOP:
		case K1_RELAY_STOP:
		case START_DURATION:
		case INTERVAL:

			if(KeyIndex <=CountMax)
			{
				if(KeyIndex == 2 )
				{
					LCDPutChar(KeyValue);
					LCDPutChar('.');
					KeyBuffer[KeyIndex++]=KeyValue;
					KeyBuffer[KeyIndex++]='.';
				}
				else
				{
					LCDPutChar(KeyValue);
					KeyBuffer[KeyIndex]=KeyValue;
					if(KeyIndex <4)
						KeyIndex++;
					else
						LCDInstWrite(LCD_CURSOR_LEFT);
				}
			}
		break;
		case COUNT:
			if(KeyIndex == 0)
			{
				LCDInstWrite(LCD_BLINKING_ON);
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex++]=KeyValue;
			}
			else if(KeyIndex == 1)
			{
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex]=KeyValue;
				LCDInstWrite(LCD_CURSOR_LEFT);
			}

		break;
		case DURATION:
			    if(KeyIndex == 0)
			{
			    LCDInstWrite(LCD_BLINKING_ON);        // Added on 7-11-06
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex++]=KeyValue;
				//LCDPutChar('.');					  // Modified on 7-11-06
				//KeyBuffer[KeyIndex]='.';		      // Modified on 7-11-06
				//KeyIndex++;						  // Modified on 7-11-06
			}
			else if(KeyIndex == 1)                         // Condition added on 7-11-06
			{
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex++]=KeyValue;
			}
			else if(KeyIndex == 2)
			{
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex]=KeyValue;
				LCDInstWrite(LCD_CURSOR_LEFT);
			}
		break;
		case BATTERY_SERIAL:
			if(KeyIndex <=CountMax)
			{
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex]=KeyValue;
				if(KeyIndex <CountMax)
					KeyIndex++;
				else
					LCDInstWrite(LCD_CURSOR_LEFT);
			}
		break;
		case TIME_SETTINGS:
		case DATE_SETTINGS:
			if(KeyIndex <=CountMax)
			{
				LCDPutChar(KeyValue);
				KeyBuffer[KeyIndex]=KeyValue;
				if(KeyIndex == 1|| KeyIndex == 4)
				{
					LCDInstWrite(LCD_CURSOR_RIGHT);
					KeyIndex++;
				}
				if(KeyIndex <CountMax)
					KeyIndex++;
				else
					LCDInstWrite(LCD_CURSOR_LEFT);
			}
		break;
	}

}
//Intialize all global variables
void IntializeVariables()
{
	unsigned char idata Index;
	gbDebugPort = ENABLE_DISABLE_DEBUGGING;  //1 - enable debugging 0= disable debugging
	RecordsUsed  	= 	0;
	completedPacket =	0;
	KeyIndex 		=	0;
	TransmitChksum 	=	0;
	RecordsUsed 	=	0;
	FreeRecords 	=	0;
	Remote 			=	0;

	memset(&stRecordInfo,0,sizeof(struct Record));
	memset(&WriteRelayValues,0,sizeof(struct Relay));
	memset(&ReadRelayValues,0,sizeof(struct Relay));
	memset(&stRecordReadInfo,0,sizeof(struct Record));
	memset(&ADCCh,0,sizeof(ADCCh));
	//finding out records
	for(Index=1;Index<=MAX_RECORDS;Index++)
	{
		FLASH_Read( DATA_RECORD_ADDRESS(Index),(char*) &stRecordReadInfo,sizeof(struct Record));
		if((strncmp(stRecordReadInfo.Battery1Type,"ECT-",4) == 0) )
			RecordsUsed++;
		else
			FreeRecords++;
		
	}
}
