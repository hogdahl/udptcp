/*
 * ListHead.h
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#ifndef LISTHEAD_H_
#define LISTHEAD_H_

#include "List.h"

class ListHead: public List {
protected:
	int sz;
public:
	ListHead(){sz = 0;}
	~ListHead(){}

	bool isHead(){ return true; }

	void inc(){sz++;}
	void dec(){sz--;}
	int size(){ return sz;}


};

#endif /* LISTHEAD_H_ */
