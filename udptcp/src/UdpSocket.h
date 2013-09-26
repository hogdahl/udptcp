/*
 * UdpSocket.h
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#ifndef UDPSOCKET_H_
#define UDPSOCKET_H_

#include <netdb.h>

#include "FdList.h"
#include "Buffer.h"

class UdpSocket: public FdList {
public:
	  struct sockaddr_in si_from;
	  struct sockaddr_in si_to;
	  int _som;
	  int _eom;
	  Buffer buffer;
public:
	UdpSocket();
	virtual ~UdpSocket();

	/** Callback from poll loop when event is triggered
	 * @returns false if work is done and should be deleted
	 */
	virtual bool eventHandler(pollfd * pfd);

	/** Send a message
	 * sender is responsible for trashing message after use ( or reuse )
	 */
	virtual void msgHandler(Message * msg);

	/**
	 * Set up this socket for receiving
	 */
	void setup(int port);


	/**
	 * Set up a destination
	 */
	bool setDestination(const char * address, int port);

	/**
	 * Write to socket or fd
	 * @param data to write
	 * @param len length of data
	 */
	virtual int write(char * data, int len);

	void setSom(int som){
		_som = som;
	}
	void setEom(int eom){
		_eom = eom;
	}

};

#endif /* UDPSOCKET_H_ */
