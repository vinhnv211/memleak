#ifndef BACKTRACEFUNTION_H_
#define BACKTRACEFUNTION_H_ 1
// #include <execinfo.h>
#include <dlfcn.h>
#include <stdlib.h>
#include <unwind.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
// #include <unistd.h>
// #include <dlfcn.h>
#include <link.h>
#include <features.h>
__BEGIN_DECLS

#ifndef trace_arg
struct trace_arg
{
  void **array;
  int cnt, size;
};
#endif
// #define _GNU_SOURCE
#ifndef Dl_info
typedef struct
{
  const char *dli_fname;	/* File name of defining object.  */
  void *dli_fbase;		/* Load address of that object.  */
  const char *dli_sname;	/* Name of nearest symbol.  */
  void *dli_saddr;		/* Exact value of nearest symbol.  */
} Dl_info;
#endif
extern char **backtrace_symbolsF (void *const *array,  int size) __THROW __nonnull ((1));
extern int backtraceF (void **array, int size) __THROW __nonnull ((1));
__END_DECLS
#endif