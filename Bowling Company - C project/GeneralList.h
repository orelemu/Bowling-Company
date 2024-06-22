/********************************************/
/*   list.h                                 */
/*   a dynamic  linked list with a header   */
/********************************************/


#ifndef _LIST_
#define _LIST_

#define CHECK_RETURN_NULL(ptr) {if(!ptr) return NULL;}
#define CHECK_COLSE_RETURN_NULL(ptr,fp) {if(!ptr){fclose(fp); return NULL;}}

/*** Definitions ***/
typedef void* DATA;

typedef enum { False, True } BOOL;	// a boolean type

// Node
typedef struct node
{
	DATA			key;
	struct node* next;
}NODE;


// List
typedef struct
{
	NODE head;
}LIST;


/*** Function prototypes ***/
// create new list
BOOL L_init(LIST* pList);

// add new node after *pNode
NODE* L_insert(NODE* pNode, DATA Value);

// erase node after *pNode
BOOL L_delete(NODE* pNode, void freeFunc(void*));

// return a pointer to the node 
const NODE* L_find(const NODE* pNode, DATA Value, int compare(const void*, const void*));

NODE* L_findMax(NODE* pNode, int compare(void*, void*));

// free list memory
BOOL L_free(LIST* pList, void freeFunc(void*));

// print the list content
int L_print(const LIST* pList, void (*print)(const void*));
#endif
