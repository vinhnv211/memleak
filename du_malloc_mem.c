#include <stdio.h>
#include <execinfo.h>
#include <stdlib.h>
#include <string.h>
#include "du_malloc_mem.h"

#define LOG printf
#define num_funtion_callback 100
// typedef enum bool
// {
//     false,
//     true
// } bool;

// typedef struct addressMallocInFuntion
// {
//     void *point;
//     struct addressMallocInFuntion *NextAddressMalloc;
// } addressMallocInFuntion;

// typedef struct list
// {
//     char **traceFuntion;
//     addressMallocInFuntion *addressPoint;
//     int numbertrace;
//     struct list *next;
// } list;
// typedef struct listGobal
// {
//     list *head;
// } listGobal;
// List = ll_create_list();

addressMallocInFuntion *Init_addressPointFuntion()
{
    addressMallocInFuntion *new = NULL;
    new = malloc(sizeof(addressMallocInFuntion));
    // check new null
    if (NULL == new)
    {

        LOG("FALSE not init address Malloc");
        exit(1);
    }
    else
    {
        new->NextAddressMalloc = NULL;
        new->point = NULL;

        return new;
    }
}

list *Init_newListEntry()
{
    list *newEntry = NULL;
    addressMallocInFuntion *newAddressMalloc;
    newAddressMalloc = Init_addressPointFuntion();
    newEntry = malloc(sizeof(list));
    // check null newEnttry
    if (newEntry == NULL)
    {
        LOG("FALSE not init list");
        exit(1);
    }
    else
    {
        // memset(*(newEntry->traceFuntion), 0, sizeof(newEntry->traceFuntion));
        newEntry->addressPoint = NULL;
        newEntry->addressPoint = newAddressMalloc;
        newEntry->next = NULL;
        newEntry->numbertrace = 0;

        return newEntry;
    }
}

bool checkNameFuntionInEntry(listGobal *List, char **traceFuntion, int numberAddress)
{
    bool checkConfident = false;
    list *entryTmp = List->head;
    while (entryTmp != NULL)
    {
        if (entryTmp->numbertrace == numberAddress)
        {
            for (int i = 0; i < numberAddress; i++)
            {
                if (entryTmp->traceFuntion[i] != traceFuntion[i])
                {
                    // LOG("%s \n",entryTmp->traceFuntion[i]);
                    // LOG("%s \n",traceFuntion[i]);
                    checkConfident = true;
                }
            }
        }
        // else
        // {
        //     // int valueLarge = (numberAddress > entryTmp->numbertrace) ? numberAddress : entryTmp->numbertrace;
        //     checkConfident = false;
        // }

        entryTmp = entryTmp->next;
    }

    return checkConfident;
}

list *findNameInEntry(list *entry, char **traceFuntion, int numberAddress)
{
    // using when return true in checkNameFuntionInEntry
    list *entryTmp = entry;

    while (entryTmp->next != NULL)
    {

        for (int i = numberAddress; i < numberAddress; i++)
        {
            if (entry->traceFuntion[i] != traceFuntion[i])
            {
                return entryTmp;
            }
        }

        entryTmp = entryTmp->next;
    }

    return entry;
}

bool findExistsMallocAddress(addressMallocInFuntion *addressMalloc, void *FindPointInMallocAddress)
{
    bool check = false;
    addressMallocInFuntion *tmpAddressMalloc = addressMalloc;
    while (tmpAddressMalloc != NULL)
    {
        /* code */
        if (tmpAddressMalloc->point != NULL)
        {
            if (FindPointInMallocAddress == tmpAddressMalloc->point)
            {
                check = true;

                return check;
            }
        }

        tmpAddressMalloc = tmpAddressMalloc->NextAddressMalloc;
    }
    return check;
}

void *ll_create_entry(listGobal *List, char **traceFuntion, int numberAddress, void *pointMallocAddress)
{
    list *newEntry = NULL, *FindNameInEntry = NULL;
    // newEntry->traceFuntion=traceFuntion;
    if (List->head == NULL)
    {
        newEntry = Init_newListEntry();
        newEntry->addressPoint->point = pointMallocAddress;
        newEntry->traceFuntion = traceFuntion;
        newEntry->numbertrace = numberAddress;
        List->head = newEntry;
        LOG("List Gobal is emtry then create new list entry \n");
    }
    else
    {
        list *entryTmp = List->head;
        if (checkNameFuntionInEntry(List, traceFuntion, numberAddress) && List->head->addressPoint != NULL ) // check find exitst name funtion entry
        {

            list *FindNameInEntry = findNameInEntry(entryTmp, traceFuntion, numberAddress);
            if (findExistsMallocAddress(FindNameInEntry->addressPoint, pointMallocAddress))
            {
                LOG("Point Malloc Address Exists \n");
            }
            else
            {
                // check not find exitst malloc point in entrace then add new point adddress
                addressMallocInFuntion *newMallocAddress = Init_addressPointFuntion();
                addressMallocInFuntion *preMallocAdress = List->head->addressPoint;
                newMallocAddress->point = pointMallocAddress;
                newMallocAddress->NextAddressMalloc = NULL;
                // add last newMallocAddress
                while (preMallocAdress->NextAddressMalloc != NULL)
                {
                    preMallocAdress = preMallocAdress->NextAddressMalloc;
                }
                if(preMallocAdress!=NULL)
                {
                    preMallocAdress->NextAddressMalloc = newMallocAddress;
                }
                

                LOG("Find entrace funtion exist, added pointed %p malloc in entrace %d number funtion call \n", pointMallocAddress, numberAddress);
            }
        }
        else
        {
            newEntry = Init_newListEntry();
            newEntry->numbertrace = numberAddress;
            newEntry->traceFuntion = traceFuntion;
            newEntry->addressPoint->point = pointMallocAddress;
            newEntry->next = List->head;
            List->head = newEntry;

            LOG("Create entrace funtion, added pointed %p malloc in entrace \n ", pointMallocAddress);
        }
    }
}

listGobal *ll_create_list()
{
    listGobal *newList = (listGobal *)malloc(sizeof(newList));
    if (newList != NULL)
    {
        newList->head = NULL;
    }
    else
    {
        LOG("Initialization failed list");
        exit(1);
    }
}

void Free_Malloc(addressMallocInFuntion **maloc_func)
{
    if ((*maloc_func)->NextAddressMalloc != NULL)
    {
        Free_Malloc(&((*maloc_func)->NextAddressMalloc));
        (*maloc_func)->NextAddressMalloc = NULL;
        if ((*maloc_func)->point != NULL)
        {
            (*maloc_func)->point = NULL;
        }
        free(maloc_func);
    }
}
void Free_ALL_List(list **entry)
{
    if ((*entry)->next != NULL)
    {
        Free_ALL_List(&((*entry)->next));
        (*entry)->next = NULL;
    }

    if ((*entry)->addressPoint != NULL)
    {
        Free_Malloc(&((*entry)->addressPoint));
    }
}

void ll_print_all(listGobal *List)
{
    list *tmp = List->head;
    list *symbol_str = tmp;
    if (tmp == NULL)
    {
        LOG("Emtry list! \n");
    }
    else
    {
        LOG("\n....... Memory leak ...... \n");

        while (tmp != NULL)
        {
            // LOG("PRINt \n");
            if (tmp != NULL)
            {
                LOG("\n +>");
            }

            for (int i = tmp->numbertrace - 3; i >= 0; --i)
            {
                LOG("%s ", strchr(strtok(strtok(tmp->traceFuntion[i], ") "), "+"), 40) + 1);
                if (i > 0)
                {
                    LOG("-> ");
                }
            }
            if (tmp != NULL)
            {
                LOG("\n");
            }
            if (tmp->addressPoint == NULL)
            {
                LOG("   => Emtry Address Point !\n");
                // break;
            }

            else
            {
                addressMallocInFuntion *tmpAddressMalloc = tmp->addressPoint;
                while (tmpAddressMalloc != NULL)
                {
                    if (tmpAddressMalloc->point != NULL)

                    {
                        LOG("\t *Address %p \n", tmpAddressMalloc->point);
                    }
                    tmpAddressMalloc = tmpAddressMalloc->NextAddressMalloc;
                }
            }
            tmp = tmp->next;
        }
    }
}

void free_listGobal(listGobal *List)
{
    list *tmp = List->head;
    Free_ALL_List(&List->head);
    free(List);
}
void *du_malloc(int size_points, char *type_point, listGobal *List)
{

    int i, num_of_addrs;
    void *buff[num_funtion_callback];
    char **symbol_str;

    void *new_malloc = malloc(size_points * sizeof(type_point));
    if (new_malloc == NULL)
    {
        LOG("Malloc point false");
        exit(1);
    }

    num_of_addrs = backtrace(buff, num_funtion_callback);
    // LOG("bt element : %d \n", num_of_addrs);

    symbol_str = backtrace_symbols(buff, num_of_addrs);
    if (symbol_str == NULL)
    {
        LOG("backtrace_symbols returned NULL \n");
        return NULL;
    }
    char *pch;

    ll_create_entry(List, symbol_str, num_of_addrs, new_malloc);
    // free(symbol_str);
    return new_malloc;
}
void removeListInStructAddressMallocInFuntion(addressMallocInFuntion *ListMallocInFuntion, void *p_addr)
{
    // find list in struct address malloc in funtion if exits then del
    bool check = false;
    addressMallocInFuntion *temp;
    addressMallocInFuntion *CurrentListMallocInFuntion = ListMallocInFuntion;
    addressMallocInFuntion *PreListMallocInFuntion = NULL;
    addressMallocInFuntion *NextMallocInFuntion = NULL;
    if (CurrentListMallocInFuntion->point == p_addr)
    {
        CurrentListMallocInFuntion=ListMallocInFuntion->NextAddressMalloc;
        free(CurrentListMallocInFuntion);
        // return;

    }
    else
    {
        while (CurrentListMallocInFuntion != NULL)
        {
            if (CurrentListMallocInFuntion->point == p_addr)
            {
                NextMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
                if (PreListMallocInFuntion != NULL)
                {
                    PreListMallocInFuntion->NextAddressMalloc = NextMallocInFuntion;
                }
                CurrentListMallocInFuntion->NextAddressMalloc = NULL;
                CurrentListMallocInFuntion->point = NULL;
                free(CurrentListMallocInFuntion);
                LOG("Remove %p \n", p_addr);
                check = true;
            }
            PreListMallocInFuntion = CurrentListMallocInFuntion;
            CurrentListMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
        }
    }
}

void du_free(void *p_addr, listGobal *List)
{
    list *entryCurrent =List->head;
    while (entryCurrent != NULL)
    {
        addressMallocInFuntion *addressMalloc = entryCurrent->addressPoint;
        // removeListInStructAddressMallocInFuntion(addressMalloc, p_addr);
        bool check = false;
        addressMallocInFuntion *temp;
        addressMallocInFuntion *CurrentListMallocInFuntion = addressMalloc;
        addressMallocInFuntion *PreListMallocInFuntion = NULL;
        addressMallocInFuntion *NextMallocInFuntion = NULL;
        if ( CurrentListMallocInFuntion != NULL && CurrentListMallocInFuntion->point == p_addr)
        {
            // CurrentListMallocInFuntion=ListMallocInFuntion->NextAddressMalloc;
            entryCurrent->addressPoint=addressMalloc->NextAddressMalloc;
            // List->head=entryCurrent->next;
            free(CurrentListMallocInFuntion);
            // return;

        }
        else
        {
            while (CurrentListMallocInFuntion != NULL)
            {
                if (CurrentListMallocInFuntion->point == p_addr)
                {
                    NextMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
                    if (PreListMallocInFuntion != NULL)
                    {
                        PreListMallocInFuntion->NextAddressMalloc = NextMallocInFuntion;
                    }
                    CurrentListMallocInFuntion->NextAddressMalloc = NULL;
                    CurrentListMallocInFuntion->point = NULL;
                    free(CurrentListMallocInFuntion);
                    LOG("Remove %p \n", p_addr);
                    check = true;
                }
                PreListMallocInFuntion = CurrentListMallocInFuntion;
                CurrentListMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
        }
    }
    
        entryCurrent = entryCurrent->next;
}
}
// void du_free(void *p_addr, listGobal *List)
// {
//     list *entryCurrent =List->head;
//     while (entryCurrent != NULL)
//     {
//         addressMallocInFuntion *addressMalloc = entryCurrent->addressPoint;
//         removeListInStructAddressMallocInFuntion(addressMalloc, p_addr);
    
//         entryCurrent = entryCurrent->next;
// }
// }
void ll_dump(listGobal *List)
{
    list *tmp = List->head;
    list *symbol_str = tmp;
    if (tmp == NULL)
    {
        LOG("Emtry list! \n");
    }
    else
    {
        LOG("\n....... Memory leak ...... \n");

        while (tmp != NULL)
        {
            if (tmp->addressPoint !=NULL)
            {
                LOG("+> ");
                for (int i = tmp->numbertrace - 3; i >= 0; --i)
                {
                    LOG("%s ", strchr(strtok(strtok(tmp->traceFuntion[i], ") "), "+"), 40) + 1);
                    if (i > 0)
                    {
                        LOG("-> ");
                    }
                }
                if (tmp != NULL)
                {
                    LOG("\n");
                }
                addressMallocInFuntion *tmpAddressMalloc = tmp->addressPoint;
                while (tmpAddressMalloc != NULL)
                {
                    if (tmpAddressMalloc->point != NULL)

                    {
                        LOG("\t *Address %p \n", tmpAddressMalloc->point);
                    }
                    tmpAddressMalloc = tmpAddressMalloc->NextAddressMalloc;
                }
            }
            tmp = tmp->next;
        }
    }
}
void ll_delete_all(listGobal *List)
{
    
}
// void fun1(listGobal *List)
// {
//     char *k = (char*)du_malloc(5, "char", List);
//     // free(k);
//     du_free(k, List);
// }
// void fun2(listGobal *List)
// {
//     return fun1(List);
// }
// void fun3(listGobal *List)
// {
//     return fun2(List);
// }
// void fun9(int *p,listGobal *List)
// {
//     du_free(p,List);
// }
// int t(listGobal *List)
// {
//     int *k=(int*)du_malloc(100,"int",List);
//     return k;
// }
// int main()
// {
//     listGobal *begin = ll_create_list();
//     // ll_print_all(begin);
//     int *p = du_malloc(10, "int", begin);
//     int *p1 = du_malloc(10, "int", begin);
//     int *p2 = du_malloc(10, "int", begin);
//     int *p12 = du_malloc(10, "int", begin);
//     int *p32 = du_malloc(10, "int", begin);
//     // // // // Free_ALL_List(&begin->head);
//     fun2(begin);
//     fun2(begin);
//     fun2(begin);
//     fun2(begin);
//     fun3(begin);
//     fun3(begin);
//     fun3(begin);
//     fun3(begin);
//     fun3(begin);
//     // ll_print_all(begin);
//     du_free(p, begin);
//     int *kd = t(begin);
//     // du_free(kd,begin);
//     // fun9(p, begin);
//     // ll_print_all(begin);
//     // fun9(p, begin);
//     // ll_print_all(begin);
//     // free_listGobal(begin);
//     // int *k = du_malloc(10, "int", begin);
//     ll_print_all(begin);
//     // removeTraceFuntionIfNotExisInListGobal(begin);
//     ll_dump(begin);

//     // list *k = ;
// }