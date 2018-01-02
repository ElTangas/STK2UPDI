/*
 * stk_io.h
 *
 * Created: 18-11-2017 14:55:53
 *  Author: JMR_2
 */ 


#ifndef STK_IO_H_
#define STK_IO_H_

#include <stdint.h>

namespace STK_io {
	// Function prototypes
	uint8_t put(char c);
	uint8_t get();
	void init();
}

#endif /* STK_IO_H_ */