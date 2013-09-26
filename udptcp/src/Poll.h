/*
 * Poll.h
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#ifndef POLL_H_
#define POLL_H_

class Poll {
public:
	bool doExit;
public:
	Poll();
	virtual ~Poll();

	void loop();

	void doPoll();

};

#endif /* POLL_H_ */
