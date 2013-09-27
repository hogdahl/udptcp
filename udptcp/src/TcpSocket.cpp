/*
 * TcpSocket.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#include "TcpSocket.h"

#include <netdb.h>
#include <fcntl.h>
#include <stddef.h>
#include <unistd.h>
#include <string.h>
#include <stdio.h>

TcpSocket::TcpSocket() {
	server = NULL;
	filter = MF_TCP_WRT;
	state = ST_CONNECTED; // if we create socket this way, its from srvsocket
	events = POLLIN;
}

TcpSocket::TcpSocket(const char * address, int serverport) {
	if(setConnection(address,serverport)){
		connect();
	}
	filter = MF_TCP_WRT;
}






TcpSocket::~TcpSocket() {
	// Socket is closed by parent
}

/** Callback from poll loop when event is triggered
 * @returns false if work is done and should be deleted
 */
bool TcpSocket::eventHandler(pollfd * pfd){
	if(pfd->revents & (POLLERR|POLLHUP)){
		//Failed to connect or write
		close(_fd);
		_fd = -1;
		if(server != NULL){
			state = ST_NONE;
		}else{
			state = ST_DELETE;
		}
		if(pfd->revents & POLLERR){
			fprintf(stderr,"Tcp connection failed\n");
		}
		if(pfd->revents & POLLHUP){
			fprintf(stderr,"Tcp hangup\n");
		}
		return true;
	}

	if(pfd->revents & POLLOUT){
		state = ST_CONNECTED;
		setEvents(POLLERR|POLLHUP|POLLIN|POLLPRI);
		printf("TCP connected\n");
		Message * msg = Message::getFreeMessage(MSG_DEFAULT_SIZE, MF_TCP_READ|MF_UDP_WRT,&msgOutQue);
		msg->length = 0; // send an empty message to indicate connect
		send(msg);
		msg->unlink();
	}

	if(pfd->revents & POLLIN){
		// received data
		Message * msg = Message::getFreeMessage(MSG_DEFAULT_SIZE, MF_TCP_READ|MF_UDP_WRT,&msgOutQue);
		msg->length = recv(_fd,msg->buffer,msg->maxSize(),0);
		//fprintf(stdout,"TCP rcv %d", msg->length);
		if(msg->length > 0){
			send(msg);
		}
		msg->unlink();
	}


	return true;
}


void TcpSocket::msgHandler(Message * msg){
	if(filter & msg->filter){
		// our message
		if(_fd >= 0){
			if(state == ST_CONNECTED){
				write(msg->buffer,msg->length);
			}
		}else{
			if(server != NULL)
				connect();
		}
	}
}

bool TcpSocket::setConnection(const char* address, int serverport) {

	server = gethostbyname(address);
	if(server != NULL){
		 bzero((char *) &serv_addr, sizeof(serv_addr));
		 serv_addr.sin_family = AF_INET;
		 bcopy((char *)server->h_addr, (char *)&serv_addr.sin_addr.s_addr, server->h_length);
		 serv_addr.sin_port = htons(serverport);
		 return true;
	}
	return false;
}

bool TcpSocket::connect() {

	fprintf(stderr,"TCP connecting ...");
    if((_fd = socket(AF_INET, SOCK_STREAM, 0)) >= 0){
    	fcntl(_fd, F_SETFL, O_NONBLOCK);
    	setEvents(POLLERR|POLLHUP|POLLIN|POLLPRI|POLLOUT);
    	// cause none blocking we dont expect result here
    	::connect(_fd,(struct sockaddr *) &serv_addr,sizeof(serv_addr));
    	return true;
    }
    return false;
}
