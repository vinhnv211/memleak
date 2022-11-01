
typedef enum bool
{
    false,
    true
} bool;
typedef struct addressMallocInFuntion
{
    void *point;
    struct addressMallocInFuntion *NextAddressMalloc;
} addressMallocInFuntion;

typedef struct list
{
    char **traceFuntion;
    addressMallocInFuntion *addressPoint;
    int numbertrace;
    struct list *next;
} list;
typedef struct listGobal
{
    list *head;
} listGobal;
// typedef struct addressMallocInFuntion{} addressMallocInFuntion;
// typedef struct list{} list;
// addressMallocInFuntion *Init_addressPointFuntion();

// typedef struct listGobal{} listGobal;= ll_create_list();
// extern listGobal *List ;
list *Init_newListEntry();
bool checkNameFuntionInEntry(listGobal *List, char **traceFuntion, int numberAddress);
list *findNameInEntry(list *entry, char **traceFuntion, int numberAddress);
bool findExistsMallocAddress(addressMallocInFuntion *addressMalloc, void *FindPointInMallocAddress);
void *ll_create_entry(listGobal *List, char **traceFuntion, int numberAddress, void *pointMallocAddress);
listGobal *ll_create_list();
void ll_print_all(listGobal *List);
void free_listGobal(listGobal *List);

void removeListInStructAddressMallocInFuntion(addressMallocInFuntion *ListMallocInFuntion, void *p_addr);
void *du_malloc(int size_points, char *type_point, listGobal *List);
void du_free(void *p_addr, listGobal *List);
void ll_dump(listGobal *List);
void ll_delete_all(listGobal *List);