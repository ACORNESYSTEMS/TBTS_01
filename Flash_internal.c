//#include <c8051f120.h>
/*
File Name :Flash_internal.c
Description : Internla Flash Read,Write and Erase Routines
*/
#include <functions.h>

//-----------------------------------------------------------------------------
// Type Definitions
//-----------------------------------------------------------------------------
typedef union ULong 
{ 												// Byte addressable unsigned long
	unsigned long ULong;
	unsigned int Int[2];
	unsigned char Char[4];
} ULong;


#define FLASH_PAGESIZE 1024 					// Number of bytes in each FLASH page

#define COBANK 	0xF0 							// Bit mask for the high nibble of PSBANK
#define COBANK0 0x00 							// These macros define the bit mask values
#define COBANK1 0x10 							// for the PSBANK register used for
#define COBANK2 0x20 							// selecting COBANK. COBANK should always
#define COBANK3 0x30 							// be cleared then OR-Equaled (|=) with


//-----------------------------------------------------------------------------
// FLASH_PageErase
//-----------------------------------------------------------------------------
//
// This function erases the FLASH page containing <addr>.
//

void FLASH_PageErase (unsigned long addr)
{
	char SFRPAGE_SAVE = SFRPAGE; 				// Preserve current SFR page
	char PSBANK_SAVE = PSBANK; 					// Preserve current code bank
	bit EA_SAVE = EA; 							// Preserve interrupt state
	ULong temp_addr; 							// Temporary ULong
	char xdata * idata pwrite; 					// FLASH write/erase pointer
	temp_addr.ULong = addr; 					// copy <addr> to a byte addressable

// Extract address information from <addr>
	pwrite = (char xdata *) temp_addr.Int[1];
// Extract code bank information from <addr>
	PSBANK &= ~COBANK; 							// Clear the COBANK bits

	if( temp_addr.Char[1] == 0x00)
	{ 											// If the address is less than
												// 0x10000, the Common area and
		PSBANK |= COBANK1; 						// Bank1 provide a 64KB linear
												// address space
	} 
	else 
	{ 											// Else, Bank2 and Bank3 provide
												// a 64KB linear address space
		if (temp_addr.Char[2] & 0x80)
		{ 										// If bit 15 of the address is
												// a '1’, then the operation should
			PSBANK |= COBANK3; 					// target Bank3, else target Bank2
		} 
		else 
		{
			PSBANK |= COBANK2;
			temp_addr.Char[2] |= 0x80;
			pwrite = (char xdata *) temp_addr.Int[1];
		}
	}
	SFRPAGE = LEGACY_PAGE;
	EA = 0; 									// Disable interrupts
	FLSCL |= 0x01; 								// Enable FLASH writes/erases
	PSCTL = 0x03; 								// MOVX erases FLASH page
	*pwrite = 0; 								// Initiate FLASH page erase
	FLSCL &= ~0x01; 							// Disable FLASH writes/erases
	PSCTL = 0x00; 								// MOVX targets XRAM
	EA = EA_SAVE; 								// Restore interrupt state
	PSBANK = PSBANK_SAVE; 						// Restore current code bank
	SFRPAGE = SFRPAGE_SAVE; 					// Restore SFR page
}

//-----------------------------------------------------------------------------
// FLASH_Write
//-----------------------------------------------------------------------------
//
// This routine copies <numbytes> from <src> to the FLASH addressed by <dest>.
//
void FLASH_Write (unsigned long dest, char* src, unsigned int numbytes)
{
	unsigned int i; 							// Software Counter
	for (i = 0; i < numbytes; i++) 
	{
		FLASH_ByteWrite( dest++, *src++);
	}
}
//-----------------------------------------------------------------------------
// FLASH_ByteWrite
//-----------------------------------------------------------------------------
//
// This routine writes <dat> to the FLASH byte addressed by <dest>.
//
void FLASH_ByteWrite (unsigned long dest, char dat)
{
	char SFRPAGE_SAVE = SFRPAGE; 				// Preserve current SFR page
	char PSBANK_SAVE = PSBANK; 					// Preserve current code bank
	bit EA_SAVE = EA; 							// Preserve interrupt state
	ULong temp_dest; 							// Temporary ULong
	char xdata * idata pwrite; 					// FLASH write/erase pointer
	temp_dest.ULong = dest; 					// copy <dest> to a byte
							// addressable unsigned long
							// Check if data byte being written is 0xFF
							// There is no need to write 0xFF to FLASH since erased
							// FLASH defaults to 0xFF.
							// Extract address information from <dest>
	pwrite = (char xdata *) temp_dest.Int[1];
	// Extract code bank information from <addr>
	PSBANK &= ~COBANK; 							// Clear the COBANK bits
	if( temp_dest.Char[1] == 0x00)
	{ 											// If the address is less than
												// 0x10000, the Common area and
		PSBANK |= COBANK1; 						// Bank1 provide a 64KB linear
												// address space
	} 
	else 
	{ 
					// Else, Bank2 and Bank3 provide
					// a 64KB linear address space
		if (temp_dest.Char[2] & 0x80)
		{			// If bit 15 of the address is
					// a '1’, then the operation should
			PSBANK |= COBANK3; // target Bank3, else target Bank2
		} 
		else 
		{
			PSBANK |= COBANK2;
			temp_dest.Char[2] |= 0x80;
			pwrite = (char xdata *) temp_dest.Int[1];
		}
	}
	SFRPAGE = LEGACY_PAGE;
	EA = 0; 									// Disable interrupts
	FLSCL |= 0x01; 								// Enable FLASH writes/erases
	PSCTL = 0x01; 								// MOVX writes FLASH byte
	*pwrite = dat; 								// Write FLASH byte
	FLSCL &= ~0x01; 							// Disable FLASH writes/erases
	PSCTL = 0x00; 								// MOVX targets XRAM
	
	EA = EA_SAVE; 								// Restore interrupt state
	PSBANK = PSBANK_SAVE; 						// Restore current code bank
	SFRPAGE = SFRPAGE_SAVE; 					// Restore SFR page
}
//-----------------------------------------------------------------------------
// FLASH_Read
//-----------------------------------------------------------------------------
//
// This routine copies <numbytes> from FLASH addressed by <src> to <dest>.
//
//void FLASH_Read ( char* dest, unsigned long src, unsigned int numbytes)
void FLASH_Read ( unsigned long src, char* dest, unsigned int numbytes)
{
	unsigned int i; 							// Software Counter
	for (i = 0; i < numbytes; i++) 
	{
		*dest++ = FLASH_ByteRead(src++);
	}
}
//-----------------------------------------------------------------------------
// FLASH_ByteRead
//-----------------------------------------------------------------------------
//
// This routine returns to the value of the FLASH byte addressed by <addr>.
//

unsigned char FLASH_ByteRead (unsigned long addr)
{
	char SFRPAGE_SAVE = SFRPAGE; 				// Preserve current SFR page
	char PSBANK_SAVE = PSBANK; 					// Preserve current code bank
	ULong temp_addr; 							// Temporary ULong
	char temp_char; 							// Temporary char
	char code * idata pread; 					// FLASH read pointer
	temp_addr.ULong = addr; 					// copy <addr> to a byte addressable
	// unsigned long
	// Extract address information from <addr>
	pread = (char code *) temp_addr.Int[1];
	// Extract code bank information from <addr>
	PSBANK &= ~COBANK; // Clear the COBANK bits
	if( temp_addr.Char[1] == 0x00)
	{ 			// If the address is less than
				// 0x10000, the Common area and
		PSBANK |= COBANK1; // Bank1 provide a 64KB linear
				// address space
	} 
	else 
	{ 			// Else, Bank2 and Bank3 provide
				// a 64KB linear address space
		if (temp_addr.Char[2] & 0x80)
		{ 		// If bit 15 of the address is
				// a '1’, then the operation should
			PSBANK |= COBANK3; // target Bank3, else target Bank2
		} 
		else 
		{
			PSBANK |= COBANK2;
			temp_addr.Char[2] |= 0x80;
			pread = (char code *) temp_addr.Int[1];
		}
	}
	temp_char = *pread; 						// Read FLASH byte
	PSBANK = PSBANK_SAVE; 						// Restore current code bank
	SFRPAGE = SFRPAGE_SAVE; 					// Restore SFR page
	return temp_char;
}


	