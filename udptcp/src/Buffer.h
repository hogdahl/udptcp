/*
 * Buffer.h
 *
 *  Created on: Sep 26, 2013
 *      Author: root
 */

#ifndef BUFFER_H_
#define BUFFER_H_

#include <string.h>

class Buffer {
	char * _data;
	char * _readPos;
	char * _writePos;
	int _size;
public:
	Buffer();
	Buffer(int size);
	virtual ~Buffer();
	char * getData(){
		return _data;
	}

	bool addData(char * adata, int len){
		int lenin = (int)(_writePos - _data);
		if(len < _size - lenin){
			memcpy(_writePos, adata, len);
			_writePos += len;
			return true;
		}
		return false;
	}

	/**
	 * Read data from buffer,
	 * if *len < 0, peek all
	 * else read all up to *len
	 */
	char * readData(int * len){
		int tlen = _writePos - _readPos;
		if(*len >= 0){
			// read as much as is required or is there
			if(tlen < *len){
				*len = tlen;
			}
			char * tmp = _readPos;
			_readPos += *len;
			if(_readPos == _writePos ){
				_readPos = _writePos = _data;
			}
			return tmp;
		}
		// read all there is, but don't raise readPos
		*len = tlen;
		return _readPos;
	}

	/**
	 * Move data in buffer to beginning of buffer
	 */
	void move(){
		if(_readPos > _data){
			register char * to = _data;
			register char * from = _readPos;
			while(from < _writePos){
				*to++ = *from++;
			}
			int sz = _readPos - _data;
			_readPos = _data;
			_writePos -= sz;
		}
	}

	/**
	 * Flush contents of buffer
	 */
	void flush(){
		_readPos = _writePos = _data;
	}
	/**
	 * create a new buffer, data in ev old buffer is void
	 */
	bool setSize(int size);

	/** check if there is data in buffer
	 */
	int length(){
		return _writePos - _readPos;
	}

};

#endif /* BUFFER_H_ */
