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


inline
uint32_t	mulu(uint16_t a, uint16_t b)
{
	/* コンパイラによる reordering を許可 */
	uint32_t	ret;

	asm (
		"	mulu %1,%0\n"
	:	/* 出力 */	"=d"	(ret)	/* out %0 (戻り値) */
	:	/* 入力 */	"d"		(a),	/* in  %1 (入力) */
					"0"		(b)		/* in  %2 (入力) = %0 と同じレジスタに割り当て */
	);
	return	ret;
}

inline
int32_t		muls(int16_t a, int16_t b)
{
	/* コンパイラによる reordering を許可 */
	int32_t		ret;

	asm (
		"	muls %1,%0\n"
	:	/* 出力 */	"=d"	(ret)	/* out %0 (戻り値) */
	:	/* 入力 */	"d"		(a),	/* in  %1 (入力) */
					"0"		(b)		/* in  %2 (入力) = %0 と同じレジスタに割り当て */
	);
	return	ret;
}

inline
uint16_t	divu(uint32_t a, uint16_t b)
{
	/* コンパイラによる reordering を許可 */
	uint16_t	ret;

	asm (
		"	divu %1,%0\n"
	:	/* 出力 */	"=d"	(ret)	/* out %0 (戻り値) */
	:	/* 入力 */	"d"		(b),	/* in  %1 (入力) */
					"0"		(a)		/* in  %2 (入力) = %0 と同じレジスタに割り当て */
	);
	return	ret;
}

inline
int16_t		divs(int32_t a, int16_t b)
{
	/* コンパイラによる reordering を許可 */
	int16_t		ret;

	asm (
		"	divs %1,%0\n"
	:	/* 出力 */	"=d"	(ret)	/* out %0 (戻り値) */
	:	/* 入力 */	"d"		(b),	/* in  %1 (入力) */
					"0"		(a)		/* in  %2 (入力) = %0 と同じレジスタに割り当て */
	);
	return	ret;
}

inline
uint16_t	modu(uint32_t a, uint16_t b)
{
	/* コンパイラによる reordering を許可 */
	uint32_t	ret;

	asm (
		"	divu %1,%0\n"
	:	/* 出力 */	"=d"	(ret)	/* out %0 (戻り値) */
	:	/* 入力 */	"d"		(b),	/* in  %1 (入力) */
					"0"		(a)		/* in  %2 (入力) = %0 と同じレジスタに割り当て */
	);
	return	(uint16_t)(ret >> 16);
}

#endif
