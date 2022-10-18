#ifndef LINKLIST_H_   /* Include guard */
#define LINKLIST_H_
typedef struct Node Node;
typedef struct list list;
list *ll_create_List(void);
Node *ll_create_Node(int entry);
void ll_add_last_Node(list *List, Node *entry);
void ll_print_all_node(list *List);
void ll_add_first_Node(list *List, Node *entry);
void ll_find_node(list *List, Node *entry);
int ll_delete_entry(list *List, Node *entry);
void ll_insert_Node(list *List, Node *entry, int ll_index);
void ll_delete_all_node(list *List);
#endif
