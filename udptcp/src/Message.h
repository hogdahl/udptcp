/*
 * Message.h
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#ifndef MESSAGE_H_
#define MESSAGE_H_

#include "ListHead.h"

#define MSG_DEFAULT_SIZE 2000

class Message : public List{
	static ListHead pool;
public:
	char * buffer;
	int length;
	int filter;
protected:
	int _maxSize;
public:
	Message(int len, int filter);
	virtual ~Message();
	static Message * getFreeMessage(int len, int filter, ListHead * tohead){
		Message * free = (Message *)pool.getNext();
		while(! free->isHead()){
			if(free->maxSize() >= len){
				free->linkBefore(tohead);
				free->filter = filter;
				return free;
			}
			free = (Message *)free->getNext();
		}
		free = new Message(len, filter);
		free->linkBefore(tohead);
		return free;
	}

	static void freePool(){
		Message * free;
		do{
			 free = (Message *)pool.getNext();
			if(! free->isHead()) delete free;

		}while(! free->isHead());

	}

	virtual void unlink(){
		List::unlink();
		linkAfter(&pool);
	}

	int maxSize(){return _maxSize;}

};

#endif /* MESSAGE_H_ */
