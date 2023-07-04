/*
 * STD_TYPES.h
 *
 *  Created on: Jul 1, 2023
 *      Author: Mohamed Hashim Attiya Said
 */

#ifndef STD_TYPES_H_
#define STD_TYPES_H_

typedef signed char s8;
typedef signed short s16;
typedef signed long s32;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned long u32;
typedef float f32;
typedef double f64;

typedef struct component
{
    u32 id;
    u32 comp;
} component_t;

#endif /* STD_TYPES_H_ */
