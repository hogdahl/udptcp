/*
 * TcpSrvSocket.h
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#ifndef TCPSRVSOCKET_H_
#define TCPSRVSOCKET_H_

#include "FdList.h"
#include <netdb.h>

class TcpSrvSocket: public FdList{
public:
	struct sockaddr_in accept_in;
public:
	TcpSrvSocket(int tcpport, int maxconn, const char * acceptFrom);
	virtual ~TcpSrvSocket(){} // closed by FdList

	/** Callback from poll loop when event is triggered
	 * @returns false if work is done and should be deleted
	 */
	virtual bool eventHandler(pollfd * pfd);

	/** Send a message
	 * sender is responsible for trashing message after use ( or reuse )
	 */
	virtual void msgHandler(Message * msg);

	/**
	 * Only accept connections from  this host
	 */
	bool setAccept(const char* address);

	/**
	 * Check if client should be accepted
	 */
	bool checkAccept(struct sockaddr_in * check_in);

};

#endif /* TCPSRVSOCKET_H_ */
