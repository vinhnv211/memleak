#ifndef DU_MALLOC_MEM_H
#define DU_MALLOC_MEM_H
// #define LengthNameTraceFuntion 1500
// #define LengthOneTraceFuntioin 500
#include <stdio.h>
#include "configDuMem.h"


#define memleak
// #define LogLinkList
// #define LogDuMem
// #define LogFreeDuMem


// #define TimeStartTrackMalloc 1000000
// #define TimeEndTrackMalloc 500000

typedef enum bool
{
    false,
    true
} bool;

typedef struct addressMallocInFuntion
{
    void *point;
    size_t sizePointMalloc;
    struct addressMallocInFuntion *NextAddressMalloc;
} addressMallocInFuntion;

typedef struct list
{
    unsigned short int CountMalloc;
    unsigned short int CountFree;
    unsigned short int CountMallocTrackWithTime;
    unsigned short int CountFreeTrackWithTime;
    char traceFuntion[LengthNameTraceFuntion];
    addressMallocInFuntion *addressPoint;
    int numbertrace;
    struct list *next;
} list;

typedef struct listGobal
{
    list *head;
} listGobal;


// #ifdef memleak
// #define DU_MA   du_malloc
// #else
// #define DU_MA   malloc
// #endif

extern pthread_mutex_t lock;
extern listGobal *List;
extern unsigned short int countMemLeck;
extern const int shm_fd;
extern time_t timeStart;
extern time_t TimeFinished;
extern bool checkInitListGobal;

extern int i;
static pthread_t id;
extern time_t TimeStart;

void InitTime();
time_t TimeCurrent();

list *Init_newListEntry();
bool checkNameFuntionInEntry(char **traceFuntion, int numberAddress);
list *findNameInEntry(list *entry, char **traceFuntion, int numberAddress);
bool findExistsMallocAddress(addressMallocInFuntion *addressMalloc, void *FindPointInMallocAddress);
void ll_create_entry(char **traceFuntion, int numberAddress, void *pointMallocAddress, size_t sizeMalloc);
listGobal *ll_create_list();
// void ll_getNameBackTrace(char **traceFuntion,int numberAddress);
void ll_print_all();
void free_listGobal();
void ll_InitlistGobal();
void init_lock_thread();
// void removeListInStructAddressMallocInFuntion(addressMallocInFuntion *ListMallocInFuntion, void *p_addr);
list *removeListInStructAddressMallocInFuntion(void *ptr);
char *ll_getNameBackTrace(char **traceFuntion, int numberAddress);
bool CompareString( char *str1, char *str2);
void TurnUpMallocCountNoTimeTrace(list *Tmp);
void TurnUpMallocCountTimeTrace(list *Tmp);
void TurnUpFreeCountNoTimeTrace(list *Tmp);
void TurnUpFreeCountTimeTrace(list *Tmp);
void ThreadToTrackTime();

extern void *du_malloc(size_t size_points);
// void *du_realloc(char *type_point,size_t size_points );
bool ll_CompareNameBackTrace(char *traceNameFuntion, char **traceFuntion, int numberAddress);
extern void *du_realloc(void *ptr, size_t newSize);
extern void *du_calloc(size_t Num, size_t newSize);
extern void *du_free(void *p_addr);
void ll_dump();
void ll_dumpLeak();
void ll_delete_all();
void ll_save_text();
void ll_sig_handler();
extern void ll_signal();
void create_share_memory();
#endif