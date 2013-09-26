/*
 * SystemTime.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#include "SystemTime.h"

/**
 * Get relative time
 */
bool SystemTime::gettime(timespec * ts){
	  return clock_gettime(0,ts) != -1;
}
