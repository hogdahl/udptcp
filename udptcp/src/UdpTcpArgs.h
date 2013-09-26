/*
 * UdpTcpArgs.h
 *
 *  Created on: Sep 21, 2013
 *      Author: root
 */

#ifndef UDPTCPARGS_H_
#define UDPTCPARGS_H_

#include "Args.h"

#define ARGLIST  { "tcpAddress","tcpPort","tcpServerPort","udpLocalPort","udpAddress","udpPort","som","eom" }
#define REQUIRED {       1     ,    0    ,     1      ,    1     }

enum       argids{   tcpAddress,  tcpPort,  tcpServerPort,  udpLocalPort, udpAddress,udpPort, som, eom, argsize};


class UdpTcpArgs: public Args {
	const char * argp[argsize];
	long argval[argsize];
public:
public:
	UdpTcpArgs(int argc, char ** argv);
	virtual ~UdpTcpArgs();

	/**
	 * Print help for this
	 */
	static void help(int argc, char ** argv);

	/**
	 * True if parameter corespoding to argid is set
	 */
	bool isset(int argid){ return argp[argid][0] != 0; }

	/**
	 * Return arg corespoding to argid
	 */
	const char * arg(int argid){ return argp[argid]; }

	/**
	 * Return value corespoding to argid
	 */
	long val(int argid){ return argval[argid]; }
};

#endif /* UDPTCPARGS_H_ */
