#include "linkList.h"
#include <stdlib.h>
#include <stdio.h>

/*
*	this data structure represent a general list
*/
struct node
{
	void *data;
	struct node *next;
};

/*
*	this function create new node , constructor
*/
struct node *CreateNode(void *data, struct node *next)
{
	struct node *new_node = malloc(sizeof(struct node));
	if (new_node == NULL)
	{
		return NULL;
	}
	
	new_node -> data = data;
	new_node -> next = next; 
	return new_node;
}

/*
*	this function destroy a list from the memory
*/
void FreeLinkList(struct node *head)
{
	struct node *current = head;	
	while(head != NULL)
	{
		current = head -> next;
		free(head);
		head = current;
	}
}


/*
*	this function insert new node into the list and add it to the end
*/
void InsertNode(struct node **head, struct node *node_to_insert)
{
	if (*head == NULL)
	{
		*head = node_to_insert;
	}
	
	else
	{
		struct node *temp = (*head) -> next;
		(*head) -> next = node_to_insert;
		node_to_insert -> next = temp;
	}
}


/*
*	this function search the given data , in a list , with matched compare function
*/
void* LinkListFind(struct node *head, void *data, int(*comapr_func)(const void *, const void *))
{	
	while(head != NULL)
	{
		if (comapr_func(data, head -> data) == 0)
		{
			break;
		}
		head = head -> next;
	}
	if(head==NULL)
	{
		return NULL;
	}
	return head -> data;
}


/*
*	this function prints a list
*/
void printList(struct node *head,  int(*print_func)(const void *))
{
	while(head != NULL)
	{
		print_func(head->data);
		head = head -> next;
	}	
}


