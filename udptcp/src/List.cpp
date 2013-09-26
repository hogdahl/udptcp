/*
 * List.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#include "List.h"


List::~List() {
	_unlink();
}

/**
 * Link this to a header or chain
 * If this is linked it will be unlinked
 */
void List::linkBefore(List * header)
{
	List * l = header;
	while(l->next != header){
		l = l->next;
	}
	next = header;
	l->next = this;
	head = header;
	head->inc();
}

void List::linkAfter(List * header)
{
	List * l = header->next;
	header->next = this;
	next = l;
	head = header;
	head->inc();
}

void List::_unlink()
{
	// this will work even if we are not linked
	List * l = next;
	while(l->next != this){
		l = l->next;
	}
	// now l is before us
	l->next = this->next;
	// make us our own
	next = this;
	head->dec();
	head = this;
}



