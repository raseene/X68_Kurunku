#ifndef	___COMMON_H___
#define	___COMMON_H___

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <iocslib.h>
#include <doslib.h>


typedef char			int8_t;
typedef unsigned char	uint8_t;
typedef short			int16_t;
typedef unsigned short	uint16_t;
typedef long			int32_t;
typedef unsigned long	uint32_t;
typedef int				Bool;

#define	FALSE	(Bool)0
#define	TRUE	(Bool)1


enum
{
	X	= 0,
	Y,
	XY,
};

#define	RGB(_r, _g, _b)		(((uint16_t)(_g) << 11) | ((uint16_t)(_r) << 6) | ((uint16_t)(_b) << 1))

#endif
