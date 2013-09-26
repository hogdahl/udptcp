/*
 * Buffer.cpp
 *
 *  Created on: Sep 26, 2013
 *      Author: root
 */

#include "Buffer.h"

Buffer::Buffer() {
	_data = 0;
	_readPos = _writePos = _data;
	_size = 0;
}

Buffer::Buffer(int size) {
	_data = new char[size];
	_readPos = _writePos = _data;
	_size = size;
}

Buffer::~Buffer() {
	if(_data) delete _data;
}

bool Buffer::setSize(int size) {
	if(_data) delete _data;
	if(_data){
		_data = new char[size];
		_readPos = _writePos = _data;
		_size = size;
		return true;
	}
	_readPos = _writePos = _data;
	_size = 0;
	return false;
}
