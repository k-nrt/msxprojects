//-----------------------------------------------------------------------------
//. types.
//-----------------------------------------------------------------------------
#ifndef __MSX_TYPES_H__
#define __MSX_TYPES_H__

typedef unsigned char  u8;
typedef unsigned short u16;
typedef unsigned long   u32;
typedef signed char  s8;
typedef signed short s16;
typedef signed long  s32;

typedef u8 bool;

#define TRUE (1)
#define FALSE (0)
#define NULL  (0)

#if !defined(__INTELLISENSE__)
#define SDCCCALL(value) __sdcccall(value)
#endif

#define UNUSED(value) (value)
#endif //__MSX_TYPES_H__
