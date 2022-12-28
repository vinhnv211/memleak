#include <stdio.h>
// #include <execinfo.h>
#include <stdlib.h>
#include <string.h>

#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <assert.h>
#include <stddef.h>
#include <signal.h>
#include "du_malloc_mem.h"
#include "pthread.h"
// #include "threadTime.h"
#include <time.h>
// #include "backtraceFuntion.h"

// #include "backtracesyms.h"

// #define LOG(x...) fprintf(stderr, ##x)
// // #define LOG printf
// #define num_funtion_callback 100
// #define SIZE_SHARE_MEM 1024 * 2
// #define NAME_MEM_SHARE "nameShare"
// #define _GNU_SOURCE

listGobal *List = NULL;
unsigned short int countMemLeck = 0;
bool checkInitListGobal = false;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;
// int shm_fd;
time_t TimeFinished = TimeEndToTrack + TimeStartToTrack;
time_t timeStart = TimeStartToTrack ;
time_t TimeStart;

void InitTime()
{
    TimeStart = time(NULL);
}
time_t TimeCurrent()
{
    time_t timeEnd = time(NULL);
    time_t timeSecCont = timeEnd - TimeStart;
    // printf("%u \n", timeSecCont);
    return timeSecCont;
}

addressMallocInFuntion *Init_addressPointFuntion()
{
    addressMallocInFuntion *new = NULL;
    // new = malloc(sizeof(addressMallocInFuntion));
    new = mmap(NULL, sizeof(addressMallocInFuntion), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
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
        new->sizePointMalloc = 0;
        return new;
    }
}

list *Init_newListEntry()
{
    list *newEntry = NULL;
    addressMallocInFuntion *newAddressMalloc;
    newAddressMalloc = Init_addressPointFuntion();
    // newEntry = malloc(sizeof(list));
    newEntry = mmap(NULL, sizeof(list), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // check null newEnttry
    if (newEntry == NULL)
    {
        LOG("FALSE not init list");
        exit(1);
    }
    else
    {
        // memset(newEntry->traceFuntion, 0, sizeof(newEntry->traceFuntion));
        newEntry->addressPoint = NULL;
        newEntry->addressPoint = newAddressMalloc;

        newEntry->next = NULL;
        newEntry->numbertrace = 0;
        newEntry->CountMalloc = 0;
        newEntry->CountFree = 0;
        newEntry->CountMallocTrackWithTime = 0;
        newEntry->CountFreeTrackWithTime = 0;
        return newEntry;
    }
}

bool checkNameFuntionInEntry(char **traceFuntion, int numberAddress)
{
    __attribute__((unused)) bool checkConfident = false;
    __attribute__((unused)) list *entryTmp = List->head;
#ifdef LogDuMem
    LOG("q1 \n");
#endif

    while (entryTmp != NULL)
    {
#ifdef LogDuMem
        LOG("q2 \n");
#endif
        if (entryTmp->numbertrace == numberAddress)
        {
#ifdef LogDuMem
            LOG("q3 \n");
#endif

            if (ll_CompareNameBackTrace(entryTmp->traceFuntion, traceFuntion, numberAddress))
            {
#ifdef LogDuMem
                LOG("q4 \n");
#endif

                checkConfident = true;
            }
        }
        // else
        // {
        //     // int valueLarge = (numberAddress > entryTmp->numbertrace) ? numberAddress : entryTmp->numbertrace;
        //     checkConfident = false;
        // }

        entryTmp = entryTmp->next;
    }
#ifdef LogDuMem
    LOG("q5 \n");
#endif
    // LOG("q5 \n");
    return checkConfident;
}

list *findNameInEntry(list *entry, char **traceFuntion, int numberAddress)
{
    // using when return true in checkNameFuntionInEntry
    __attribute__((unused)) list *entryTmp = entry;
    // if (numberAddress >1)
    // {
    //     numberAddress=numberAddress-1;
    // }
    while (entryTmp->next != NULL)
    {

        // for (int i = numberAddress; i < numberAddress; i++)
        // {
        //     if (entry->traceFuntion[i] != traceFuntion[i])
        //     {
        //         return entryTmp;
        //     }
        // }
        if (ll_CompareNameBackTrace(entryTmp->traceFuntion, traceFuntion, numberAddress))
        {
            return entryTmp;
        }

        entryTmp = entryTmp->next;
    }

    return entry;
}

bool findExistsMallocAddress(addressMallocInFuntion *addressMalloc, void *FindPointInMallocAddress)
{
    __attribute__((unused)) bool check = false;
    __attribute__((unused)) addressMallocInFuntion *tmpAddressMalloc = addressMalloc;
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
// int findcharInName(char *charNameBackTrace)
// {
//     int i=0;
//     char k="[";
//     char j= "]";
//     while((char *)(charNameBackTrace[i]) != (char *)j)
//     {
//         if (charNameBackTrace[i] == k)
//         {
//             printf("%s \n",charNameBackTrace[i]);
//             return i;
//         }
//         if (i <= LengthNameTraceFuntion)
//         {
//             i++;
//         }
//         else
//         {
//             i=0;
//             break;
//         }
//     }
// }

char *ll_getNameBackTrace(char **traceFuntion, int numberAddress)
{
    // char *nameBackTrace = (char *)malloc(LengthNameTraceFuntion * sizeof(char));
    __attribute__((unused)) char *nameBackTrace = mmap(NULL, LengthNameTraceFuntion * sizeof(char), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    __attribute__((unused)) int count_str = 0;
    __attribute__((unused)) char name[LengthOneTraceFuntioin];
    __attribute__((unused)) int i;
    for (i = numberAddress-2; i >= 0; i--)
    {
        memcpy(&name, traceFuntion[i], LengthOneTraceFuntioin);
        // __attribute__((unused)) int j = 10;
        // LOG("name : %s",traceFuntion[i]);
        count_str += snprintf(nameBackTrace + count_str, 20, "%s ", (strchr(strtok(name, "]"), 91) + 1));
        // count_str += snprintf(nameBackTrace + count_str, 20, "%s -> ", (strchr(strtok(strtok(traceFuntion[i], ") "), "+"), 40) + 1));
        // LOG("%s +\n",strstr(traceFuntion[i],'['));
        // count_str += snprintf(nameBackTrace + count_str, 20, "%s -> ", (strchr(strtok(strtok(traceFuntion[i], ") "), "+"), 40) + 1));
        // count_str += snprintf(nameBackTrace + count_str, 20, "%s -> ", strchr(strtok(traceFuntion[i], "]"),133));
        // LOG("%d",j);

        // assert(strtok(traceFuntion[i], "[") != NULL);
        // count_str += snprintf(nameBackTrace + count_str, 20, "%s -> ", (strchr(strtok(strtok(traceFuntion[i], "] "), "["), 40) + 1));
        // count += strlen(strchr(strtok(strtok(traceFuntion[i], ") "), "+"), 40) + 1);
        // printf("%s", strchr(strtok(strtok(traceFuntion[i], ") "), "+"), 40) + 1);
        // strcpy(nameBackTrace+count,(strchr(strtok(strtok(traceFuntion[i], ") "), "+"), 40) + 1));
        // if(i == 0)
        // {
        //     printf("\n");
        // }
        // else
        // {
        //     printf(" -> ");
        // }
    }
    LOG("%s \n", nameBackTrace);
    // ll_dump();
    // printf("%s",nameBackTrace);
    // memcpy(nameBackTrace,nameBackTrace,);
    return nameBackTrace;
}

bool CompareString( char *str1, char *str2)
{
    int i;
    int check = 0, charCount=0;
    for(i = 0; i< LengthOneTraceFuntioin;i++)
    {
        if (str1[i] != str2[i])
        {
            check ++;
        }
        if (check > 3)
            return false;
        if ((char)(str1[i]) == ' ')
            charCount ++;
    }
    if (check >= 0 && check < 3 && charCount > 0)
    {
        return true;
    }
    else
    {
        return false;
    }
}


bool ll_CompareNameBackTrace(__attribute__((unused)) char *traceNameFuntion,
                             __attribute__((unused)) char **traceFuntion, __attribute__((unused)) int numberAddress)
{
#ifdef LogDuMem
    LOG("w1 \n");
#endif

    // if(traceNameFuntion == NULL)
    // {
    //     return false;
    // }
    // printf("%d \n",numberAddress);
    // printf("%p",traceNameFuntion);
#ifdef LogDuMem
    LOG("w2 \n");
#endif
    __attribute__((unused)) char nameBackTrace[LengthOneTraceFuntioin];
    __attribute__((unused)) int count_str = 0;
    __attribute__((unused)) char name[LengthOneTraceFuntioin];
    __attribute__((unused)) int i;
    // LOG("Check2 \n");
    for (i = numberAddress-3; i >= 0; i--)
    {
        // LOG("Check3 \n");
        strncpy(name, traceFuntion[i], LengthOneTraceFuntioin);
        // sscanf(,"0x%x",name);
        // LOG("ad %d \n\n", strlen(strchr(strtok(strtok(traceFuntion[i], ") "), "+"), 40) + 1));
        // count_str += snprintf(nameBackTrace + count_str, 20, "%s -> ", (strchr(strtok(strtok(traceFuntion[i], ") "), "+"), 40) + 1));
        // count_str += snprintf(nameBackTrace + count_str, 20, "%s -> ", strchr(strtok(traceFuntion[i], "]"),133));
        count_str += snprintf(nameBackTrace + count_str, 20, "%s ", (strchr(strtok(name, "]"), 91) + 1));
        // LOG("name %s \n",name);
    }
#ifdef LogDuMem
    LOG("w3 \n");
#endif
    // LOG("nameBackTrace %s \n\n",nameBackTrace);
    // LOG("name %s \n\n",traceNameFuntion);
    if (CompareString(traceNameFuntion, nameBackTrace) == 0)
    {
        return true;
    }
    return false;
}

void TurnUpMallocCountNoTimeTrace(list *Tmp)
{
    Tmp->CountMalloc = Tmp->CountMalloc + 1;
}
void TurnUpMallocCountTimeTrace(list *Tmp)
{
    time_t timeCurrent = TimeCurrent();

    if (timeStart < timeCurrent && timeCurrent < TimeFinished)
    {
        Tmp->CountMallocTrackWithTime = Tmp->CountMallocTrackWithTime + 1;
    }
}
void TurnUpFreeCountNoTimeTrace(list *Tmp)
{
    Tmp->CountFree = Tmp->CountFree + 1;
}
void TurnUpFreeCountTimeTrace(list *Tmp)
{
    time_t timeCurrent = TimeCurrent();
    if (timeStart < timeCurrent && timeCurrent < TimeFinished)
    {
        // LOG("%ld \n",TimeCurrent());
        Tmp->CountFreeTrackWithTime = Tmp->CountFreeTrackWithTime + 1;
    }
}

void ll_create_entry(char **traceFuntion, int numberAddress, void *pointMallocAddress, size_t sizeMalloc)
{
    __attribute__((unused)) list *newEntry = NULL, *FindNameInEntry = NULL;
    // newEntry->traceFuntion=traceFuntion;
    // if (List==NULL)
    // {
    //     List = ll_create_list();
    // }
    // else
#ifdef LogDuMem
    LOG("du 34 \n");
#endif

    if (List->head == NULL)
    {
#ifdef LogDuMem
        LOG("du 3455 \n");
#endif

        newEntry = Init_newListEntry();
        newEntry->addressPoint->point = pointMallocAddress;
        // newEntry->traceFuntion = traceFuntion;
        // ll_getNameBackTrace(newEntry->traceFuntion,traceFuntion,numberAddress);
        // newEntry->traceFuntion = ll_getNameBackTrace(traceFuntion, numberAddress);
#ifdef LogDuMem
        LOG("du 34 \n");
#endif
        strcpy(newEntry->traceFuntion, ll_getNameBackTrace(traceFuntion, numberAddress));
        TurnUpMallocCountNoTimeTrace(newEntry);
        newEntry->numbertrace = numberAddress;

        List->head = newEntry;

#ifdef LogLinkList
        LOG("List Gobal is emtry then create new list entry \n");
#endif
    }
    else
    {
        // ll_dump();
#ifdef LogDuMem
        LOG("du 3451 \n");
#endif

        __attribute__((unused)) list *entryTmp = List->head;
#ifdef LogDuMem
        LOG("p1 \n");
#endif

        if (checkNameFuntionInEntry(traceFuntion, numberAddress)==true && List->head->addressPoint != NULL) // check find exitst name funtion entry
        {
#ifdef LogDuMem
            LOG("pt1 \n");
#endif

            __attribute__((unused)) list *NameInEntry = findNameInEntry(entryTmp, traceFuntion, numberAddress);
            if (findExistsMallocAddress(NameInEntry->addressPoint, pointMallocAddress) == true)
            {
                LOG("tk1 \n");
                LOG("Point Malloc Address Exists \n");
            }
            else
            {
                // check not find exitst malloc point in entrace then add new point adddress
#ifdef LogDuMem
                LOG("mt1 \n");
#endif
                __attribute__((unused)) addressMallocInFuntion *newMallocAddress = Init_addressPointFuntion();
                __attribute__((unused)) addressMallocInFuntion *preMallocAdress = List->head->addressPoint;
                newMallocAddress->point = pointMallocAddress;
                newMallocAddress->NextAddressMalloc = NULL;
                newMallocAddress->sizePointMalloc = sizeMalloc;
                // add last newMallocAddress
                while (preMallocAdress->NextAddressMalloc != NULL)
                {
                    preMallocAdress = preMallocAdress->NextAddressMalloc;
                }
                if (preMallocAdress != NULL)
                {
                    preMallocAdress->NextAddressMalloc = newMallocAddress;
                }

#ifdef LogDuMem
                LOG("ty1 \n");
#endif

#ifdef LogLinkList
                LOG("Find entrace funtion exist 1, added pointed %p malloc in entrace %d number funtion call \n", pointMallocAddress, numberAddress);
#endif
                countMemLeck++;
                TurnUpMallocCountNoTimeTrace(NameInEntry);
                TurnUpMallocCountTimeTrace(NameInEntry);                
            }

        }
        else
        {
#ifdef LogDuMem
            LOG("tz1 \n");
#endif
            ll_dumpLeak();            
            newEntry = Init_newListEntry();
#ifdef LogDuMem
            LOG("tz12 \n");
#endif
            newEntry->numbertrace = numberAddress;

            TurnUpMallocCountNoTimeTrace(newEntry);
            TurnUpMallocCountTimeTrace(newEntry);
#ifdef LogDuMem
            LOG("tz21 \n");
#endif   
            // LOG("%s \n", ll_getNameBackTrace(traceFuntion, numberAddress));
            LOG("%s \n", ll_getNameBackTrace(traceFuntion, numberAddress));
            memcpy(&(newEntry->traceFuntion), ll_getNameBackTrace(traceFuntion, numberAddress),LengthNameTraceFuntion);
#ifdef LogDuMem
            LOG("tz233 \n");
#endif   
            newEntry->addressPoint->point = pointMallocAddress;
            newEntry->addressPoint->sizePointMalloc = sizeMalloc;
#ifdef LogDuMem
            LOG("tz2 \n");
#endif           
            newEntry->next = List->head;
#ifdef LogDuMem
            LOG("tz3 \n");
#endif
            List->head = newEntry;
#ifdef LogDuMem
            LOG("tz4 \n");
#endif
            countMemLeck++;

#ifdef LogLinkList
            LOG("Create entrace funtion, added pointed %p malloc in entrace \n ", pointMallocAddress);
#endif

#ifdef LogDuMem
            LOG("tb1 \n");
#endif
        }
    }
}

listGobal *ll_create_list()
{
    // listGobal *newList = (listGobal *)malloc(sizeof(newList));
    if (checkInitListGobal == false)
    {
        InitTime();
        pthread_create(&id, NULL, &ThreadToTrackTime, NULL);
        // countMemLeck ++;
    }
    __attribute__((unused)) listGobal *newList = (listGobal *)mmap(NULL, sizeof(listGobal), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (newList != NULL)
    {
        newList->head = NULL;
    }
    else
    {
        LOG("Initialization failed list");
        exit(1);
    }
    return newList;
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
void init_lock_thread()
{
    if (pthread_mutex_init(&lock, NULL) != 0)
    {
        printf("\n mutex init has failed\n");
        // return 1;
    }
}

void ll_print_all()
{
    __attribute__((unused)) list *tmp = List->head;
    __attribute__((unused)) list *symbol_str = tmp;
    __attribute__((unused)) int i;
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

            for (i = tmp->numbertrace - 3; i >= 0; --i)
            {
                LOG("%s ", strchr(strtok(strtok(&(tmp->traceFuntion[i]), ") "), "+"), 40) + 1);
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

void free_listGobal()
{
    __attribute__((unused)) list *tmp = List->head;
    Free_ALL_List(&List->head);
    free(List);
}
extern void *du_malloc(size_t size_points)
{

#ifdef memleak
// #define
#ifdef LogDuMem
    LOG("m1 \n");
#endif
    pthread_mutex_lock(&lock);
#ifdef LogDuMem
    LOG("m2 \n");
#endif
    __attribute__((unused)) int i, j, num_of_addrs;
    void *buff[num_funtion_callback];
    char **symbol_str;
    __attribute__((unused)) char *pch;
    if (checkInitListGobal == false)
    {
        List = ll_create_list();
        ++countMemLeck;
        checkInitListGobal = true;
    }
#ifdef LogDuMem
    LOG("m3 \n");
#endif
    void *new_malloc = malloc(size_points);
    // void *new_malloc = mmap(NULL, size_points, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    if (new_malloc == NULL)
    {
        LOG("Malloc point false");
        // exit(1);
    }
#ifdef LogDuMem
    LOG("m4  \n");
#endif
    num_of_addrs = backtraceF(buff, num_funtion_callback);
    // LOG("bt element : %d \n", num_of_addrs);

    symbol_str = backtrace_symbolsF(buff, num_of_addrs);
    if (symbol_str == NULL)
    {
        LOG("backtrace_symbols returned NULL \n");
        return NULL;
    }
    // printf
    // for (j = 0; j < num_of_addrs; j++)
    // {
    //     LOG("%s sdf \n", symbol_str[j]);
    // }
#ifdef LogDuMem
    LOG("m5 \n");
#endif
    ll_create_entry(symbol_str, num_of_addrs, new_malloc, size_points);
    // free(symbol_str);
#ifdef LogDuMem
    LOG("m6 \n");
#endif
    // ll_getNameBackTrace(symbol_str,num_of_addrs);
    pthread_mutex_unlock(&lock);
#ifdef LogDuMem
    LOG("m7 \n");
#endif
    return new_malloc;
#else
    // #define
    return malloc(size_points);
#endif
}

void ll_InitlistGobal()
{
    // List = ll_create_list();
    // InitTime();
    // pthread_create(&id, NULL, &ThreadToTrackTime, NULL);
}

list *removeListInStructAddressMallocInFuntion(void *ptr)
{
    __attribute__((unused)) list *entryTmp = List->head;
    __attribute__((unused)) addressMallocInFuntion *entryTmpPointAddress = entryTmp->addressPoint;
    __attribute__((unused)) addressMallocInFuntion *PreviousPointAddress = entryTmpPointAddress;
    __attribute__((unused)) addressMallocInFuntion *NextPointAddress = NULL;
    __attribute__((unused)) bool checkDone = false;
    // LOG("Em \n");
    while (entryTmp != NULL)
    {
        if (entryTmp->next != NULL)
        {
            NextPointAddress = (addressMallocInFuntion *)(entryTmp->next);
            // LOG("Em \n");
        }
        else
        {
            // LOG("break1 \n");
            break;
        }

        while (entryTmpPointAddress != NULL)
        {
            // LOG("Em1ds \n");
            if (entryTmpPointAddress->point == ptr)
            {
                // LOG("Em2 \n");
                PreviousPointAddress->NextAddressMalloc = NextPointAddress;
                // LOG("Em3 \n");
                munmap(entryTmpPointAddress, sizeof(addressMallocInFuntion));
                // free(entryTmpPointAddress);
                // LOG("Em4 3\n");
                checkDone = true;
                // LOG("Em667 \n");
                entryTmpPointAddress = NextPointAddress;
                break;
            }
            // LOG("Em6 \n");
            PreviousPointAddress = entryTmpPointAddress;
            // LOG("Em5 \n");

            entryTmpPointAddress = entryTmpPointAddress->NextAddressMalloc;
        }
        // LOG("Em \n");
        if (checkDone == true)
        {
            // LOG("Em 12\n");
            list *entryTmp1 = mmap(NULL, sizeof(list), PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            // list *entryTmp1 = malloc(sizeof(list));
            memcpy(entryTmp1, entryTmp, sizeof(list)); // memcmp
            return entryTmp1;
        }
        // LOG("Em 12\n");
        entryTmp = entryTmp->next;
    }
    return NULL;
}

extern void *du_realloc(void *ptr, size_t newSize)
{
    // LOG("count Mem %d",countMemLeck );
#ifdef memleak
#ifdef LogDuMem
    LOG("f1lll \n");
#endif
    pthread_mutex_lock(&lock);
    __attribute__((unused)) list *entryCurrent = List->head;
    __attribute__((unused)) void *checkHead = List->head->addressPoint->point;
    __attribute__((unused)) bool check = false;
    __attribute__((unused)) void *newPtr = NULL;
    size_t sizeMalloc = 1;
#ifdef LogDuMem

    LOG("Point ptrss %p \n", ptr);
#endif

    while (entryCurrent != NULL)
    {
        // LOG("f3 \n");

        __attribute__((unused)) addressMallocInFuntion *addressMalloc = entryCurrent->addressPoint;

        // removeListInStructAddressMallocInFuntion(addressMalloc, p_addr);

        __attribute__((unused)) addressMallocInFuntion *temp;
        __attribute__((unused)) addressMallocInFuntion *CurrentListMallocInFuntion = addressMalloc;
        __attribute__((unused)) addressMallocInFuntion *PreListMallocInFuntion = NULL;
        __attribute__((unused)) addressMallocInFuntion *NextMallocInFuntion = NULL;
        if (CurrentListMallocInFuntion != NULL && entryCurrent->addressPoint->point == ptr)
        {
            // LOG("f4 \n");
            // CurrentListMallocInFuntion=ListMallocInFuntion->NextAddressMalloc;
            entryCurrent->addressPoint = addressMalloc->NextAddressMalloc;
            munmap(CurrentListMallocInFuntion, sizeof(addressMallocInFuntion));
            // LOG("f5 \n");
            check = true;
            // ll_dump();
            // ll_dumpLeak();
            break;
        }
        else
        {
            // LOG("f6 \n");
            while (CurrentListMallocInFuntion != NULL)
            {
                // LOG("f7 \n");
                if (CurrentListMallocInFuntion->point == ptr)
                {
                    // LOG("f8 \n");
                    NextMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
                    sizeMalloc = CurrentListMallocInFuntion->sizePointMalloc;
                    if (PreListMallocInFuntion != NULL)
                    {
                        // LOG("Curenn %p \n  ",CurrentListMallocInFuntion->point);
                        // LOG("Curenn %p ",p_addr);
                        PreListMallocInFuntion->NextAddressMalloc = NextMallocInFuntion;
                    }
                    CurrentListMallocInFuntion->NextAddressMalloc = NULL;
                    CurrentListMallocInFuntion->point = NULL;
                    // free(CurrentListMallocInFuntion);

                    munmap(CurrentListMallocInFuntion, sizeof(addressMallocInFuntion));
                    check = true;
#ifdef LogDuMem
                    LOG("f9ss \n");
#endif
                    break;
                }

                PreListMallocInFuntion = CurrentListMallocInFuntion;
                CurrentListMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
            }
        }

        // if (check == true)
        // {
        //     break;
        // }
        if (entryCurrent->next != NULL)
        {
            entryCurrent = entryCurrent->next;
        }
        else
        {
            break;
        }

#ifdef LogDuMem
        LOG("f3ass \n");
#endif
    }
    // pthread_mutex_unlock(&lock);
    // du_free(ptr);
    // ll_dump();

    if (check == true)
    {
#ifdef LogDuMem

        LOG("f3addsss \n");
#endif
        // if (sizeMalloc == 0)
        // {
        //     TurnUpFreeCountNoTimeTrace(entryCurrent);
        //     TurnUpFreeCountTimeTrace(entryCurrent);
        //     countMemLeck--;
        // }
        // ll_dump();
        addressMallocInFuntion *CurrentListMallocInFuntion1 = entryCurrent->addressPoint;
        if (CurrentListMallocInFuntion1 == NULL)
        {
            addressMallocInFuntion *newMallocAddress = Init_addressPointFuntion();
            // LOG("%d \n", sizeof(ptr));
            // newPtr = (void *)mremap(ptr, sizeMalloc, newSize);
            newPtr = (void *)realloc(ptr, newSize);

            newMallocAddress->NextAddressMalloc = NULL;

            newMallocAddress->point = newPtr;
            newMallocAddress->sizePointMalloc = newSize;
#ifdef LogDuMem
            LOG("f3mmmm \n");
#endif

            // LOG("%p \n",CurrentListMallocInFuntion1->NextAddressMalloc);
            entryCurrent->addressPoint = newMallocAddress;
            pthread_mutex_unlock(&lock);
            return newPtr;
        }
        else
        {
            while (CurrentListMallocInFuntion1->NextAddressMalloc != NULL)
            {
                // if ( (CurrentListMallocInFuntion1->NextAddressMalloc) == NULL)
                // {
                //
                //     break;
#ifdef LogDuMem
                LOG("%p \n", CurrentListMallocInFuntion1->NextAddressMalloc);
#endif

                // }
                CurrentListMallocInFuntion1 = CurrentListMallocInFuntion1->NextAddressMalloc;
                // else
                // {
                //     LOG("%p \n",CurrentListMallocInFuntion1->NextAddressMalloc);
                //     break;
                // }
            }

#ifdef LogDuMem
            LOG("%p \n", CurrentListMallocInFuntion1->point);
#endif

            addressMallocInFuntion *newMallocAddress = Init_addressPointFuntion();
            // void *new_relloc = mmap(NULL, newPtr, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
            // memset(new_relloc, 0, newSize);
            // memcpy(new_relloc, ptr, newSize);
            // newPtr = (void*)mremap(ptr,sizeMalloc, newSize  );
            newPtr = (void *)realloc(ptr, newSize);
#ifdef LogDuMem

            LOG("%p point \n", newPtr);
#endif

            // newMallocAddress->NextAddressMalloc = NULL;
            // ll_dump();
            newMallocAddress->point = (void *)newPtr;
#ifdef LogDuMem

            LOG("f3add jl \n");
#endif

            // LOG("%p kkkkkkk\n",CurrentListMallocInFuntion1->NextAddressMalloc);
            // ll_dump();
            CurrentListMallocInFuntion1->NextAddressMalloc = newMallocAddress;

            pthread_mutex_unlock(&lock);

            return newPtr;
        }
    }
    else
    {
        // ll_dump();

#ifdef LogDuMem
        LOG("Point ptr %p \n", ptr);
        LOG("f2\n");
#endif

        pthread_mutex_unlock(&lock);
        LOG("f2111\n");
        // exit(1);
        return newSize;
    }
#else
    return realloc(ptr, newSize);

#endif
}

extern void *du_calloc(size_t Num, size_t newSize)
{
#ifdef memleak
#ifdef LogDuMem

    LOG("cll \n");
#endif

    pthread_mutex_lock(&lock);
    __attribute__((unused)) int i, j, num_of_addrs;
    __attribute__((unused)) void *buff[num_funtion_callback];
    __attribute__((unused)) char **symbol_str;

    void *new_malloc = calloc(Num, newSize);
    // void *new_malloc = mmap(NULL, Num * newSize, PROT_WRITE | PROT_READ, MAP_PRIVATE | MAP_ANONYMOUS, -1, 0);
    // memset(newSize, 0, Num*newSize);
    if (new_malloc == NULL)
    {
        LOG("Malloc point false");
        exit(1);
    }

    num_of_addrs = backtraceF(buff, num_funtion_callback);
    // LOG("bt element : %d \n", num_of_addrs);

    symbol_str = backtrace_symbolsF(buff, num_of_addrs);
    if (symbol_str == NULL)
    {
        LOG("backtrace_symbols returned NULL \n");
        return NULL;
    }
    // for (j = 0; j < num_of_addrs; j++)
    // {
    //     printf("%s \n", symbol_str[j]);
    // }

    __attribute__((unused)) char *pch;
    if (checkInitListGobal == false)
    {
        List = ll_create_list();
        ++countMemLeck;
        checkInitListGobal = true;
    }
    // else
    ll_create_entry(symbol_str, num_of_addrs, new_malloc, Num * newSize);
    // free(symbol_str);
    // ll_getNameBackTrace(symbol_str,num_of_addrs);
    pthread_mutex_unlock(&lock);
    return new_malloc;
#else

    return calloc(Num, newSize);

#endif
}

extern void *du_free(void *p_addr)
{
#ifdef memleak
#ifdef LogDuMem

    LOG("f1 \n");
#endif
    pthread_mutex_lock(&lock);

    __attribute__((unused)) list *entryCurrent = List->head;
    bool checkOneAddressMalloc = false;
    if (entryCurrent == NULL)
    {
        return 0;
    }

    while (entryCurrent != NULL)
    {
#ifdef LogDuMem
        LOG("f3 \n");
#endif

        __attribute__((unused)) addressMallocInFuntion *addressMalloc = entryCurrent->addressPoint;
        // removeListInStructAddressMallocInFuntion(addressMalloc, p_addr);
        __attribute__((unused)) bool check = false;
        __attribute__((unused)) addressMallocInFuntion *temp;
        __attribute__((unused)) addressMallocInFuntion *CurrentListMallocInFuntion = addressMalloc;
        __attribute__((unused)) addressMallocInFuntion *PreListMallocInFuntion = NULL;
        __attribute__((unused)) addressMallocInFuntion *NextMallocInFuntion = NULL;

        if (CurrentListMallocInFuntion != NULL && CurrentListMallocInFuntion->point == p_addr)
        {
#ifdef LogDuMem
            LOG("f4 \n");
#endif

            entryCurrent->addressPoint = addressMalloc->NextAddressMalloc;
#ifdef LogFreeDuMem
            LOG("Remove a %p\n", p_addr);
#endif

            munmap(CurrentListMallocInFuntion->point, sizeof(void));
            munmap(CurrentListMallocInFuntion, sizeof(addressMallocInFuntion));
            countMemLeck--;
            TurnUpFreeCountNoTimeTrace(entryCurrent);
            TurnUpFreeCountTimeTrace(entryCurrent);
            free(p_addr);
#ifdef LogDuMem
            LOG("f5 \n");
#endif
            // break;
            // return;
        }
        else
        {
#ifdef LogDuMem
            LOG("f6 \n");
#endif
            while (CurrentListMallocInFuntion != NULL)
            {
                // LOG("f7 \n");
                if (CurrentListMallocInFuntion->point == p_addr)
                {
                    // LOG("f8 \n");
                    NextMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
                    if (PreListMallocInFuntion != NULL)
                    {
                        // LOG("Curenn %p \n  ",CurrentListMallocInFuntion->point);
                        // LOG("Curenn %p ",p_addr);
                        PreListMallocInFuntion->NextAddressMalloc = NextMallocInFuntion;
                    }
                    CurrentListMallocInFuntion->NextAddressMalloc = NULL;
                    CurrentListMallocInFuntion->point = NULL;
                    // free(CurrentListMallocInFuntion);
                    munmap(CurrentListMallocInFuntion, sizeof(addressMallocInFuntion));
                    // LOG("%hu \n",entryCurrent ->CountFree);
                    TurnUpFreeCountNoTimeTrace(entryCurrent);
                    TurnUpFreeCountTimeTrace(entryCurrent);
                    // LOG("%hu \n",entryCurrent ->CountFree);
                    countMemLeck--;
                    free(p_addr);

#ifdef LogFreeDuMem
                    LOG("Remove a %p\n", p_addr);
#endif
                    check = true;

                    // LOG("f9 \n");
                    break;
                }
                PreListMallocInFuntion = CurrentListMallocInFuntion;
                if (CurrentListMallocInFuntion->NextAddressMalloc != NULL)
                {
                    CurrentListMallocInFuntion = CurrentListMallocInFuntion->NextAddressMalloc;
                }
                else
                {
                    break;
                }
                // LOG("f1a \n");
                // CurrentListMallocInFuntion = NextMallocInFuntion;
            }
        }
#ifdef LogDuMem
        LOG("f2a33333 \n");
#endif
        if (entryCurrent->next != NULL)
        {
            entryCurrent = entryCurrent->next;
        }
        else
        {
            break;
        }
#ifdef LogDuMem
        LOG("f3adddd \n");
#endif
    }
#ifdef LogDuMem
    LOG("f4890 \n");
#endif
    pthread_mutex_unlock(&lock);
#ifdef LogDuMem
    LOG("f2a-End \n");
#endif

#else
    free(p_addr);
    return 0;
#endif
}

void ThreadToTrackTime()
{
    bool checkLogStart = false;
    LOG("Create Thread Track Time \n");
    // time_t TimeFinished = TimeEndToTrack + TimeStartToTrack;
    // time_t timeStart = TimeStartToTrack ;
    if (TimePrintEveryTime == 0)
    {
        while (1)
        {
            time_t timeCurrent = TimeCurrent();
            if ( timeStart < timeCurrent)
            {
                if(checkLogStart == false)
                {
                    LOG("Start to track leak mem !!! \n\n");
                    checkLogStart = true;
                }
                // sleep(TimePrintEveryTime);
                // ll_dump();
                
                if (timeCurrent >  TimeFinished)   
                {
                    ll_dumpLeak();
                    LOG("End to track leak mem !!! \n\n");
                    exit(1);
                }                
            }
        }
    }
    else
    {
        while (1)
        {
            time_t timeCurrent = TimeCurrent();
            if ( timeStart < timeCurrent)
            {
                if(checkLogStart == false)
                {
                    LOG("Start to track leak mem !!! \n\n");
                    checkLogStart = true;
                }
                sleep(TimePrintEveryTime);
                ll_dump();
                LOG("%u \n",countMemLeck );
                ll_dumpLeak();
                if (timeCurrent >  TimeFinished)   
                {
                    LOG("End to track leak mem !!! \n\n");
                    exit(1);
                }                
            }
        }
    }
        

}

void ll_dump()
{
    __attribute__((unused)) list *tmp = List->head;
    // __attribute__((unused)) list *symbol_str = tmp;
    if (tmp == NULL)
    {
        LOG("Emtry list! \n");
    }
    else
    {
        LOG("\n............. Memory leak ............ \n");

        while (tmp != NULL)
        {
            // LOG("%s \n", tmp->traceFuntion);
            if (tmp->addressPoint != NULL)
            {
                LOG("+> ");
                // for (int i = tmp->numbertrace - 3; i >= 0; --i)
                // {
                //     LOG("%s ", strchr(strtok(strtok(tmp->traceFuntion[i], ") "), "+"), 40) + 1);
                //     if (i > 0)
                //     {
                //         LOG("-> ");
                //     }
                // }
                LOG("%s", tmp->traceFuntion);
                if (tmp != NULL)
                {
                    LOG("\n");
                }
                // LOG("c \n");
                addressMallocInFuntion *tmpAddressMalloc = tmp->addressPoint;
                // LOG("c4 \n");
                while (tmpAddressMalloc != NULL)
                {
                    // LOG("c6 \n");
                    if (tmpAddressMalloc->point != NULL)
                    {
                        // LOG("c9 \n");
                        LOG("\t *Address %p , size %zu  \n", tmpAddressMalloc->point, tmpAddressMalloc->sizePointMalloc);
                        // LOG("c10 \n");
                    }
                    // LOG("c20 \n");
                    if (tmpAddressMalloc->NextAddressMalloc != NULL)
                    {
                        tmpAddressMalloc = tmpAddressMalloc->NextAddressMalloc;
                    }
                    else
                    {
                        break;
                    }

                    // LOG("c15 \n");
                }
            }
            if (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            else
            {
                break;
            }
        }
        LOG("=========== Sum pointer not free: %d ===========\n", countMemLeck);
    }
}

void ll_dumpLeak()
{
    __attribute__((unused)) list *tmp = List->head;
    // __attribute__((unused)) list *symbol_str = tmp;
    if (tmp == NULL)
    {
        LOG("Emtry list! \n");
    }
    else
    {
        LOG("\n............. Memory leak ............ \n");

        while (tmp != NULL)
        {
            // LOG("%s \n", tmp->traceFuntion);
            if (tmp->addressPoint != NULL)
            {
                LOG("+> ");
                // for (int i = tmp->numbertrace - 3; i >= 0; --i)
                // {
                //     LOG("%s ", strchr(strtok(strtok(tmp->traceFuntion[i], ") "), "+"), 40) + 1);
                //     if (i > 0)
                //     {
                //         LOG("-> ");
                //     }
                // }
                LOG("%s", tmp->traceFuntion);
                if (tmp != NULL)
                {
                    LOG("\n");
                }
                LOG("\t *Count_Malloc: %hu \n ", tmp->CountMalloc);
                LOG("\t *Count_Free: %hu \n", tmp->CountFree);
                LOG("\t *Count_Malloc_Track_Time %hu \n", tmp->CountMallocTrackWithTime);
                LOG("\t *Count_Free_Track_Time %hu \n", tmp->CountFreeTrackWithTime);
            }
            if (tmp->next != NULL)
            {
                tmp = tmp->next;
            }
            else
            {
                break;
            }
        }
        LOG("=========== Sum pointer not free: %d ===========\n", countMemLeck);
    }
}

void ll_save_text()
{
    __attribute__((unused)) FILE *fp;
    __attribute__((unused)) list *tmp = List->head;
    __attribute__((unused)) list *symbol_str = tmp;
    fp = fopen("ResultsMemoryLeak.txt", "w");
    if (tmp == NULL)
    {
        fputs("Emtry list! \n", fp);
    }
    else
    {
        fputs("\n....... Memory leak ...... \n", fp);

        while (tmp != NULL)
        {
            if (tmp->addressPoint != NULL)
            {
                fputs("+> ", fp);
                // for (int i = tmp->numbertrace - 3; i >= 0; --i)
                // {

                //     // fprintf(fp, "%s ", strchr(strtok(strtok(tmp->traceFuntion[i], ") "), "+"), 40) + 1);

                //     // if (i > 0)
                //     // {
                //     //     fputs("-> ", fp);
                //     // }
                //     fprintf(fp,"%s",tmp->traceFuntion);
                // if (tmp != NULL)
                // {
                //     fprintf(fp,"\n");
                // }
                // }
                fprintf(fp, "%s", tmp->traceFuntion);
                if (tmp != NULL)
                {
                    fputs("\n", fp);
                }
                addressMallocInFuntion *tmpAddressMalloc = tmp->addressPoint;
                while (tmpAddressMalloc != NULL)
                {
                    if (tmpAddressMalloc->point != NULL)

                    {
                        fprintf(fp, "\t *Address %p \n", tmpAddressMalloc->point);
                    }
                    tmpAddressMalloc = tmpAddressMalloc->NextAddressMalloc;
                }
            }
            tmp = tmp->next;
        }

        fprintf(fp, "=========== Sum pointer not free: %d ===========\n", countMemLeck);
    }

    fclose(fp);
}

void ll_delete_all()
{
    Free_ALL_List(&(List->head));
}
void ll_sig_handler(__attribute__((unused)) int signum)
{

    // Return type of the handler function should be void
    // ll_save_text();
    // printf("\n Save ResultsMemoryLeak.txt success \n");
    // exit(1);
}
void ll_signal()
{
    // __pid_t pid;
    // pid=getpi
    signal(SIGINT, ll_sig_handler);
}

// void create_share_memory()
// {
//     LOG("sdfdsf \n");

//     const char *memname = NAME_MEM_SHARE;

//     int shm_fd = shm_open(memname, O_CREAT | O_TRUNC | O_RDWR, S_IRUSR | S_IWUSR);
//     LOG("%d \n", shm_fd);
//     if (shm_fd < 0)
//     {
//         perror("Erro shm_open !");
//         return EXIT_FAILURE;
//     }
//     ftruncate(shm_fd, SIZE_SHARE_MEM);
// }
