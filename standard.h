/********************************
* Title   : Standard.h			*
* Release : 1.1					*
* Creator : Malik				*
* Platform: AVR					*
* Created : 15.05.2017 14:17:28	*
* Changed : 20.10.2020			*
********************************/

#ifndef STANDARD_H
#define STANDARD_H
/********************************************************************************/
#include <avr/io.h>
#include <avr/interrupt.h>
/********************************* Definition ***********************************/
#define BIT(bit) (1 << (bit))
#define NULLPTR  ((void*)0)
#define FALSE	 0
#define TRUE	 1
/********************************************************************************/
typedef unsigned char 		BYTE;
typedef signed char 		SBYTE;
typedef unsigned int 		WORD;
typedef signed int 			SWORD;
typedef unsigned long int	DWORD;
typedef signed long int 	SDWORD;
typedef unsigned long long 	QWORD;
typedef signed long long 	SQWORD; 

typedef unsigned char 		BOOLEAN; /*  8 bits - use TRUE or FALSE for this type */
typedef char 				CHAR;    /*  For use in character arrays. Use int type otherwise. */
typedef unsigned char 		UINT8;   /*  8 bits */
typedef signed char 		SINT8;   /*  8 bits */
typedef unsigned short 		UINT16;  /* 16 bits */
typedef signed short 		SINT16;  /* 16 bits */
typedef unsigned int 		UINT32;  /* 32 bits */
typedef signed int 			SINT32;  /* 32 bits */
typedef unsigned long long 	UINT64;  /* 64 bits */
typedef signed long long 	SINT64;  /* 64 bits */
typedef float 				FLOAT;   /* 32 bits */
typedef float 				FLOAT32; /* 32 bits */
typedef double 				DOUBLE;  /* 64 bits */
typedef double 				FLOAT64; /* 64 bits */

#define BIT7 ((UINT8)0x80)
#define BIT6 ((UINT8)0x40)
#define BIT5 ((UINT8)0x20)
#define BIT4 ((UINT8)0x10)
#define BIT3 ((UINT8)0x08)
#define BIT2 ((UINT8)0x04)
#define BIT1 ((UINT8)0x02)
#define BIT0 ((UINT8)0x01)

#define MIN(a, b) (((a) < (b)) ? (a) : (b))
#define MAX(a, b) (((a) > (b)) ? (a) : (b))

/************************ Something like a <limits.h> ***************************/
#define SBYTE_MIN	-128
#define SWORD_MIN	-32768
#define SDWORD_MIN	-2147483648
#define SQWORD_MIN	-9223372036854775808
#define SBYTE_MAX	127
#define SWORD_MAX	32767
#define SDWORD_MAX	2147483647
#define SQWORD_MAX	9223372036854775807
#define BYTE_MAX	255
#define WORD_MAX	65535
#define DWORD_MAX	4294967295
#define QWORD_MAX	18446744073709551615

/********************************************************************************/
#endif /* STANDARD_H */
