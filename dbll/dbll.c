#include <stdlib.h>
#include "dbll.h"

/* Routines to create and manipulate a doubly-linked list */

/* create a doubly-linked list */
/* returns an empty list or NULL if memory allocation failed */


struct dbll *dbll_create()
{
	struct dbll *this = malloc(sizeof(struct dbll));
	if(this==NULL) return NULL;
	
	this->first = this->last = NULL;
	return this;

 
}

/* frees all memory associated with a doubly-linked list */
/* this must also free all memory associated with the linked list nodes */
/* assumes user data has already been freed */
void dbll_free(struct dbll *list)
{
	
	if (list == NULL){
		return;
	}
	// Free the elements
	
	struct llnode *node = list->first;

	while (node!=NULL){
		struct llnode *next = node->next;
		free(node);
		node = next;
	}
	// Free the list itself
	free(list);


}

/* iterate over items stored in a doubly-linked list */

/* begin iteration from node start, end iteration at node stop (include stop in iteration) */
/* the list is traversed using the next pointer of each node */

/* if start is NULL, then start is the first node */
/* if end is NULL, then end is the last node */

/* at each node, call the user function f with a pointer to the list,
   a pointer to the current node, and the value of ctx */

/* if f returns 0, stop iteration and return 1 */

/* return 0 if you reached the end of the list without encountering end */
/* return 1 on successful iteration */

int dbll_iterate(struct dbll *list,
				 struct llnode *start,
				 struct llnode *end,
				 void *ctx,
				 int (*f)(struct dbll *, struct llnode *, void *)){
				 

	
		if(start==NULL){
			start=list->first; 
		}
		if(end==NULL){
			end=list->last; 
		}
		struct llnode *node1;
		for (node1= start; node1!=end->next; node1=node1->next){
			
			int a=f(list,node1,ctx);
			if(a==0) return 1;
			
		}
	//if(node1!=end) return 0;

  return 1;
}

/* similar to dbll_iterate, except that the list is traversed using
   the prev pointer of each node (i.e. in the reverse direction).

   Adjust accordingly. For example if start is NULL, then start is the
   last node in the list.  If end is NULL, then end is the first node
   in the list.

*/

int dbll_iterate_reverse(struct dbll *list,
						 struct llnode *start,
						 struct llnode *end,
						 void *ctx,
						 int (*f)(struct dbll *, struct llnode *, void *)
						 ){

	
		if(start==NULL) start=list->last;
		if(end==NULL) end=list->first;
		
		for (struct llnode *node1= start; node1!=end->prev; node1=node1->prev){
			int a=f(list,node1,ctx);
			if(a==0) return 1;	
		}
  return 1;
}


/* Remove `llnode` from `list` zz*/
/* Memory associated with `node` must be freed */
/* You can assume user_data will be freed by somebody else (or has already been freed) */
void dbll_remove(struct dbll *list, struct llnode *node){

  
  for (struct llnode *node1=list->first; node1 != NULL; node1=node1->next){
		if (node1 == node) {
			if (node1 == list->first){
				list->first = node->next;
			}
			if (node1 == list->last){
				list->last = node->prev;
			}
			if (node1->prev != NULL){
				node1->prev->next = node1->next;
			}
			if (node1->next != NULL){
				node1->next->prev = node1->prev;
			}
			free(node1);
			return;
		}
	}

  
}

/* Create and return a new node containing `user_data` */
/* The new node must be inserted after `node` */
/* if node is NULL, then insert the node at the end of the list */
/* return NULL if memory could not be allocated */
struct llnode *dbll_insert_after(struct dbll *list, struct llnode *node, void *user_data)
{
	struct llnode *node2 = malloc(sizeof(struct llnode));
	if(node2==NULL) return NULL;
	
	node2->user_data = user_data;
	node2->next = node2->prev = NULL;
	

	if(node==NULL){
		node2->user_data=user_data;
		list->first=node2;
		list->last=node2;
	}
	else if (node->next==NULL){
		
		node2->user_data=user_data;
		node2->prev=node;
		node->next=node2;
		
		
		
	} 
	else{
		if(node->next!=NULL){
		node2->next=node->next;
		node2->prev=node;
		node->next->prev=node2;
		node->next=node2;
		}	
	}
	
  return node2;
	
 
}

/* Create and return a new node containing `user_data` */
/* The new node must be inserted before `node` */
/* if node is NULL, then insert the new node at the beginning of the list */
/* return NULL if memory could not be allocated */
struct llnode *dbll_insert_before(struct dbll *list, struct llnode *node, void *user_data){

	struct llnode *node2 = malloc(sizeof(struct llnode));
	if(node2==NULL) return NULL;
	
	node2->user_data = user_data;
	node2->next = node2->prev = NULL;
	

	if(node==NULL){
		if(list->first==NULL){
			node2->user_data=user_data;
			list->first=node2;
			list->last=node2;
		}
		else{
		node2->user_data=user_data;
		node2->next=list->first;
		list->first=node2;	
		}
	}
	else if (node->prev==NULL){
		
		node2->user_data=user_data;
		node2->next=node;
		node->prev=node2;
		
		
		
	} 
	else{
		if(node->prev!=NULL){
		node2->prev=node->prev;
		node2->next=node;
		node->prev->next=node2;
		node->prev=node2;
		}
	}
	
  return node2;
}

/* create and return an `llnode` that stores `user_data` */
/* the `llnode` is added to the end of list */
/* return NULL if memory could not be allocated */
/* this function is a convenience function and can use the dbll_insert_after function */
struct llnode *dbll_append(struct dbll *list, void *user_data){

	struct llnode *node = malloc(sizeof(struct llnode));
	if(node==NULL) return NULL;
	
	node->user_data = user_data;
	node->next = node->prev = NULL;
	node->prev=list->last;
	
	if(list->last!=NULL){
		list->last->next=node;
	}
	list->last=node;
	if(list->first==NULL){
		list->first=node;
		
	}
	
  return node;
}
