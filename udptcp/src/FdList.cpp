/*
 * FdList.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#include "FdList.h"
#include "Message.h"
#include <unistd.h>

// static
ListHead FdList::list;
bool FdList::closeall = false;

FdList::FdList(int fd) {
	_fd = fd;
	events = POLLIN;
	filter = 0;
	readMsg = 0;
	linkAfter(&list);
	state=ST_NONE;
}

FdList::~FdList() {
	if(_fd > -1){
		close(_fd);
	}
	if(readMsg){
		delete readMsg;
	}
	msgOutQue.freeAll();
}

void FdList::send(Message * msg){
	FdList * fdp = (FdList*)list.getNext();

	int filter = msg->filter;
	while(! fdp->isHead()){
		if(filter & fdp->filter){
			fdp->msgHandler(msg);
		}

		fdp = (FdList*)fdp->getNext();
	}
}

/**
 * Write to socket or fd
 * @param data to write
 * @param len length of data
 */
int FdList::write(char * data, int len){
	return ::write(_fd,data,len);
}


void FdList::cleanup(){
	FdList * fdl;
	while((fdl = (FdList*)list.pop())){
		delete fdl;
	}
}

/**
 * Fill in a pollfd arr for use in poll(..)
 */
int FdList::setPollFd(pollfd * fdarr, FdList ** fdparr){
	FdList * fdp = (FdList *)list.getNext();
	int sz = 0;
	while(fdp != (FdList *)&list){
		if(fdp->_fd >= 0){
			fdarr->fd = fdp->_fd;
			fdarr->events = fdp->events;
			fdarr->revents = 0;
			fdarr++;
			*fdparr++ = fdp;
			sz++;
		}
		fdp = (FdList *)fdp->getNext();
	}
	return sz;
}

bool FdList::allTimeOut() {
	FdList * fdp = (FdList *)list.getNext();
	while(fdp != (FdList *)&list){
		fdp->timeout();
		if(fdp->state == ST_DELETE){
			FdList * todel = fdp;
			fdp = (FdList *)fdp->getNext();
			delete todel;
		}else{
			if(fdp->state == ST_EXIT){
				return false;
			}
			fdp = (FdList *)fdp->getNext();
		}
	}
	return true;
}
