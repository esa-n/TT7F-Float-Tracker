/*
GPS_NMEA_error_bitfield
	Bit 7 0b10100000 not used - DL-FLDIGI doesn't show ASCII-8 characters																			
	Bit 6 0b01100000 NAVmode error (!6)																											'`'
	Bit 5 0b00100000 default message - NO ERROR (for all ERRORS Bit 5 changes to 0 (0b01011111) - for the character to be shown in DL-FLDIGI)	' '
	Bit 4 0b00110000 no FIX																														'0'
	Bit 3 0b00101000 UBX response timeout																										'('
	Bit 2 0b00100100 GGA checksum error																											'$'
	Bit 1 0b00100010 GGA buffer error																											'"'
	Bit 0 0b00100001 UBLOX response timeout																										'!'

GPS_UBX_error_bitfield
	Bit 7 0b10100000 not used - DL-FLDIGI doesn't show ASCII-8 characters																			
	Bit 6 0b01100000 NAVmode error (!6)																											'`'
	Bit 5 0b00100000 default message - NO ERROR (for all ERRORS Bit 5 changes to 0 (0b01011111) - for the character to be shown in DL-FLDIGI)	' '
	Bit 4 0b00110000 0611 error POWER MODE																										'0'
	Bit 3 0b00101000 0121 error TIME																											'('
	Bit 2 0b00100100 0102 error POSITION DATA / 0107 error POSITION DATA																		'$'
	Bit 1 0b00100010 0106 error FIX, SATS																										'"'
	Bit 0 0b00100001 SSDV get image error																										'!'

	character 'DEL' doesn't show in DL-FLDIGI, needs to be changed ->																			'_'
	character corresponds to ',' which is used in the parser ->																					'L'
	character corresponds to '*' which is used in the parser ->																					'J'
	character corresponds to '$' which is used in the parser ->																					'D'
*/


#ifndef ARM_UBLOX_H
#define ARM_UBLOX_H


#include "stdint.h"


static char RTTY_callsign[] = "TT7S";							// CALLSIGN for the RTTY telemetry string

#define TELEMETRY_SYNC_BYTES	4								// number of '$' characters at the beginning of the telemetry string


/*
	UBX		60
	NMEA	125
*/
#define GPSBUFFER_SIZE			125


/*
	CONFIGURATION.
	Response to these UBX messages is ACK/NACK - 10 bytes.
*/
static uint8_t setGPSonly[28]			= {0xB5, 0x62, 0x6, 0x3E, 0x14, 0x0, 0x0, 0x0, 0xFF, 0x2, 0x0, 0x8, 0xFF, 0x0, 0x1, 0x0, 0x1, 0x0, 0x6, 0x8, 0xFF, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6F, 0xCC};
static uint8_t setNMEAoff[28]	   		= {0xB5, 0x62, 0x06, 0x00, 0x14, 0x00, 0x01, 0x00, 0x00, 0x00, 0xD0, 0x08, 0x00, 0x00, 0x80, 0x25, 0x00, 0x00, 0x07, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0xA0, 0xA9};
static uint8_t setNAVmode[44]			= {0xB5, 0x62, 0x06, 0x24, 0x24, 0x00, 0xFF, 0xFF, 0x06, 0x03, 0x00, 0x00, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x05, 0x00, 0xFA, 0x00, 0xFA, 0x00, 0x64, 0x00, 0x2C, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x52, 0xE8};
static uint8_t setRMCrate0[16]			= {0xB5, 0x62, 0x6, 0x1, 0x8, 0x0, 0xF0, 0x4, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x03, 0x3F};
static uint8_t setVTGrate0[16]			= {0xB5, 0x62, 0x6, 0x1, 0x8, 0x0, 0xF0, 0x5, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x04, 0x46};
static uint8_t setGSArate0[16]			= {0xB5, 0x62, 0x6, 0x1, 0x8, 0x0, 0xF0, 0x2, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x01, 0x31};
static uint8_t setGSVrate0[16]			= {0xB5, 0x62, 0x6, 0x1, 0x8, 0x0, 0xF0, 0x3, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x02, 0x38};
static uint8_t setGLLrate0[16]			= {0xB5, 0x62, 0x6, 0x1, 0x8, 0x0, 0xF0, 0x1, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x00, 0x2A};
static uint8_t setGGArate0[16]			= {0xB5, 0x62, 0x6, 0x1, 0x8, 0x0, 0xF0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0xFF, 0x23};
static uint8_t setGGArate1[16]			= {0xB5, 0x62, 0x6, 0x1, 0x8, 0x0, 0xF0, 0x0, 0x1, 0x1, 0x1, 0x1, 0x1, 0x1, 0x05, 0x38};


/*
	REQUESTS.
	Response to these messages is message specific.
*/

/* NAVIGATION ENGINE					response 44 bytes */
static uint8_t request0624[8]			= {0xB5, 0x62, 0x06, 0x24, 0x00, 0x00, 0x2A, 0x84};

/* POWER MODE settings					response 10 bytes */
static uint8_t request0611[8]			= {0xB5, 0x62, 0x06, 0x11, 0x00, 0x00, 0x17, 0x4B};
	
/* POWER MMANAGEMENT					response 56 bytes */
static uint8_t request063B[8]			= {0xB5, 0x62, 0x06, 0x3B, 0x00, 0x00, 0x41, 0xC9};
	
/* GNSS configuration					response 52 bytes */
static uint8_t request063E[8]			= {0xB5, 0x62, 0x06, 0x3E, 0x00, 0x00, 0x44, 0xD2};
	
/* POSITION DATA						response 36 bytes */
static uint8_t request0102[8]			= {0xB5, 0x62, 0x01, 0x02, 0x00, 0x00, 0x03, 0x0A};

/* FIX, SATS							response 60 bytes */
static uint8_t request0106[8]			= {0xB5, 0x62, 0x01, 0x06, 0x00, 0x00, 0x07, 0x16};

/* TIME, DATE							response 28 bytes */
static uint8_t request0121[8]			= {0xB5, 0x62, 0x01, 0x21, 0x00, 0x00, 0x22, 0x67};

/* EVERYTHING							response 100 bytes (UBLOX 8), 92 bytes (UBLOX 7) */
static uint8_t request0107[8]			= {0xB5, 0x62, 0x01, 0x07, 0x00, 0x00, 0x08, 0x19};

/* GPGGA MAX-7C default					response ~75 bytes */
static uint8_t requestGPGGA[15]			= "$EIGPQ,GGA*27\r\n";

/* GPZDA MAX-7C default					response ~36 bytes */
static uint8_t requestGPZDA[15]			= "$EIGPQ,ZDA*39\r\n";

/* GNGGA MAX-M8Q default				response ~75 bytes */
static uint8_t requestGNGGA[15]			= "$EIGNQ,GGA*39\r\n";

/* GNZDA MAX-M8Q default				response ~36 bytes */
static uint8_t requestGNZDA[15]			= "$EIGNQ,ZDA*27\r\n";


/*
	POWER SAVING.
	Response to these UBX messages is ACK/NACK - 10 bytes.
*/

/* To wake up the module. Wait for 1.5s. */
static uint8_t dummyByte[1]								= {0xFF};

/* Switches to the continuous mode. */
static uint8_t setContinuousMode[10]					= {0xB5, 0x62, 0x06, 0x11, 0x02, 0x00, 0x00, 0x00, 0x19, 0x81};

/* Switches to the selected power saving configuration. */
static uint8_t setPowerSaveMode[10]						= {0xB5, 0x62, 0x06, 0x11, 0x02, 0x00, 0x00, 0x01, 0x1A, 0x82};

/* CYCLIC 3s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation1[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x00, 0x2D, 0x00, 0x00, 0x10, 0x3, 0x0, 0xBB, 0x8, 0x0, 0x0, 0xBB, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x34, 0x09};

/* CYCLIC 3s, doNotEnterOff. */
static uint8_t setCyclicOperation2[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x00, 0x2D, 0x00, 0x00, 0x00, 0x3, 0x0, 0xBB, 0x8, 0x0, 0x0, 0xBB, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x24, 0x99};

/* CYCLIC 3s, update EPH. */
static uint8_t setCyclicOperation3[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x00, 0x2D, 0x00, 0x00, 0x10, 0x2, 0x0, 0xBB, 0x8, 0x0, 0x0, 0xBB, 0x8, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x33, 0xE3};

/* CYCLIC 1s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_1s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0xE8, 0x03, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0xD3, 0xCC};

/* CYCLIC 2s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_2s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0xD0, 0x07, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0xBF, 0xF8};

/* CYCLIC 3s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_3s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0xB8, 0x0B, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0xAB, 0x24};

/* CYCLIC 4s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_4s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0xA0, 0x0F, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x97, 0x50};

/* CYCLIC 5s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_5s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0x88, 0x13, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x83, 0x7C};

/* CYCLIC 6s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_6s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0x70, 0x17, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x6F, 0xA8};

/* CYCLIC 7s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_7s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0x58, 0x1B, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x5B, 0xD4};

/* CYCLIC 8s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_8s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0x40, 0x1F, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x47, 0x00};

/* CYCLIC 9s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_9s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0x28, 0x23, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x33, 0x2C};

/* CYCLIC 10s, doNotEnterOff, update EPH. */
static uint8_t setCyclicOperation_10s[52]				= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x3C, 0x00, 0x00, 0x90, 0x03, 0x01, 0x10, 0x27, 0x00, 0x00, 0x10, 0x27, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x1F, 0x58};

/* ONOFF 60s, doNotEnterOff, update EPH. */
static uint8_t setONOFFoperation1[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x00, 0x3C, 0x00, 0x00, 0x10, 0x1, 0x0, 0x60, 0xEA, 0x0, 0x0, 0x60, 0xEA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x4F, 0x4B};

/* ONOFF permanently. */
static uint8_t setONOFFoperation2[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x00, 0x01, 0x00, 0x00, 0x00, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x6F, 0x33};

/* ONOFF permanently. */
static uint8_t setONOFFoperation3[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x00, 0x00, 0x00, 0x80, 0x00, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x62, 0x50};

/* ONOFF permanently, doNotEnterOff, maxStartup 5. */
static uint8_t setONOFFoperation4[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x05, 0x00, 0x00, 0x80, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x68, 0x48};

/* ONOFF permanently, doNotEnterOff, update EPH, maxStartup 1. */
static uint8_t setONOFFoperation5[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x01, 0x00, 0x00, 0x90, 0x01, 0x01, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0x74, 0x10};

/* ONOFF 60s, maxStartup 1. */
static uint8_t setONOFFoperation6[52]					= {0xB5, 0x62, 0x06, 0x3B, 0x2C, 0x00, 0x01, 0x06, 0x01, 0x00, 0x00, 0x80, 0x00, 0x01, 0x60, 0xEA, 0x00, 0x00, 0x60, 0xEA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x2C, 0x01, 0x00, 0x00, 0x4F, 0xC1, 0x03, 0x00, 0x87, 0x02, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00, 0x64, 0x40, 0x01, 0x00, 0xF7, 0x4E};

/* ONOFF 60s, maxStartup 10. */
static uint8_t setONOFFoperation7[52]					= {0xB5, 0x62, 0x6, 0x3B, 0x2C, 0x0, 0x1, 0x6, 0x1, 0x0, 0x0, 0x80, 0x0, 0x1, 0x60, 0xEA, 0x0, 0x0, 0x60, 0xEA, 0x0, 0x0, 0x0, 0x0, 0x0, 0x0, 0x10, 0x27, 0x0, 0x0, 0x2C, 0x1, 0x0, 0x0, 0x4F, 0xC1, 0x3, 0x0, 0x87, 0x2, 0x0, 0x0, 0xFF, 0x0, 0x0, 0x0, 0x64, 0x40, 0x1, 0x0, 0x2E, 0x4F};

/* Backup mode for infinite duration. */
static uint8_t setBackupMode[16]						= {0xB5, 0x62, 0x02, 0x41, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x02, 0x00, 0x00, 0x00, 0x4D, 0x3B};

/* Software backup mode. */
static uint8_t setSwBackupMode[16]						= {0xB5, 0x62, 0x06, 0x57, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x4B, 0x43, 0x42, 0x86, 0x46};

/* GNSS running. */
static uint8_t setGNSSrunning[16]						= {0xB5, 0x62, 0x06, 0x57, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x20, 0x4E, 0x55, 0x52, 0x7B, 0xC3};

/* GNSS stopped. */
static uint8_t setGNSSstopped[16]						= {0xB5, 0x62, 0x06, 0x57, 0x08, 0x00, 0x01, 0x00, 0x00, 0x00, 0x50, 0x4F, 0x54, 0x53, 0xAC, 0x85};


// VARIABLES
static uint8_t GPSbuffer[GPSBUFFER_SIZE];

static uint8_t GPS_UBX_ack_error = 0;
static uint8_t GPS_UBX_checksum_error = 0;
static uint8_t GPS_UBX_buffer_error = 0;

static uint8_t GPS_NMEA_checksum_toverify = 0;
static uint8_t GPS_NMEA_checksum_calculated = 0;

static uint8_t GPS_NMEA_error_bitfield;							// default no ERRORs 0b00100000
extern uint8_t GPS_UBX_error_bitfield;							// default no ERRORs 0b00100000, global for APRS use

extern uint16_t GPS_NMEA_latitude_int;							// YYYY, +
extern uint32_t GPS_NMEA_latitude_dec;							// YYYYY, +
extern uint16_t GPS_NMEA_longitude_int;							// XXXXX, +
extern uint32_t GPS_NMEA_longitude_dec;							// XXXXX, +
extern uint8_t GPS_NMEA_NS;										// SOUTH 0	NORTH 1
extern uint8_t GPS_NMEA_EW;										// WEST 0	EAST 1

extern int32_t GPS_UBX_latitude;								// YYYYYYYYY, +/-
extern int32_t GPS_UBX_longitude;								// XXXXXXXXXX, +/-
extern float GPS_UBX_latitude_Float;							// YY.YYYYYYY, in +/- DEGREES (�), also used as output of Coords_DEGtoDEC()
extern float GPS_UBX_longitude_Float;							// XXX.XXXXXXX, in +/- DEGREES (�), also used as output of Coords_DEGtoDEC()

extern int32_t GPSaltitude;										// for both UBX and NMEA, in +/- meters (m)

extern uint8_t GPShour;											// for both UBX and NMEA
extern uint8_t GPSminute;										// for both UBX and NMEA
extern uint8_t GPSsecond;										// for both UBX and NMEA
extern uint8_t GPSday;											// for both UBX and NMEA
extern uint8_t GPSmonth;										// for both UBX and NMEA
extern uint16_t GPSyear;										// for both UBX and NMEA

extern uint8_t GPSsats;											// for both UBX and NMEA
extern uint8_t GPSfix;											// for both UBX and NMEA

extern uint8_t GPSnavigation;									// only UBX source
extern uint8_t GPSpowermode;									// only UBX source
extern uint8_t GPSpowersavemodestate;							// only UBX source

extern int32_t GPSgroundspeed;									// only UBX source, mm/s
extern int32_t GPSheading;										// only UBX source, degrees (1e-5)

extern uint16_t AD3data;										// raw ADC reading
extern uint16_t AD9data;										// raw ADC reading
extern uint16_t AD15data;										// raw ADC reading
extern uint16_t Si4060Temp;										// raw Si4060's ADC reading of temperature
extern uint32_t telemCount;										// current telemetry ID
extern uint32_t telemetry_len;									// length of the telemetry string

extern int32_t GPS_UBX_latitude_L;								// LAST valid value (in case of lost FIX)
extern int32_t GPS_UBX_longitude_L;								// LAST valid value (in case of lost FIX)
extern int32_t GPSaltitude_L;									// LAST valid value (in case of lost FIX)
extern uint8_t GPS_NMEA_NS_L;									// LAST valid value (in case of lost FIX)
extern uint8_t GPS_NMEA_EW_L;									// LAST valid value (in case of lost FIX)
extern uint16_t GPS_NMEA_latitude_int_L;						// LAST valid value (in case of lost FIX)
extern uint32_t GPS_NMEA_latitude_dec_L;						// LAST valid value (in case of lost FIX)
extern uint16_t GPS_NMEA_longitude_int_L;						// LAST valid value (in case of lost FIX)
extern uint32_t GPS_NMEA_longitude_dec_L;						// LAST valid value (in case of lost FIX)


// FUNCTIONS
uint16_t crc_xmodem_update(uint16_t crc, uint8_t data);
uint16_t CRC16_checksum(uint8_t *string, uint32_t len, uint32_t start);
uint32_t ASCII_8bit_transmit(uint8_t number, uint8_t *buffer, uint32_t sequence);
uint32_t ASCII_8bit_2DEC_transmit(uint8_t number, uint8_t *buffer, uint32_t sequence);
uint32_t ASCII_16bit_transmit(uint16_t number, uint8_t *buffer, uint32_t sequence);
uint32_t ASCII_16bit_HEX_transmit(uint16_t number, uint8_t *buffer, uint32_t sequence);
uint32_t ASCII_32bit_transmit(uint32_t number, uint8_t *buffer, uint32_t sequence);
uint32_t ASCII_32bit_LATLON_DECIMAL_transmit(uint32_t number, uint8_t *buffer, uint32_t sequence, uint8_t figures);
void Coords_DEGtoDEC(uint32_t lat_INT, uint32_t lat_DEC, uint32_t lon_INT, uint32_t lon_DEC, uint8_t latNS, uint8_t lonEW);

uint8_t UBLOX_verify_checksum(volatile uint8_t *buffer, uint8_t len);
void UBLOX_fill_buffer_UBX(uint8_t *buffer, uint8_t len);
void UBLOX_fill_buffer_NMEA(uint8_t *buffer);
void UBLOX_send_message(uint8_t *message, uint8_t len);
void UBLOX_request_UBX(uint8_t *request, uint8_t len, uint8_t expectlen, void (*parse)(volatile uint8_t*));
void UBLOX_parse_0102(volatile uint8_t *buffer);
void UBLOX_parse_0121(volatile uint8_t *buffer);
void UBLOX_parse_0106(volatile uint8_t *buffer);
void UBLOX_parse_0624(volatile uint8_t *buffer);
void UBLOX_parse_0611(volatile uint8_t *buffer);
void UBLOX_parse_0107(volatile uint8_t *buffer);
void UBLOX_parse_ACK(volatile uint8_t *buffer);
void UBLOX_parse_empty(void);
void UBLOX_process_GGA(uint8_t *buffer);
void UBLOX_process_ZDA(uint8_t *buffer);
uint32_t UBLOX_construct_telemetry_UBX(uint8_t *buffer, uint32_t sequence);
uint32_t UBLOX_construct_telemetry_NMEA(uint8_t *buffer, uint32_t sequence);
void UBLOX_powersave_mode_init(uint8_t * mode);


#endif // ARM_UBLOX_H_