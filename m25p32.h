
#ifndef _M25P32__H
#define _M25P32__H


#define SPI_DUMMY_DATA 			0x00
#define SPI_FLASH_INS_RDID		0x9F  //READ IDENTIFICATION 
#define SPI_FLASH_INS_RDSR		0x05  // READ STATUS REGISTER
#define SPI_FLASH_INS_WREN		0x06  //WRITE ENABLE
#define SPI_FLASH_INS_WRDI		0x04  //WRITE DISABLE
#define SPI_FLASH_INS_READ		0x03  //READ DATA BYTES
#define SPI_FLASH_INS_PP		0x02  //PAGE PROGRAM
#define SPI_FLASH_INS_SE		0xD8  //SECTOR ERASE
#define SPI_FLASH_INS_BE   		0xC7  // bulk erase

#define DEVICE_ID				0x2016
#define MANUFACTURER_ID 		0x20

#define CHIP0  					0x00
#define CHIP1  					0x01
#define CHIP2  					0x02
#define CHIP3  					0x03


struct stFlash
{
	unsigned long Address;
	unsigned char ChipSelect;
	unsigned char SectorNumber;
	unsigned long  NoOfElements;
	unsigned long Offset;
	unsigned char idata *Buffer;
};
#endif

