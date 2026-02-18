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
#define dprintf(fmt, ...) { fprintf(stdout, "d:[%s:%s:%d]: " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); fflush(stdout); }
#define eprintf(fmt, ...) { fprintf(stderr, "e:[%s:%s:%d]: " fmt, __FILE__, __func__, __LINE__, ##__VA_ARGS__); fflush(stderr); }
#elif
#define dprintf (void)0
#define eprintf (void)0
#endif

typedef unsigned int uint;

typedef int32_t  i32;
typedef int64_t  i64;

typedef uint32_t u32;
typedef uint64_t u64;
typedef uint8_t  u8;

typedef float  f32;
typedef double f64;

#endif
