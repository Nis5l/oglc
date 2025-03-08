#ifndef __DEF_H__
#define __DEF_H__

#include <stdio.h>
#include <stdint.h>

#define DEBUG

#ifdef DEBUG
#define ASSERT(X, fmt, ...) if(!(X)) eprintf(fmt, ##__VA_ARGS__)
#elif
#define ASSERT(X) (void)0
#endif

#ifdef DEBUG
#define dprintf(fmt, ...) fprintf(stdout, "[%s:%s:%d]: " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#define eprintf(fmt, ...) fprintf(stderr, "[%s:%s:%d]: " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__)
#elif
#define dprintf (void)0
#define eprintf (void)0
#endif

#define i32 int
#define i64 long int

#define u32 unsigned int
#define u64 unsigned long int

#define u8 uint8_t

#define f32 float
#define f64 double
#endif
