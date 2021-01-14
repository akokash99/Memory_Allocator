#include <stdlib.h>
#include "dbll.h"



/*
In here we create a doubly linked list.
dbll_Create will return an empty list or NULL if malloc fails.
*/
struct dbll *dbll_create()
{
	struct dbll *this = malloc(sizeof(struct dbll));
	if(this==NULL) return NULL;
	
	this->first = this->last = NULL;
	return this;

 
}




/*
in dbll_free we'll free all memory associated with a doubly linked list. This also 
frees the memory associated with the nodes. 
We herea assume that user data has been freed.
*/
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

/*
dbll_iterate will iterate through the nodes of the list.
This returns 1 on succesful iteration
*/

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




/*
Similar to dbll_iterate but done in reverse.
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





/*
This removes the llnode from the list given. After we remove we free the memory and assume user data has already been freed.
*/
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





/*
Insert a node after a certain node
*/
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





/*
insert a node before a certain node.
*/
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


/*
Append a node at the end of the list
*/
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
