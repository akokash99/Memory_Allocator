/*
 * poolalloc.c
 *
 *  Created on: Nov 8, 2019
 *      Author: abdul
 */


#include "dbll.h"
#include <stdlib.h>
#include "poolalloc.h"


int previousA=0;
/*
   a pool-based allocator that uses doubly-linked lists to track
   allocated and free blocks
 */

/* create and initialize a memory pool of the required size */
/* use malloc() or calloc() to obtain this initial pool of memory from the system */
struct memory_pool *mpool_create(size_t size){

	struct memory_pool *pool= malloc(sizeof(struct memory_pool));
	 /* set start to memory obtained from malloc */
	 /* set size to size */
	if(pool==NULL) return NULL;
	pool->start = (char *)malloc(size);

	pool->size=size;


 /* create a doubly-linked list to track allocations */
  /* create a doubly-linked list to track free blocks */
	pool->alloc_list= dbll_create();
	pool->free_list= dbll_create();


	

	struct alloc_info *alloc = malloc(sizeof(struct alloc_info));
	if(alloc==NULL) return NULL;
	alloc->size=size;
	alloc->offset=0;

	dbll_append(pool->free_list, alloc);
  /* create a free block of memory for the entire pool and place it on the free_list */

  /* return memory pool object */

  return pool;
}

/* ``destroy'' the memory pool by freeing it and all associated data structures */
/* this includes the alloc_list and the free_list as well */
void mpool_destroy(struct memory_pool *p){

	//loop through nodes and free the alloc_info objects
	if(p->free_list->first!=NULL){
	for (	struct llnode *node1= p->free_list->first; node1!=p->free_list->last->next; node1=node1->next) {
				struct alloc_info *data = (struct alloc_info *) node1->user_data;
				
				free(data);

		}}

	if(p->alloc_list->first!=NULL){
			for (	struct llnode *node1= p->alloc_list->first; node1!=p->alloc_list->last->next; node1=node1->next) {
					struct alloc_info *data = (struct alloc_info *) node1->user_data;

					free(data);

			}
	}
	
	dbll_free(p->alloc_list);
	dbll_free(p->free_list);
	free(p->start);
	free(p);

  /* make sure the allocated list is empty (i.e. everything has been freed) */
  /* free the alloc_list dbll */
  /* free the free_list dbll  */
  /* free the memory pool structure */
}


/* allocate a chunk of memory out of the free pool */

/* Return NULL if there is not enough memory in the free pool */

/* The address you return must be aligned to 1 (for size=1), 2 (for
   size=2), 4 (for size=3,4), 8 (for size=5,6,7,8). For all other
   sizes, align to 16.
*/


int decideAllign(int a){
	int b;

	if(a==1) b=1;
	else if(a==2) b=2;
	else if(a==3||a==4) b=4;
	else if(a>=5 && a<=8) b=8;
	else b=16;


	return b;
}




void *mpool_alloc(struct memory_pool *p, size_t size){

	char* address=0;
	
	struct alloc_info *ai;
	
	
	int qw=0;
	if(p->free_list->first!=NULL){ //looping the free list
	
		for (	struct llnode *node1= p->free_list->first; node1!=p->free_list->last->next; node1=node1->next) {
				struct alloc_info *data = (struct alloc_info *) node1->user_data;
				
				if(data->size > size){
				ai = (struct alloc_info *) data;
				qw=1;
				break;
			
				}


		}
	}
	
	
	
	struct alloc_info *last;
	
	struct alloc_info *alloc = malloc(sizeof(struct alloc_info));
	if(alloc==NULL) return NULL;

	if(p->alloc_list->last!=NULL)
	{last = (struct alloc_info *) p->alloc_list->last->user_data;


}

	
	if(p->alloc_list->first==NULL){

		char* end = p->start + p->size;
		int ch=0;
		for (char *z = p->start; z != end; ++z) { //loop through addresses until we find suitable one
			if((int)z%decideAllign(size)==0){ alloc->offset=0; //margin=z-p->start;
				ch=1;
				address=z;
				break; 
			}


		}
		if(ch==0) return NULL;
		
		alloc->size=size;
		ai->offset=alloc->size;
		

		ai->size=1024 - alloc->offset - size; //updating the size of the free block

	
		dbll_append(p->alloc_list, alloc);


		}



	else{

		char* end = p->start + ai->size+ai->offset;
		char* s=p->start + last->offset + last->size;
		

		int align=decideAllign(size);
		
		char counter=0;
		
		int ch2=0;
		for (char *z = s; z != end; ++z) { //loop through addresses until we find suitable one

			if((int)z%align==0 ){
			ch2=1;
			alloc->offset= (z-p->start);

			alloc->size=size;
			last->size=last->size+counter;
			ai->offset=z-p->start+alloc->size;
			ai->size=1024 - alloc->offset - size;
			
			address=z;
		

			dbll_append(p->alloc_list, alloc);
			counter=0;
			break; 
			}
		
			counter++;

		}
		if(ch2==0) return NULL;


	}

		



	return address;
  /* check if there is enough memory for allocation of `size` (taking
	 alignment into account) by checking the list of free blocks */

  /* search the free list for a suitable block */
  /* there are many strategies you can use: first fit (the first block that fits),
	 best fit (the smallest block that fits), etc. */

  /* if no suitable block can be found, return NULL */

  /* if found, create an alloc_info node, store start of new region
	 into offset, set size to allocation size (take alignment into
	 account!), set free to null */

  /* add the new alloc_info node to the memory pool's allocated
	 list */

  /* return pointer to allocated region*/



}

/* Free a chunk of memory out of the pool */
/* This moves the chunk of memory to the free list. */
/* You may want to coalesce free blocks [i.e. combine two free blocks
   that are are next to each other in the pool into one larger free
   block. Note this requires that you keep the list of free blocks in order */
void mpool_free(struct memory_pool *p, void *addr){

	char* passed=  addr;
	int offset=(int) (passed - p->start); //this is the offset that we loop for

	
	
	
	for (	struct llnode *node1= p->alloc_list->first; node1!=p->alloc_list->last->next; node1=node1->next) {
			struct alloc_info *data = (struct alloc_info *) node1->user_data;
			if(data->offset==offset) {
				dbll_remove(p->alloc_list, node1);
				dbll_insert_before(p->free_list, NULL, data);	
				break;
			}

	}

		  /* search the alloc_list for the block */
		  /* move it to the free_list */
		  /* coalesce the free_list */

		int check=0;
		if(p->free_list->first!=NULL){ //looping the free list
		struct llnode *remove= p->free_list->first;
		struct alloc_info *removed = (struct alloc_info *) remove->user_data;
		
		
			for (	struct llnode *node1= p->free_list->first->next; node1!=p->free_list->last->next; node1=node1->next){
					struct alloc_info *data = (struct alloc_info *) node1->user_data;
					

					if(removed->offset==data->offset+data->size){
					
					
						struct alloc_info *alloc = malloc(sizeof(struct alloc_info));
						alloc->size=data->size+removed->size;
						alloc->offset=data->offset; //this doesn't take into account a node removed in between two free nodes
						free(removed);
						dbll_remove(p->free_list, remove);
						free(data);
						dbll_remove(p->free_list, node1);
						
						dbll_append(p->free_list, alloc);
						check =alloc->offset+alloc->size;
						
						break;
					}


			}
		}

	
	 
	
	if(check==p->size) {
		free(p->free_list->first->user_data);
		dbll_remove(p->free_list,p->free_list->first);
	}
	


	



 
}


