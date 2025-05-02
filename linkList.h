#ifndef linklist_h
#define linklist_h

typedef struct node link_list;

struct node *CreateNode(void *, struct node *);/*this function create new node , constructor*/
void InsertNode(struct node **, struct node *);/*this function insert new node into the list and add it to the end*/
void FreeLinkList(struct node *);/*this function destroy a list from the memory*/

/*this function search the given data , in a list , with matched compare function*/
void* LinkListFind(struct node *head, void *data, int(*comapr_func)(const void *, const void *));

#endif
