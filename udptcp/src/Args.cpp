/*
 * Args.cpp
 *
 *  Created on: Sep 21, 2013
 *      Author: root
 */

#include "Args.h"
#include <stdlib.h>
#include <stddef.h>
long Args::argToLong(const char * arg) {
	if(arg[0] == '0' && ( arg[1] == 'x' || arg[1] == 'X' ) ){
		return strtol(arg,NULL,16);
	}
	return strtol(arg,NULL,10);
}
