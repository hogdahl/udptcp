/*
 * Poll.cpp
 *
 *  Created on: Sep 3, 2013
 *      Author: johan
 */

#include "Poll.h"
#include "FdList.h"
#include <stdio.h>

Poll::Poll() {
	// TODO Auto-generated constructor stub
	doExit = false;
}

Poll::~Poll() {

}

void Poll::loop(){
	while(! doExit){
		doPoll();
	}
}

void Poll::doPoll() {
	int sz = FdList::list.size();
	pollfd pfdp[sz];// = new pollfd[sz];
	FdList *fastList[sz];// = new FdList*[sz];
	int nopfd = FdList::setPollFd(pfdp,fastList);
	int no;

	if((no = poll(pfdp, nopfd, 1000)) >= 0){
	      if(no == 0){
	    	  // time out
	    	  if(! FdList::allTimeOut()){
	    		  doExit = true;
	    	  }
	      }else{
	    	  for(int i = 0 ; i < nopfd ; i++){
	    		  if(pfdp[i].revents != 0){
	    			  FdList * fdp = (fastList[i]);
	    			  fdp->eventHandler(&pfdp[i]);
	    			  if(fdp->state >= ST_DELETE){
	    				  if(fdp->state == ST_EXIT){
	    					  fprintf(stderr, "%s requested EXIT\n", fdp->className());
	    					  doExit = true;
	    				  }else{
	    					 delete fdp;
	    				  }
	    			  }
	    		  }
	    	  }
	      }
	}else{
  	  fprintf(stderr,"NODO %d ",no);
	}
}
