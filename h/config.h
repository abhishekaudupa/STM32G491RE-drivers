#ifndef __CONFIG_H__
#define __CONFIG_H__

#ifndef DEBUG
#define LOCAL static inline
#else
#define LOCAL
#endif

#define READ_REG_BITS(REG, BITS)	((REG & BITS) >> BITS##_Pos)

#endif
