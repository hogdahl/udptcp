/*
 * Args.h
 *
 *  Created on: Sep 21, 2013
 *      Author: root
 */

#ifndef ARGS_H_
#define ARGS_H_

class Args {
public:
	Args(){}
	virtual ~Args(){}

	/**
	 * evaluate a long value from arg, returns 0 on fail
	 */
	long argToLong(const char * arg);

};

#endif /* ARGS_H_ */
