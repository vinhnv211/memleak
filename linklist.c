#include <stdio.h>
#include <stdlib.h>

typedef struct Node
{
    int data;
    struct Node *next;
} Node;

typedef struct list
{
    Node *head;
} list;

#define LOG printf

list *ll_create_List(void)
{
    list *newList = (list *)malloc(sizeof(list));

    if (newList != NULL)
    {
        newList->head = NULL;
        return newList;
    }
    else
    {
        LOG("initialization failed list");
        return 0;
    }
}

Node *ll_create_Node(int entry)
{
    Node *newNode = (Node *)malloc(sizeof(Node));

    if (newNode != NULL)
    {
        newNode->data = entry;
        newNode->next = NULL;
        return newNode;
    }
    else
    {
        LOG("initialization failed entry");
        return 0;
    }
}

void ll_add_last_Node(list *List, Node *entry)
{

    if (List->head == NULL)
    {
        List->head = entry;
    }
    else
    {
        Node *tmp = List->head;
        while (tmp->next != NULL)
        {
            tmp = tmp->next;
        }
        tmp->next = entry;
    }
}
void ll_print_all_node(list *List)
{
    if (NULL == List->head)
    {
        LOG("Emtry list !\n");
    }
    Node *tmp = List->head;
    while (tmp->next != NULL)
    {
        LOG("%d ", tmp->data);
        tmp = tmp->next;
    }
    LOG("\n");
}
void ll_add_first_Node(list *List, Node *entry)
{
    if (List->head == NULL)
    {
        List->head = entry;
    }
    else
    {

        entry->next = List->head;
        List->head = entry;
    }
}
void ll_find_node(list *List, Node *entry)
{
    int index = 0, checkFindEntry = 0;
    Node *tmp = List->head;
    if (NULL == entry)
    {
        LOG("%s, Invalid argument!!! \n", __func__);
    }
    while (tmp->next != NULL)
    {
        if (tmp->data == entry->data)
        {
            checkFindEntry = 1;
            LOG("Entry found location %d index in List\n", index);
        }
        tmp = tmp->next;
        index++;
    }
    if (checkFindEntry == 0)
    {
        LOG("No found entry to find exists \n");
    }
}

int ll_delete_entry(list *List, Node *entry)
{
    int i = 0;
    Node *previous = NULL;
    Node *current = NULL;
    Node *next = NULL;
    int ret = 0;

    if (NULL == entry)
    {
        LOG("%s, Invalid argument!!! \n", __func__);
        return 0;
    }

    current = List->head;
    previous = NULL;

    while (current != NULL)
    {
        i++;

        if (current->data == entry->data)
        {
            next = current->next;

            if (previous != NULL)
            {
                previous->next = next;
            }

            // Found
            ret = 1;
            current->next = NULL;
            free(current);
            break;
        }

        previous = current;
        current = current->next;
    }

    if (ret == 1)
    {
        LOG("%s, found = %d, entry=%d\n", __func__, ret, i);
    }
    else
    {
        LOG("%s, No found entry to delete!!! \n", __func__);
    }

    return ret;
}

void ll_insert_Node(list *List, Node *entry, int ll_index)
{
    int index = 0, checkFindEntry = 0;
    Node *tmp = List->head;
    if (NULL == entry)
    {
        LOG("%s, Invalid argument!!! \n", __func__);
    }
    while (tmp->next != NULL)
    {
        if (ll_index == index + 1)
        {
            checkFindEntry = 1;
            Node *tmp1 = tmp;
            entry->next = tmp1->next;
            tmp->next = entry;
            LOG("Insert entry location %d in list\n", ll_index);
        }
        tmp = tmp->next;

        index++;
    }
    if (checkFindEntry == 0)
    {
        LOG("Insert entry failed \n");
    }
}
void ll_delete_all_node(list *List)
{
    Node *current = NULL;
    Node *previous = NULL;
    // Node *next = NULL;
    current = List->head;
    while (current->next != NULL)
    {
        previous = current;
        
        current = current->next;
        previous->next=NULL;
        free(previous);
    }

    LOG("Delete all entry \n");
}
