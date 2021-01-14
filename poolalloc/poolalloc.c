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
create a pool based allocator. This uses a double linked list(dbll.c) to track allocated blocks and free
them when needed
*/


/*
In memory_pool we're creating a memory pool of the required size.
*/
struct memory_pool *mpool_create(size_t size){

	struct memory_pool *pool= malloc(sizeof(struct memory_pool));


	if(pool==NULL) return NULL;
	pool->start = (char *)malloc(size);

	pool->size=size;



  
  /*
  creating two linked lists to track allocations and free blocks
  */
	pool->alloc_list= dbll_create();
	pool->free_list= dbll_create();


	

	struct alloc_info *alloc = malloc(sizeof(struct alloc_info));
	if(alloc==NULL) return NULL;
	alloc->size=size;
	alloc->offset=0;

	dbll_append(pool->free_list, alloc); // creating a free block of memory for the entire pool and then we're adding it on the free list



  return pool;
}



/*
This destroys and memory pool and all the associated data structures.
*/
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

  
}




/*
decideAllign returns an what the address should be aligned to. That is decided according to size we want to allocate.
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




	/*
	allocate a chunk of the memory pool and return address
	*/

	/*
	in here we are looking for a suitable block to put our data in.
	We search the free list for a suitable block that we can use. We are using the first fit approach.
	If we can't find a suitable block we return null.
	*/
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
	
	




}


   
   
   /*
	Free chunk of memory from the pool. (and so it moves it to the free list)
	Then we coalesce free blocks.
   */
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


