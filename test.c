#include "linklist.h"
int main()
{
    list *begin = ll_create_List();
    Node *k1 = ll_create_Node(5);
    for (int i = 0; i < 20; i++)
    {
        Node *entry = ll_create_Node(i);
        ll_add_first_Node(begin, entry);
    }
    ll_find_node(begin, k1);
    ll_print_all_node(begin);
    ll_insert_Node(begin, k1, 3);
    ll_print_all_node(begin);
    ll_delete_entry(begin, k1);
    ll_print_all_node(begin);
    ll_delete_all_node(begin);
    ll_print_all_node(begin);
    return 0;
}