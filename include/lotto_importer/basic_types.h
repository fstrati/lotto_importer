// basic types

#ifndef LOTTO_BASIC_TYPES_H
#define LOTTO_BASIC_TYPES_H

#include <cstdint>

typedef enum : uint64_t
{
	NAZIONALE = 0,
	BARI,
	CAGLIARI,
	FIRENZE,
	GENOVA,
	MILANO,
	NAPOLI,
	PALERMO,
	ROMA,
	TORINO,
	VENEZIA,
	TUTTE,
	UNKNOWN,
} ruota_t;

typedef struct __attribute__((packed)) BITS
{
	uint64_t ruota : 4;
	uint64_t a     : 7;
	uint64_t b     : 7;
	uint64_t c     : 7;
	uint64_t d     : 7;
	uint64_t e     : 7;
	uint64_t day   : 5;
	uint64_t month : 4;
	uint64_t year  : 16;
} bits_t;

typedef union __attribute__((packed)) EXTRACTION
{
	uint64_t raw;
	bits_t   bits;
} extraction_t;

typedef enum : uint64_t
{
	NULL_MESE = 0,
	GEN,
	FEB,
	MAR,
	APR,
	MAG,
	GIU,
	LUG,
	AGO,
	SET,
	OTT,
	NOV,
	DIC,
} mese_t;

#endif // LOTTO_BASIC_TYPES_H




