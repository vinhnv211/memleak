#ifndef CONFIG_DU_MEM_H
#define CONFIG_DU_MEM_H
// #ifdef DU_MALLOC_MEM_H

#define LOG(x...) fprintf(stderr, ##x)
// #define LOG printf
#define num_funtion_callback 100
#define SIZE_SHARE_MEM 1024 * 2
#define NAME_MEM_SHARE "nameShare"
#define _GNU_SOURCE

#define LengthNameTraceFuntion 1500
#define LengthOneTraceFuntioin 500

// #ifndef THREAD_TIME_H_
// #define TimeStartTrackMalloc 60*1
// #define TimeEndTrackMalloc 60*2

#define TimeStartToTrack 0// s
#define TimeEndToTrack 60*2
#define TimePrintEveryTime 10

// Enable LOG
#define memleak
// #define LogLinkList
// #define LogDuMem
// #define LogFreeDuMem

#endif
