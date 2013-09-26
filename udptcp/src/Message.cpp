/*
 * Message.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#include "Message.h"

ListHead Message::pool;

Message::Message(int len, int msgfilter) {
	buffer = new char[len];
	filter = msgfilter;
	_maxSize = len;
	length=0;
}

Message::~Message() {
	if(buffer){
		delete buffer;
	}
}

