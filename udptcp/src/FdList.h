/*
 * FdList.h
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */
#ifndef FDLIST_H_
#define FDLIST_H_

#include <sys/poll.h>
#include "Message.h"

#include "ListHead.h"

#include "msgfilter.h"

enum states { ST_NONE, ST_CONNECTING, ST_CONNECTED, ST_DISCONNECTING
	, ST_DELETE // Delete this fdlist
	, ST_EXIT   // require exit
	, ST_ALL};

class FdList: public List {
public:
	static ListHead list;
	static bool closeall; // set this if a major failore is discovered
	ListHead msgOutQue;   // queue for outgoing messages
public:
	int _fd;
	int events;
	int filter;
	int state;
	Message * readMsg;
public:
	FdList(){_fd = -1; linkAfter((List*)(&list)); events =  POLLIN; filter = 0; readMsg = 0; state = ST_NONE;}
	FdList(int fd);
	virtual ~FdList();

	virtual const char * className(){ return "FdList"; }

	/** Callback from poll loop when event is triggered
	 * @returns false if work is done and should be deleted
	 */
	virtual bool eventHandler(pollfd * pfd){ return true;}

	/** Handle a message
	 * sender is responsible for trashing message after use ( or reuse )
	 */
	virtual void msgHandler(Message * msg){}

	/** Send a message to all FdList's with coresponding filter
	 *
	 */
	void send(Message * msg);

	/**
	 * Write to socket or fd
	 * @param data to write
	 * @param len length of data
	 */
	virtual int write(char * data, int len);

	/**
	 * Set events for poll
	 */
	void setEvents(int sevents){
		events = sevents;
	}

	/**
	 * do something on timeout
	 */
	virtual void timeout(){};

	/**
	 * Delete all in list
	 */
	static void cleanup();

	/**
	 * Fill an area of pollfd with events from list.
	 * Area has to be big enough, can be estimated with size()
	 */
	static int setPollFd(pollfd * fdarr, FdList ** fdparr);

	/**
	 * Do something with all members on timeout,
	 * if returning false... exit
	 */
	static bool allTimeOut();

};

#endif /* FDLIST_H_ */
