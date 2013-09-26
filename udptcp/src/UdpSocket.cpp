/*
 * UdpSocket.cpp
 *
 *  Created on: Sep 5, 2013
 *      Author: johan
 */

#include "UdpSocket.h"
#include <sys/types.h>
#include <sys/socket.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <stdio.h>
#include <fcntl.h>


UdpSocket::UdpSocket() {
	filter = MF_UDP_WRT;
	events=POLLIN; // do not use on UDP |POLLHUP;
	_som = _eom = -1; // default not used
	buffer.setSize(MSG_DEFAULT_SIZE*2); // large enough to put 2 messages together
}

UdpSocket::~UdpSocket() {
	// socket closed by parent
}

/** Callback from poll loop when event is triggered
 * @returns false if work is done and should be deleted
 */
bool UdpSocket::eventHandler(pollfd * pfd){
	if(pfd->revents & POLLHUP){
		// This is BAD
		fprintf(stderr,"UDP hangup");
	}
	if(pfd->revents & POLLIN){
		Message * msg = Message::getFreeMessage(MSG_DEFAULT_SIZE, MF_UDP_READ|MF_TCP_WRT,&msgOutQue);
		socklen_t slen=sizeof(si_from);
		msg->length = recvfrom(_fd, msg->buffer, msg->maxSize(), 0, (sockaddr*)&si_from, &slen);
		if(msg->length > 0){
			// we will only accept messages from our destination
			if(si_from.sin_port == si_to.sin_port && si_from.sin_addr.s_addr == si_to.sin_addr.s_addr){
				send(msg);
			}else{
				fprintf(stderr, "Received UDP from unknown %s:%d",inet_ntoa(si_from.sin_addr), ntohs(si_from.sin_port));
			}
		}
		msg->unlink();
	}
	return true;
}

void UdpSocket::msgHandler(Message * msg){
	// This is a good spot to make packets from a stream
	// We will check for som and eom bytes in stream

	if(_eom > -1){
		// first, do we have buffered data
		if(buffer.length()){
			// ok we have, is there an eom in msg
			char * eom = (char *)memchr((void *)msg->buffer, _eom, (unsigned long int)msg->length);
			if(eom){
				// got a message
				if(buffer.addData(msg->buffer,eom + 1 - msg->buffer)){
					int len = 65000; // larger then buffer
					char * wp = buffer.readData(&len); // buffer is auto reset
					write(wp,len);
				}else{
					// nothing to do about full buffer
					buffer.flush();
					return;
				}
			}else{
				buffer.addData(msg->buffer,msg->length);
			}
			return;
		}
		// so no data in buffer, do we have a start of message
		if(_som > -1){
			char * som = (char *)memchr((void *)msg->buffer, _som, (unsigned long int)msg->length);
			if(som){
				// got som, do we have eom
				char * eom = (char *)memchr((void *)msg->buffer, _eom, (unsigned long int)msg->length);
				if(eom){
					// nice , got all we want
					write(som, eom + 1 - som);
				}else{
					// got start but not end, put msg in buffer
					int len = msg->length + som - msg->buffer;
					buffer.addData(som,len);
				}
			}
		}else{
			// dont use som ...
			// do we have eom
			char * eom = (char *)memchr((void *)msg->buffer, _eom, (unsigned long int)msg->length);
			if(eom){
				// nice , got all we want
				write(msg->buffer, eom + 1 - msg->buffer);
			}else{
				// got start but not end, put msg in buffer
				buffer.addData(msg->buffer,msg->length);
			}
		}
		return;
	}

	write(msg->buffer, msg->length);
}

/**
 * Set up this socket for receiving
 */
void UdpSocket::setup(int port){
	int sock;
	if ((sock = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP))!=-1){
		events=POLLIN; // do not use on UDP |POLLHUP;
		sockaddr_in si_me;
		memset((char *) &si_me, 0, sizeof(si_me));
		si_me.sin_family = AF_INET;
		si_me.sin_port = htons(port);
		si_me.sin_addr.s_addr = htonl(INADDR_ANY);

		//int reuse = 1;
		//if (setsockopt(sock, SOL_SOCKET, SO_REUSEADDR, &reuse, sizeof(reuse)) == -1){
			//printf("UdpSocket SO_REUSEADDR FAILED");
		//}

		if (bind(sock, (const sockaddr*)&si_me, sizeof(si_me))!=-1){
			_fd = sock;
	        // cause poll we use none blocking
	        fcntl(sock, F_SETFL, O_NONBLOCK);
		}else{
			close(sock);
			fprintf(stderr,"UDP failed to bind port %d\n", port);
			state = ST_EXIT;
		}
	}else{
		state = ST_EXIT;
	}
}

/**
 * Set up a destination
 */
bool UdpSocket::setDestination(const char * address, int port){
	//TODO change to inet_pton or inet ntop
  struct hostent * hp = gethostbyname(address);
  if(hp != NULL){
      in_addr * addrp = (in_addr *)hp->h_addr;
      memset((char *) &si_to, 0, sizeof(si_to));
      si_to.sin_addr = *addrp;
      si_to.sin_family = AF_INET;
      si_to.sin_port = htons(port);
      printf("destAddress %s", inet_ntoa(si_to.sin_addr));
  }else{
      printf("Udp destination hostname lookup failed for %s", address);
      return false;
  }

  return true;
}

/**
 * Write to socket or fd
 * @param data to write
 * @param len length of data
 */
int UdpSocket::write(char * data, int len){
	if(_fd >= 0 && si_to.sin_addr.s_addr != 0){
		socklen_t slen = sizeof(si_to);
		int res = sendto(_fd, data, len, 0, (sockaddr*)&si_to, slen);
		if(res == len){
			printf("Udp written");
			return len;
		}else{
			printf("Udp write failed");
		}
	}
	return -1;
}


