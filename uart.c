/*==============================================================================================*/
/* RIGHTS      : Acorn Embedded Systems India (Pvt) Ltd.
			       www.acornindia.com                                                           */
/* Developed by: Nagarjuna Reddy.G,
				 nagarjuna@sorokasoft.com
/* HISTORY     : V1.00    AA    10-DEC-2005
/*================================================================================================= */


#include<stdlib.h>
#include<functions.h>
#include "ds1307.h"
#include "Record.h"

extern struct Relay xdata WriteRelayValues;
extern struct Relay xdata ReadRelayValues;
extern struct time xdata stTime;
extern unsigned char TransmitChksum;
extern unsigned char xdata RecordsUsed;
extern bit Remote;
extern unsigned char idata FreeRecords;
extern unsigned char idata FileName;

unsigned char BatteryIndex=1;

struct Record xdata stRecordInfo;
struct Record xdata stRecordReadInfo;
unsigned char xdata RcvdBuffer[200];
unsigned char xdata TxdBuffer[600];
bit completedPacket;

unsigned char xdata ActiveBat1Info[]="ECT-417";
unsigned char xdata ActiveBat2Info[]="ECT-307";

void UART1_Init(void);
void UART0_Init();


#define SOM       '$'

extern void SetTime_Date(struct time stTime);


unsigned char xdata index,RecvdChksum;
unsigned int xdata MessageLength = 200;
bit Message;
bit ChksumErr=0;	

void UART1_Init(void)
{
 	SFRPAGE   = TIMER01_PAGE;
    TCON      = 0x40;
    TMOD      = 0x20;
    //TH1       = 0xA0;   //baud rate 9600
    TH1       = 0x70;   //baud rate 19200
    //TH1       = 0xB8;   //baud rate 19200
	CKCON     = 0x01;
    SFRPAGE   = UART1_PAGE;
    SCON1     = 0x50;
	EIE2     |= 0x40;   //enabe uart1 interrupt
}


void UART0_Init()
{
    SFRPAGE   = UART0_PAGE;
    SCON0     = 0x50;
    SSTA0     = 0x15;
    SFRPAGE   = TMR2_PAGE;
    TMR2CN    = 0x04;
    TMR2CF    = 0x08;
//    RCAP2L    = 0x70;  //baud rate 9600
//    RCAP2H    = 0xFF;

//	RCAP2L    = 0xF4;  //baud rate 115200
//    RCAP2H    = 0xFF;

#ifdef EXTERNAL_CLK
	RCAP2L    = 0xF4;  //baud rate 115200
    RCAP2H    = 0xFF;
#else
	RCAP2L    = 0xD0;
    RCAP2H    = 0xFF;
#endif


    IE       |= 0x90; // enable uart0 interrupt
}
char char2decimal(char c)
{
	if(c >='0' && c <= '9')
		return c-'0';
	if(c >='a' && c <='f')
		return 10 + c - 'a';
	if(c >='A' && c <= 'F')
		return 10 + c - 'A';
}

unsigned int myatoi(char *str)
{
	unsigned int number=0;
	while(*str)
		number = number*10+char2decimal(*str++);
	return number;
}

void mystrncpy(char *dest,const char*source,unsigned int len)
{
	unsigned char i;
	for(i=0;i<len;i++)
		*dest++ = *source++;
	*dest = '\0';
}
//pc interface 
void Uart0_isr(void) interrupt 4
{
	unsigned char ReceivedCharacter;
 	if(RI0)
	{
	
		RI0 = 0;
		ReceivedCharacter = SBUF0;

        if(ReceivedCharacter == SOM)
        {
			index = 0;
			RecvdChksum =0;
			Message = 1;
		}
		else
		if(Message == 1)
		{
			if(index >= 7 && index != (MessageLength+7))
				RecvdChksum=RecvdChksum^RcvdBuffer[index-1];

			RcvdBuffer[index++]=ReceivedCharacter;
			
			if(index == 6)
			{
				mystrncpy(TxdBuffer,RcvdBuffer,6);
				MessageLength = myatoi(TxdBuffer);
			}
			if(index == (MessageLength+7))
			{
				if(RcvdBuffer[index-1] == RecvdChksum)
				{
					completedPacket =1;
					RcvdBuffer[index-1]= '\0';

					Message = 0;
					index = 0;
                }
                else
				{
					completedPacket =0;
					ChksumErr  =1;
					Message = 0;
					index = 0;
				}
			}
		}											
	}

}

//debug port interface 
void UART1_ISR(void) interrupt 20
{
	if ((SCON1 & 0x01) == 0x01)
	{ 								// handle receive function
		SCON1 &= ~0x01; 			// RI1 = 0; clear RX complete
	}
}
//transmit data to pc 
void TransmitCharOnRS232(unsigned char value)
{
	char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = UART0_PAGE;
	TI0 = 0;
	SBUF0 = value;
	while(TI0 != 1);
	TI0 =0;
	SFRPAGE = SFRPAGE_SAVE;
}

//Debug port data transmission
void TransmitCharRS485(unsigned char value)
{
	char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = UART1_PAGE;
	TI1 = 0;
	SBUF1 = value;
	while(TI1 != 1);
	TI1 = 0;
	SFRPAGE = SFRPAGE_SAVE;
}
//Debug port data transmission
void Put_String_UART1(unsigned char *String)
{
	while(*String != '\0')
		TransmitCharRS485(*String++);
}


void PutStringOnRS232(unsigned char *String)
{
	while(*String != '\0')
		TransmitCharOnRS232(*String++);
}
//Decode commands from pc.
bit DecodeCommands()
{
	unsigned char *RcvdData;
	unsigned char Temp,Len ;
//	unsigned int idata Index=1;
	unsigned long MsgLength;
	bit TestResult= 0;
//	char idata buff[10];
	unsigned char idata Section1=0x01,Section2=0x02;
	switch(RcvdBuffer[6])
	{
		case 'S' :	// Set Time
					Temp = RcvdBuffer[7] -'0';
					stTime.Date =Temp << 4;
					stTime.Date |=(RcvdBuffer[8]-'0');

					Temp = RcvdBuffer[9] -'0';
					stTime.Month =Temp << 4;
					stTime.Month |=(RcvdBuffer[10]-'0');

					Temp = RcvdBuffer[13] -'0';
					stTime.Year =Temp << 4;
					stTime.Year |=(RcvdBuffer[14]-'0');

					Temp = RcvdBuffer[15] -'0';
					stTime.Hours =Temp << 4;
					stTime.Hours |=(RcvdBuffer[16]-'0');

					Temp = RcvdBuffer[17] -'0';
					stTime.Minutes =Temp << 4;
					stTime.Minutes |=(RcvdBuffer[18]-'0');

					Temp = RcvdBuffer[19] -'0';
					stTime.Seconds =Temp << 4;
					stTime.Seconds |=(RcvdBuffer[20]-'0');

					SetTime_Date(stTime);
					PutStringOnRS232(ACK);
			
		break;
		case 'G' ://  Transmit Time 
					GetTime_Date(&stTime);
					Len=sprintf(TxdBuffer,"S%02x%02x20%02x",(unsigned int)stTime.Date,(unsigned int)stTime.Month,(unsigned int)stTime.Year);
					Len+=sprintf(TxdBuffer+Len,"%02x%02x%02x",(unsigned int)stTime.Hours&0x3f,(unsigned int)stTime.Minutes,(unsigned int)stTime.Seconds);
					SendDataToRS232(TxdBuffer);
					
		break;
		case 'L' : // Transmit Test details
					ReadRecordLists();
		break;
		case 'F' :  // Fetch Test Data
					RcvdData = strtok(RcvdBuffer+7,",");
					if(RcvdData <=0)
						TestResult =0;
					strcpy(stRecordInfo.SerialNumber,RcvdData);

					Temp = RcvdBuffer[19] -'0';
					stRecordInfo.Date.Day =Temp << 4;
					stRecordInfo.Date.Day |=(RcvdBuffer[20]-'0');
					if(stRecordInfo.Date.Day <=0)
						TestResult =0;
					Temp = RcvdBuffer[21] -'0';
					stRecordInfo.Date.Month =Temp << 4;
					stRecordInfo.Date.Month |=(RcvdBuffer[22]-'0');
					if(stRecordInfo.Date.Month <=0)
						TestResult =0;
					
					Temp = RcvdBuffer[23] -'0';
					stRecordInfo.Date.Year =Temp << 12;
					Temp = RcvdBuffer[24] -'0';
					stRecordInfo.Date.Year |=Temp << 8;
					Temp = RcvdBuffer[25] -'0';
					stRecordInfo.Date.Year |=Temp << 4;
					stRecordInfo.Date.Year |=(RcvdBuffer[26]-'0');
					if(stRecordInfo.Date.Year <=2005)
						TestResult =0;
					Temp = RcvdBuffer[27] -'0';
					stRecordInfo.Time.Hours =Temp << 4;
					stRecordInfo.Time.Hours |=(RcvdBuffer[28]-'0');

					Temp = RcvdBuffer[29] -'0';
					stRecordInfo.Time.Minutes =Temp << 4;
					stRecordInfo.Time.Minutes |=(RcvdBuffer[30]-'0');

					Temp = RcvdBuffer[31] -'0';
					stRecordInfo.Time.Seconds =Temp << 4;
					stRecordInfo.Time.Seconds |=(RcvdBuffer[32]-'0');
					TransmitChksum =0;
					BatteryIndex =1;
					while(BatteryIndex <MAX_RECORDS)
					{
						//Read record information
						FLASH_Read( DATA_RECORD_ADDRESS(BatteryIndex),(char*) &stRecordReadInfo,sizeof(struct Record));
						//Compare serial number
						if(strcmp(stRecordInfo.SerialNumber,stRecordReadInfo.SerialNumber) == 0) 
						{
						//compare Date
							if(stRecordInfo.Date.Day == stRecordReadInfo.Date.Day && stRecordInfo.Date.Month == stRecordReadInfo.Date.Month && stRecordInfo.Date.Year == stRecordReadInfo.Date.Year)
							{
							//compare hours
								if(stRecordInfo.Time.Hours = stRecordReadInfo.Time.Hours && stRecordInfo.Time.Minutes == stRecordReadInfo.Time.Minutes && stRecordInfo.Time.Seconds == stRecordReadInfo.Time.Seconds)
								{

									for(BatteryIndex =1;BatteryIndex <=10;BatteryIndex++)
									{
											FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
											if((strncmp(ReadRelayValues.BatteryName, stRecordReadInfo.Battery1Type, 7)) == 0)
												break;
									}
									
									if(BatteryIndex > 10)
									{
										ReadRelayValues.K1StartTime = 0;
										ReadRelayValues.K1StopTime = 0;
										ReadRelayValues.K2StartTime = 0;
										ReadRelayValues.K2StopTime = 0;
										ReadRelayValues.K3StartTime = 0;
										ReadRelayValues.K3StopTime = 0;
										ReadRelayValues.PRStart = 0;
										ReadRelayValues.PRDuration = 0;
										ReadRelayValues.PRCount = 0;
										ReadRelayValues.Interval = 0;
									}


									Len=sprintf(TxdBuffer,"F%s,%s,",stRecordReadInfo.SerialNumber,stRecordReadInfo.Battery1Type);
									Len+=sprintf(TxdBuffer+Len,"%02x%02x%04x",(unsigned int)stRecordReadInfo.Date.Day,(unsigned int)stRecordReadInfo.Date.Month,(unsigned int)stRecordReadInfo.Date.Year);
									Len+=sprintf(TxdBuffer+Len,"%02x%02x%02x,",(unsigned int)stRecordReadInfo.Time.Hours,(unsigned int)stRecordReadInfo.Time.Minutes,(unsigned int)stRecordReadInfo.Time.Seconds);
									Len+=sprintf(TxdBuffer+Len,"%02d,%04d,%04ld,",(unsigned int)stRecordReadInfo.TestCondition,stRecordReadInfo.IgnitionDuration,stRecordReadInfo.Testduration);
									Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,",(unsigned long)ReadRelayValues.K1StartTime,(unsigned long)ReadRelayValues.K1StopTime);
									Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,",(unsigned long)ReadRelayValues.K2StartTime,(unsigned long)ReadRelayValues.K2StopTime);
									Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,",(unsigned long)ReadRelayValues.K3StartTime,(unsigned long)ReadRelayValues.K3StopTime);
									Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,%02d,%04ld,",(unsigned long)ReadRelayValues.PRStart,(unsigned long)ReadRelayValues.PRDuration,ReadRelayValues.PRCount,(unsigned long)ReadRelayValues.Interval);
					//added on 09 august 06  
								
									Len+=sprintf(TxdBuffer+Len,"%02d,%02d,",(unsigned int)stRecordReadInfo.FirstSection,(unsigned int)stRecordReadInfo.SecondSection);

					
										//Modified by Madhu on 18072006 to resolve the count/interval problem
									MsgLength = strlen(TxdBuffer)+stRecordReadInfo.NoOfBytes*2;
									PutCharOnRS232(SOM);
									PutCharOnRS232((MsgLength/100000)+'0');
									MsgLength %=100000;
									PutCharOnRS232((MsgLength/10000)+'0');
									MsgLength %=10000;
									PutCharOnRS232((MsgLength/1000)+'0');
									MsgLength %=1000;
									PutCharOnRS232((MsgLength/100)+'0');
									MsgLength %=100;
									PutCharOnRS232((MsgLength/10)+'0');
									MsgLength %=10;
									PutCharOnRS232(MsgLength+'0');
									TransmitChksum ^= Checksum(TxdBuffer);
									PutStringOnRS232(TxdBuffer);
									Len=ReadFileFromFlash(stRecordReadInfo.FileName,stRecordReadInfo.NoOfBytes*2);
									if(Len == -1)
										PutStringOnRS232("Insert Flash Card");
									BatteryIndex =1;
									TestResult =1;
									break;
								}
							}
						}
						BatteryIndex++;
					}
					PutCharOnRS232(TransmitChksum);
					if(!TestResult)
						PutStringOnRS232(NACK); //nack
						
			break;
		case 'D' : // DELETE TEST

					RcvdData = strtok(RcvdBuffer+7,",");
					strcpy(stRecordInfo.SerialNumber,RcvdData);

					Temp = RcvdBuffer[19] -'0';
					stRecordInfo.Date.Day =Temp << 4;
					stRecordInfo.Date.Day |=(RcvdBuffer[20]-'0');

					Temp = RcvdBuffer[21] -'0';
					stRecordInfo.Date.Month =Temp << 4;
					stRecordInfo.Date.Month |=(RcvdBuffer[22]-'0');

					Temp = RcvdBuffer[23] -'0';
					stRecordInfo.Date.Year =Temp << 12;
					Temp = RcvdBuffer[24] -'0';
					stRecordInfo.Date.Year |=Temp << 8;
					Temp = RcvdBuffer[25] -'0';
					stRecordInfo.Date.Year |=Temp << 4;
					stRecordInfo.Date.Year |=(RcvdBuffer[26]-'0');

					Temp = RcvdBuffer[27] -'0';
					stRecordInfo.Time.Hours =Temp << 4;
					stRecordInfo.Time.Hours |=(RcvdBuffer[28]-'0');

					Temp = RcvdBuffer[29] -'0';
					stRecordInfo.Time.Minutes =Temp << 4;
					stRecordInfo.Time.Minutes |=(RcvdBuffer[30]-'0');

					Temp = RcvdBuffer[31] -'0';
					stRecordInfo.Time.Seconds =Temp << 4;
					stRecordInfo.Time.Seconds |=(RcvdBuffer[32]-'0');
					BatteryIndex =1;
					while(BatteryIndex <MAX_RECORDS)
					{
						FLASH_Read( DATA_RECORD_ADDRESS(BatteryIndex),(char*) &stRecordReadInfo,sizeof(struct Record));
						if(strcmp(stRecordInfo.SerialNumber,stRecordReadInfo.SerialNumber) == 0) 
						{
							if(stRecordInfo.Date.Day == stRecordReadInfo.Date.Day && stRecordInfo.Date.Month == stRecordReadInfo.Date.Month && stRecordInfo.Date.Year == stRecordReadInfo.Date.Year)
							{
								if(stRecordInfo.Time.Hours == stRecordReadInfo.Time.Hours && stRecordInfo.Time.Minutes == stRecordReadInfo.Time.Minutes && stRecordInfo.Time.Seconds == stRecordReadInfo.Time.Seconds)
								{
										EraseRecords(stRecordReadInfo.FileName,1);
										TestResult = 1;
										break;
								}
							}
						}
						BatteryIndex++;
					}
					if(TestResult)
						PutStringOnRS232(ACK); //ack
					else
						PutStringOnRS232(NACK); //nack
			break;
		case 'R' :	// Transmit Record Status
					sprintf(TxdBuffer,"R%02d,%02d",(unsigned int)RecordsUsed,(unsigned int)FreeRecords);
					SendDataToRS232(TxdBuffer);
		break;
		case 'C' : // Set Test Configuration
					RcvdData = strtok(RcvdBuffer+7,",");
					strcpy(stRecordInfo.Battery1Type,RcvdData);
					strcpy(WriteRelayValues.BatteryName,RcvdData);
					for(BatteryIndex =1;BatteryIndex <=10;BatteryIndex++)
					{
							FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
							if((strncmp(ReadRelayValues.BatteryName,RcvdData,7)) == 0)
								break;
					}
					if(BatteryIndex >10)
					{
						PutStringOnRS232(NACK);	
						return 0;
					}
					RcvdData = strtok(NULL,","); 
					stRecordInfo.IgnitionDuration = atoi(RcvdData);
					if(stRecordInfo.IgnitionDuration >0 && stRecordInfo.IgnitionDuration <=999)
					{
						WriteRelayValues.IgnitionDuration = stRecordInfo.IgnitionDuration;
						RcvdData = strtok(NULL,",");
						stRecordInfo.Testduration =(atoi(RcvdData));
						if(stRecordInfo.Testduration >0 && stRecordInfo.Testduration <=120)
						{
							WriteRelayValues.Testduration = stRecordInfo.Testduration;
							RcvdData = strtok(NULL,",");
							WriteRelayValues.K1StartTime = atol(RcvdData);
							RcvdData = strtok(NULL,",");
							WriteRelayValues.K1StopTime = atol(RcvdData);
							if(WriteRelayValues.K1StartTime >=MIN_K1_START_TIME && WriteRelayValues.K1StartTime <=MAX_K1_START_TIME)
							{
								RcvdData = strtok(NULL,",");
								if(RcvdData == '\0')
									return 0;
								WriteRelayValues.K2StartTime = atol(RcvdData);
								RcvdData = strtok(NULL,",");
								if(RcvdData == '\0')
									return 0;
								WriteRelayValues.K2StopTime = atol(RcvdData);
								if(WriteRelayValues.K2StartTime >=MIN_K2_START_TIME && WriteRelayValues.K2StartTime <=MAX_K2_START_TIME)
								{							
									RcvdData = strtok(NULL,",");
									WriteRelayValues.K3StartTime = atol(RcvdData);
									RcvdData = strtok(NULL,",");
									WriteRelayValues.K3StopTime = atol(RcvdData);
									if(WriteRelayValues.K3StartTime >=MIN_K3_START_TIME && WriteRelayValues.K3StartTime <=MAX_K3_START_TIME)
									{
										RcvdData = strtok(NULL,",");
										WriteRelayValues.PRStart = atol(RcvdData);
										RcvdData = strtok(NULL,",");
										WriteRelayValues.PRDuration = atol(RcvdData);
										RcvdData = strtok(NULL,",");
										WriteRelayValues.PRCount = atoi(RcvdData);
										RcvdData = strtok(NULL,",");
										WriteRelayValues.Interval = atol(RcvdData);
										if(WriteRelayValues.Interval >=MIN_PR_INTERVAL_TIME && WriteRelayValues.Interval <=MAX_PR_INTERVAL_TIME)
										{
											if((WriteRelayValues.PRStart+(WriteRelayValues.PRCount*WriteRelayValues.Interval) - WriteRelayValues.PRDuration) <120000)
											{
												RcvdData = strtok(NULL,",");
												if(RcvdData != NULL)  //section selection
												{
													WriteRelayValues.FirstSection = atoi(RcvdData);
												}

												RcvdData = strtok(NULL,",");
												if(RcvdData != NULL)
												{
													WriteRelayValues.SecondSection = atoi(RcvdData);	
												}
//added on 17august 6.18
									//			WriteRelayValues.Section1 = 0x01;
									//			WriteRelayValues.Section1 = 0x02;
												FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
												FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));
												PutStringOnRS232(ACK);
											}
											else    //testduration <12000
												PutStringOnRS232(NACK);	
										}
										else //interval condition
											PutStringOnRS232(NACK);	
									}
									else //k3 Relay condition
										PutStringOnRS232(NACK);	
								}
								else //k2 relay condition
									PutStringOnRS232(NACK);	
							}
							else  //k1 condition
								PutStringOnRS232(NACK);
						}
						else //test duration
							PutStringOnRS232(NACK);
					}
					else //ignition duration
						PutStringOnRS232(NACK);
		break;
		case 'T' :  // Start Test
					Remote =1;
					/*****************************************/ //added on 13 july
					FileName= ReadDirectory();
					if(FileName == -1)
					{
						PutStringOnRS232("There are No Records Available");
						return 0;
					}
					RcvdData = strtok(RcvdBuffer+7,",");
					strcpy(stRecordInfo.Battery1Type,RcvdData);
//added
					for(BatteryIndex =1;BatteryIndex <=10;BatteryIndex++)
					{
							FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
							if((strncmp(ReadRelayValues.BatteryName,RcvdData,7)) == 0)
								break;
					}
					RcvdData = strtok(NULL,","); 
					if(*RcvdData !='\0')
						strcpy(stRecordInfo.SerialNumber,RcvdData);
					else
					{
						PutStringOnRS232(NACK);
						return 0;
					}
					RcvdData = strtok(NULL,","); 
					stRecordInfo.TestCondition = (unsigned char)atoi(RcvdData);
					if(stRecordInfo.TestCondition >0 && stRecordInfo.TestCondition <=20)
					{
						FLASH_Read  (RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
						stRecordInfo.Testduration=ReadRelayValues.Testduration;
						stRecordInfo.IgnitionDuration=ReadRelayValues.IgnitionDuration;
						stRecordInfo.FirstSection = ReadRelayValues.FirstSection;
						stRecordInfo.SecondSection = ReadRelayValues.SecondSection;

						if(stRecordInfo.Testduration*1000 <=120000)
						{
							LCDInstWrite(LCD_BLINKING_OFF);
							PutStringOnRS232(ACK);
							TestProcessing();
							PutStringOnRS232(TESTOK);
							Remote =0;
						}
						else
							PutStringOnRS232("Invalid TestDuration");
							
					}
					else
						PutStringOnRS232(NACK);
		break;
		case 'K' : //  Set Relay  Configurations
					RcvdData = strtok(RcvdBuffer+7,",");
					strcpy(stRecordInfo.Battery1Type,RcvdData);

					for(BatteryIndex =1;BatteryIndex <=10;BatteryIndex++)
					{
							FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
							if((strncmp(ReadRelayValues.BatteryName,RcvdData,7)) == 0)
								break;
					}
				//	FLASH_Read(RELAY_DATA_ADDRESS(BatType), (char*)&ReadRelayValues, sizeof(struct Relay));
					Len=sprintf(TxdBuffer,"K%7s,",stRecordInfo.Battery1Type);
					Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,",(unsigned long)ReadRelayValues.IgnitionDuration,(unsigned long)ReadRelayValues.Testduration);
					Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,",(unsigned long)ReadRelayValues.K1StartTime,(unsigned long)ReadRelayValues.K1StopTime);
					Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,",(unsigned long)ReadRelayValues.K2StartTime,(unsigned long)ReadRelayValues.K2StopTime);
					Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,",(unsigned long)ReadRelayValues.K3StartTime,(unsigned long)ReadRelayValues.K3StopTime);
					Len+=sprintf(TxdBuffer+Len,"%04ld,%04ld,%02d,%04ld,",(unsigned long)ReadRelayValues.PRStart,(unsigned long)ReadRelayValues.PRDuration,ReadRelayValues.PRCount,(unsigned long)ReadRelayValues.Interval);
					Len+=sprintf(TxdBuffer+Len,"%02d,%02d",(unsigned int)ReadRelayValues.FirstSection,(unsigned int)ReadRelayValues.SecondSection);
					SendDataToRS232(TxdBuffer);
		break;
		case 'N': //Add new battery 
					RcvdBuffer[14] = '\0';
					strcpy(WriteRelayValues.BatteryName,&RcvdBuffer[7]);
					if((strncmp(WriteRelayValues.BatteryName,"ECT-",4)) == 0)
					{
						for(BatteryIndex=1;BatteryIndex<=10;BatteryIndex++)
						{
							FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
							if((strcmp(ReadRelayValues.BatteryName,WriteRelayValues.BatteryName)) == 0)
								break;
							if((strncmp(ReadRelayValues.BatteryName,"ECT-",4)) == 0)
								continue;
							else
							{
								FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
								memset(&WriteRelayValues,0,sizeof(struct Relay));
								WriteRelayValues.FirstSection = 0x01;
								strcpy(WriteRelayValues.BatteryName,&RcvdBuffer[7]);
								FLASH_Write(RELAY_DATA_ADDRESS(BatteryIndex),(char*)&WriteRelayValues,sizeof(WriteRelayValues));
								PutStringOnRS232(ACK);
								return 0;
							}
						}			
					}
					PutStringOnRS232(NACK);
		break;
		case 'U':  //set active batteries
					RcvdData = strtok(RcvdBuffer+7,",");
					if(RcvdData != '\0')
						strcpy(ActiveBat1Info,RcvdData);
					RcvdData = strtok(NULL,",");
					if(RcvdData != '\0')
						strcpy(ActiveBat2Info,RcvdData);
					FLASH_PageErase(ACTIVE_BATTERIES_INFO);
					FLASH_Write(ACTIVE_BATTERY1_INFO,ActiveBat1Info,strlen(ActiveBat1Info)+1);
					FLASH_Write(ACTIVE_BATTERY2_INFO,ActiveBat2Info,strlen(ActiveBat2Info)+1);
					PutStringOnRS232(ACK);

		break;
		case 'V':  //Get Active Batteries
					Len =0;
					GetActiveBatteries();
					if((strncmp(ActiveBat2Info,"ECT-",4)) != 0)
					{
						PutStringOnRS232(NACK);
						return 0;
					}
					if((strncmp(ActiveBat1Info,"ECT-",4)) != 0)
					{
						PutStringOnRS232(NACK);
						return 0;
					}
					Temp = 0;
					for(BatteryIndex=1;BatteryIndex<=10;BatteryIndex++)
					{
						FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
						if((strncmp(ReadRelayValues.BatteryName,ActiveBat1Info,7)) == 0)
								Temp |= 0x01;
						if((strncmp(ReadRelayValues.BatteryName,ActiveBat2Info,7)) == 0)
								Temp |= 0x02;
					}
					if(Temp == 0x03)
					{					
					
						Len=sprintf(TxdBuffer,"V%7s,",ActiveBat1Info);
						Len=sprintf(TxdBuffer+Len,"%7s",ActiveBat2Info);
						SendDataToRS232(TxdBuffer);
					}
					else
				 		PutStringOnRS232(NACK);
					
		break;
		case 'B':  //Get Batteries list
			Len =1;
			TxdBuffer[0] = 'B';
			for(BatteryIndex=1;BatteryIndex<=10;BatteryIndex++)
			{
				FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
				if((strncmp(ReadRelayValues.BatteryName,"ECT-",4)) == 0)
					Len+=sprintf(TxdBuffer+Len,"%7s,",ReadRelayValues.BatteryName);
			}
			TxdBuffer[Len-1]= '\0';
			if(Len >1)
				SendDataToRS232(TxdBuffer);
			else 
				PutStringOnRS232(NACK);
		break;
		case 'E':	//Delete Battery
			TxdBuffer[0] = '\0';
			strncat(TxdBuffer,RcvdBuffer+7,7);
			for(BatteryIndex=1;BatteryIndex<=10;BatteryIndex++)
			{
				FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
				if((strcmp(ReadRelayValues.BatteryName,TxdBuffer)) == 0)
				{
					FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
					PutStringOnRS232(ACK);
					return 0;
				}
			}
			PutStringOnRS232(NACK);
		break;
		case 'M':	//Rename Battery
			RcvdData = strtok(RcvdBuffer+7,",");
			if(RcvdData != '\0')
				strcpy(TxdBuffer,RcvdData);
			if((strncmp(TxdBuffer,"ECT-",4)) != 0)
			{
				PutStringOnRS232(NACK);
				return 0;
			}

			RcvdData = strtok(NULL,",");
			if(RcvdData != '\0')
			{
				for(BatteryIndex=1;BatteryIndex<=10;BatteryIndex++)
				{
					FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
					if((strcmp(ReadRelayValues.BatteryName,TxdBuffer)) == 0)
					{
						FLASH_PageErase(RELAY_DATA_ADDRESS(BatteryIndex));
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
						WriteRelayValues.IgnitionDuration = ReadRelayValues.IgnitionDuration;
						WriteRelayValues.Testduration = ReadRelayValues.Testduration;
						WriteRelayValues.FirstSection = ReadRelayValues.FirstSection;
						WriteRelayValues.SecondSection = ReadRelayValues.SecondSection;

						strcpy(WriteRelayValues.BatteryName,RcvdData);
						if((strcmp(TxdBuffer,ActiveBat1Info)) == 0)
						{
							strcpy(ActiveBat1Info,WriteRelayValues.BatteryName);
							FLASH_PageErase(ACTIVE_BATTERIES_INFO);
							FLASH_Write(ACTIVE_BATTERY1_INFO,ActiveBat1Info,strlen(ActiveBat1Info)+1);
							FLASH_Write(ACTIVE_BATTERY2_INFO,ActiveBat2Info,strlen(ActiveBat2Info)+1);
						}
						if((strcmp(TxdBuffer,ActiveBat2Info)) == 0)
						{
							strcpy(ActiveBat2Info,WriteRelayValues.BatteryName);
							FLASH_PageErase(ACTIVE_BATTERIES_INFO);
							FLASH_Write(ACTIVE_BATTERY1_INFO,ActiveBat1Info,strlen(ActiveBat1Info)+1);
							FLASH_Write(ACTIVE_BATTERY2_INFO,ActiveBat2Info,strlen(ActiveBat2Info)+1);
						}
				
						FLASH_Write (RELAY_DATA_ADDRESS(BatteryIndex), (char*) &WriteRelayValues,sizeof(struct Relay));

					//	FLASH_Read(RELAY_DATA_ADDRESS(BatteryIndex), (char*)&ReadRelayValues, sizeof(struct Relay));
						PutStringOnRS232(ACK);
						return 0;
					}
				}
			}
			PutStringOnRS232(NACK);

		break;
		default:		//Error Message 
			PutStringOnRS232(NACK); //nack
		break;
	}

}
//get active batteries
void GetActiveBatteries()
{
	FLASH_Read(ACTIVE_BATTERY1_INFO,ActiveBat1Info,sizeof(ActiveBat1Info));
	FLASH_Read(ACTIVE_BATTERY2_INFO,ActiveBat2Info,sizeof(ActiveBat2Info));
}


unsigned char Checksum(unsigned char *cbuffer) 
{
    unsigned char csum; 
    csum =0;
	while(*cbuffer !='\0')
        csum^=*cbuffer++;
    return csum;
}


void PutCharOnRS232(unsigned char value)
{
	char SFRPAGE_SAVE = SFRPAGE;
	SFRPAGE = UART0_PAGE;
	TI0 = 0;
	SBUF0 = value;

	while(TI0 != 1);
	TI0 = 0;
	SFRPAGE = SFRPAGE_SAVE;
}

void SendDataToRS232(unsigned char *TxBuffer)
{
	 unsigned char MsgLength;
	 unsigned char xdata TempBuf[10];

	 PutCharOnRS232(SOM);
	 MsgLength = strlen(TxBuffer);
	 sprintf(TempBuf,"%06d",(unsigned int)MsgLength);
 	 PutStringOnRS232(TempBuf);
 	 PutStringOnRS232(TxBuffer);
	 PutCharOnRS232(Checksum(TxBuffer));
}



void ReadRecordLists()
{
	unsigned char Records;
	unsigned long idata MsgLength=0;
	unsigned char ChkSum=0;
    PutCharOnRS232(SOM);
    
	for(Records=1;Records<MAX_RECORDS;Records++)
	{
		FLASH_Read( DATA_RECORD_ADDRESS(Records),(char*) &stRecordReadInfo,sizeof(struct Record));
		if(strncmp(stRecordReadInfo.Battery1Type,"ECT-",4) == 0 )
		{
			stRecordReadInfo.SerialNumber[11] = '\0';
			MsgLength+=sprintf(TxdBuffer,"%11s,",stRecordReadInfo.SerialNumber);

			MsgLength+=sprintf(TxdBuffer,"%02x%02x%04x",(unsigned int)stRecordReadInfo.Date.Day,(unsigned int)stRecordReadInfo.Date.Month,(unsigned int)stRecordReadInfo.Date.Year);
			MsgLength+=sprintf(TxdBuffer,"%02x%02x%02x,",(unsigned int)stRecordReadInfo.Time.Hours,(unsigned int)stRecordReadInfo.Time.Minutes,(unsigned int)stRecordReadInfo.Time.Seconds);
		}
	}
	MsgLength++; //for adding L to msgelngth
	sprintf(TxdBuffer,"%06d",(unsigned int)MsgLength);
	if(MsgLength >1)
	{
		PutStringOnRS232(TxdBuffer);
		PutCharOnRS232('L');
		MsgLength =0;
		for(Records=1;Records<MAX_RECORDS;Records++)
		{
			FLASH_Read( DATA_RECORD_ADDRESS(Records),(char*) &stRecordReadInfo,sizeof(struct Record));
			if(strncmp(stRecordReadInfo.Battery1Type,"ECT-",4) == 0)
			{
				stRecordReadInfo.SerialNumber[11] = '\0';
				MsgLength=sprintf(TxdBuffer,"%11s,",stRecordReadInfo.SerialNumber);
				MsgLength+=sprintf(TxdBuffer+MsgLength,"%02x%02x%04x",(unsigned int)stRecordReadInfo.Date.Day,(unsigned int)stRecordReadInfo.Date.Month,(unsigned int)stRecordReadInfo.Date.Year);
				MsgLength+=sprintf(TxdBuffer+MsgLength,"%02x%02x%02x,",(unsigned int)stRecordReadInfo.Time.Hours,(unsigned int)stRecordReadInfo.Time.Minutes,(unsigned int)stRecordReadInfo.Time.Seconds);
				ChkSum ^=Checksum(TxdBuffer);
				PutStringOnRS232(TxdBuffer);
				MsgLength=0;
			}
		}
		PutCharOnRS232(ChkSum ^'L');

	}
	else
		PutStringOnRS232(NACK);
}		
