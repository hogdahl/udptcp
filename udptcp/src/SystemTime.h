/*
 * SystemTime.h
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#ifndef SYSTEMTIME_H_
#define SYSTEMTIME_H_

#include <time.h>

class SystemTime {
public:
	/**
	 * Get relative time
	 */
	static bool gettime(timespec * ts);
};

#endif /* SYSTEMTIME_H_ */
