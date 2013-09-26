//============================================================================
// Name        : udptcp.cpp
// Author      : Johan Högdahl
// Version     :
// Copyright   : Open for all uses
// Description : Hello World in C++, Ansi-style
//============================================================================

#include <stdio.h>
#include "UdpTcpArgs.h"

#include "Poll.h"
#include "TcpSocket.h"
#include "TcpSrvSocket.h"
#include "UdpSocket.h"


int main(int argc, char ** argv) {
	if(argc == 0){
		// no argument given
		UdpTcpArgs::help(argc,argv);
		return 1;
	}

	UdpTcpArgs * args = new UdpTcpArgs(argc,argv);

	if(args->isset(tcpAddress)){
		if(args->isset(tcpPort)){
			// expected to be a tcp client
			printf("Set up tcp client to %s:%s\n", args->arg(tcpAddress), args->arg(tcpPort));
			new TcpSocket(args->arg(tcpAddress), args->val(tcpPort));
		}
	}

	if(args->isset(tcpServerPort)){
		// expect to be a server
		new TcpSrvSocket(args->val(tcpPort), 10, args->arg(tcpAddress));
	}

	if(args->isset(udpLocalPort)){
		if(args->isset(udpAddress)){
			if(args->isset(udpPort)){
				// expected to have an udp friend
				printf("Set up udp %s to %s:%s\n", args->arg(udpLocalPort), args->arg(udpAddress), args->arg(udpPort));
				UdpSocket * udpsock = new UdpSocket();
				udpsock->setup(args->val(udpLocalPort));
				udpsock->setDestination(args->arg(udpAddress),(int)args->val(udpPort));

				if(args->isset(som)){
					udpsock->setSom(args->val(som));
					if(args->isset(eom)){
						udpsock->setEom(args->val(eom));
					}
				}

			}else{
				fprintf(stderr,"missing udpPort\n");
			}
		}else{
			fprintf(stderr,"missing udpPort\n");
		}
	}else{
		fprintf(stderr,"missing udpLocalPort\n");
	}

	Poll * mpoll = new Poll();

	printf("Starting loop\n");
	// will be here as long as program runs
	mpoll->loop();
	printf("Stopped loop\n");

	delete mpoll;

	return 0;
}
