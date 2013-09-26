/*
 * UdpTcpArgs.cpp
 *
 *  Created on: Sep 21, 2013
 *      Author: root
 */

#include "UdpTcpArgs.h"

#include <stdio.h>
#include <string.h>


UdpTcpArgs::UdpTcpArgs(int argc, char ** argv) {
	const char * arglist[] = ARGLIST;

	int i;
	bool unused[argc];
	// clear all
	for(i = 0 ; i < argsize ; i++){
		argp[i] = "";
	}

	for(i = 0 ; i < argc ; i++){
		unused[i] = true;
	}

	for(i = 0 ; i < argsize ; i++){
		int maxc = argc -1;
		for(int no = 1 ; no < maxc ; no++){
			if(unused[no] && strcmp(arglist[i],argv[no]) == 0){
				argp[i] = argv[no+1];
				unused[no] = unused[no+1] = false;
				argval[i] = argToLong(argp[i]);
				//printf("arg %s %s %d(%d)\n", argv[no], argv[no+1], no, argc );
			}
		}
	}
}

UdpTcpArgs::~UdpTcpArgs() {

}

void UdpTcpArgs::help(int argc, char ** argv) {
	const char * arglist[] = ARGLIST;
	int requireds[] = REQUIRED;
	printf("usage %s:", argv[0] );
	for(int i = 0 ; i < argsize ; i++){
		if(requireds[i]){
			printf(" %s <%s>", arglist[i],arglist[i] );
		}else{
			printf(" [%s <%s>]", arglist[i],arglist[i] );
		}
	}
}
