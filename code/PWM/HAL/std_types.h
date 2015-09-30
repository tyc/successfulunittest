#ifndef _STD_TYPES_H_
#define _STD_TYPES_H_

#if defined(__AVR__)

typedef unsigned char 	uchar_t
typedef unsigned char	uint8_t
typedef signed char 	sint8_t
typedef unsigned int	uint16_t
typedef signed int		sint16_t

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE,
} bool_t;

#else /* default inclusion */

typedef unsigned char 	uchar_t;
typedef unsigned char	uint8_t;
typedef signed char 	sint8_t;
typedef unsigned int	uint16_t;
typedef signed int		sint16_t;

typedef enum
{
	FALSE = 0,
	TRUE = !FALSE,
} bool_t;

#ifndef NULL

#define NULL	((void *) (0))

#endif /* NULL */

#endif

#endif /* _STD_TYPES_H_ */
