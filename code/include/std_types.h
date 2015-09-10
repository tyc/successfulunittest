#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

#if defined(__AVR__)

#define uchar_t 	unsigned char
#define uint8_t		unsigned char
#define sint8_t		signed char
#define uint16_t	unsigned int
#define sint16_t	signed int

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE,
} bool_t;

#else /* default inclusion */

#include <stdint.h>

#endif

#endif /* _STD_TYPES_H_ */
