/*
 * TcpSrvSocket.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#include "TcpSrvSocket.h"
#include "TcpSocket.h"
#include <netdb.h>
#include <fcntl.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <netinet/in.h>
#include <arpa/inet.h>

TcpSrvSocket::TcpSrvSocket(int tcpport, int maxconn, const char * acceptFrom){
    struct sockaddr_in sa;
    memset(&sa,0,sizeof(struct sockaddr_in));
    sa.sin_family= AF_INET;
    sa.sin_port= htons((u_short)tcpport);

    if ((_fd = socket(AF_INET, SOCK_STREAM,0)) >= 0){
        // try to reuse old port, result does not really matter
        // int reuse = 1;
        // setsockopt(_fd, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse));

        if (bind(_fd ,(struct sockaddr*) &sa,sizeof(sa)) < 0)
          {
            close(_fd);
            _fd = -1;
            printf("Failed to bind %d to socket\n", tcpport);
            state = ST_EXIT;
            return;
          }
        // cause poll we use none blocking
        fcntl(_fd, F_SETFL, O_NONBLOCK);
        setEvents(POLLERR|POLLHUP|POLLIN|POLLPRI);
        listen(_fd, maxconn);  /* max # of queued connects */
        setAccept(acceptFrom);
    }else{
    	state = ST_EXIT;
    }
}

/** Callback from poll loop when event is triggered
 * @returns false if work is done and should be deleted
 */
bool TcpSrvSocket::eventHandler(pollfd * pfd){
	if(pfd->revents & POLLIN){
		int sock;
		sockaddr_in from;
		unsigned int fromlen = sizeof(from);
		if((sock = accept(_fd, (struct sockaddr*)&from, &fromlen)) != -1){
			// Should this be accepted ?
			if(checkAccept(&from)){
				TcpSocket * tsock = new TcpSocket();
		        // cause poll we use none blocking
		        fcntl(sock, F_SETFL, O_NONBLOCK);
				tsock->_fd = sock;
				fprintf(stdout, "Received TCP conn from %s:%d\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port));
				return true;
			}else{
				fprintf(stderr, "Received TCP conn from unknown %s:%d\n",inet_ntoa(from.sin_addr), ntohs(from.sin_port));
			}
		}
	}
	return true;
}

/** Handle a message
 * sender is responsible for trashing message after use ( or reuse )
 */
void TcpSrvSocket::msgHandler(Message * msg){

}

bool TcpSrvSocket::setAccept(const char* address) {

	if(address && strlen(address) > 0){
		struct hostent * client = gethostbyname(address);
		if(client != NULL){
			bzero((char *) &accept_in, sizeof(accept_in));
			accept_in.sin_family = AF_INET; // not really neded
			bcopy((char *)client->h_addr, (char *)&accept_in.sin_addr.s_addr, client->h_length);
			accept_in.sin_port = 0; // any port
			return true;
		}
	}else{
		accept_in.sin_family = AF_INET; // not really neded
		accept_in.sin_addr.s_addr = 0;  // take any address
		accept_in.sin_port = 0;         // any port
		return true;
	}
	return false;
}

bool TcpSrvSocket::checkAccept(struct sockaddr_in * check_in) {
	if(accept_in.sin_addr.s_addr == 0 || check_in->sin_addr.s_addr == accept_in.sin_addr.s_addr){
		return true;
	}
	return false;
}
