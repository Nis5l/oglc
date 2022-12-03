#ifndef __DEF_H__
#define __DEF_H__

#include <stdio.h>

#define DEBUG

#define ASSERT(X) (void)0

#ifdef DEBUG
#define dprintf(fmt, args...) fprintf(stdout, "[%s:%s:%d]: " fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
#define eprintf(fmt, args...) fprintf(stderr, "[%s:%s:%d]: " fmt, __FILE__, __FUNCTION__, __LINE__, ##args)
#elif
#define dprintf (void)0
#define eprintf (void)0
#endif

#define i32 int
#define i64 long int

#define u32 unsigned int
#define u64 unsigned long int

#define f32 float
#define f64 double
#endif
