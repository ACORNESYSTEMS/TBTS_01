//Records header file

#ifndef RECORD_H
#define RECORD_H

struct Record
{
	unsigned char SerialNumber[12];
	unsigned char Battery1Type[12];
	unsigned char TestCondition;
	unsigned char FileName;
	unsigned long NoOfBytes;
	unsigned long Testduration;
	unsigned int IgnitionDuration;

	struct
	{
		unsigned char Seconds;
		unsigned char Minutes;
		unsigned char Hours;
	}Time;
	struct
	{
		unsigned int Year;
		unsigned char Day;
		unsigned char Month;
	}Date;
	unsigned char FirstSection;
	unsigned char SecondSection;
};

struct Relay
{
	unsigned long K1StartTime;
	unsigned long K1StopTime;
	unsigned long K2StartTime;
	unsigned long K2StopTime;
	unsigned long K3StartTime;
	unsigned long K3StopTime;
	unsigned long PRStart;
	unsigned long PRDuration;
	unsigned int PRCount;
	unsigned int IgnitionDuration;
	unsigned long Interval;
	unsigned long Testduration;
	unsigned char BatteryName[12];
	unsigned char FirstSection;
	unsigned char SecondSection;
	
};

#endif
//unsigned char xdata BatteryInfo[10][12];


