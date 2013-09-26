/*
 * List.h
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#ifndef LIST_H_
#define LIST_H_

class List {
protected:
	List * next;
	List * head;
public:
	List(){ next = this; head = this; }
	virtual ~List();

	List * getNext(){ return next; }

	virtual bool isHead(){ return false; }

	/**
	 * Link to header
	 */
	virtual void linkBefore(List * header);

	/**
	 * Link to header
	 */
	virtual void linkAfter(List * header);

	/**
	 * Unlink from header
	 */
	virtual void unlink(){ _unlink(); }

	/**
	 * Unlink from header
	 */
	void _unlink();

	/**
	 * Free all children
	 */
	void freeAll(){
		while(next != this){
			next->_unlink();
		}
	}

	/**
	 * return first object from list, will be unlinked, null if empty
	 */
	virtual List * pop(){
		if(next == this){
			return 0;
		}
		List * l = next;
		l->unlink();
		return l;
	}

	/**
	 * return first object from list, will be unlinked
	 * No check if empty, header could be returned;
	 */
	virtual List * _pop(){
		List * l = next;
		l->unlink();
		return l;
	}

	virtual void inc(){}
	virtual void dec(){}
	virtual int size(){ return head->isHead()?head->size():0;}


};

#endif /* LIST_H_ */
