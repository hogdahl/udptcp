/*
 * TcpSocket.h
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#ifndef TCPSOCKET_H_
#define TCPSOCKET_H_

#include "FdList.h"

#include <netdb.h>

class TcpSocket: public FdList {
    struct sockaddr_in serv_addr;
    struct hostent *server;
public:
	TcpSocket();
	// Connect to server at once
	TcpSocket(const char * address, int serverport);
	virtual ~TcpSocket();

	virtual const char * className(){ return "TcpSocket"; }


	/**
	 * do something on timeout
	 */
	virtual void timeout(){
		if(state == ST_NONE && server != 0) connect();
	};

	/** Callback from poll loop when event is triggered
	 * @returns false if work is done and should be deleted
	 */
	virtual bool eventHandler(pollfd * pfd);

	/** Send a message
	 * sender is responsible for trashing message after use ( or reuse )
	 */
	virtual void msgHandler(Message * msg);

	/**
	 * Register a servers name and port for later connect
	 */
	bool setConnection(const char * address, int serverport);

	/**
	 * Initiate connection to a server
	 */
	bool connect();



};

#endif /* TCPSOCKET_H_ */
