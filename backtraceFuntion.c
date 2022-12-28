// #include <libgcc_s.h>
// #include <execinfo.h>
// #include <dlfcn.h>
// #include <stdlib.h>
// #include <unwind.h>
// #include <assert.h>
// #include <stdio.h>
// #include <dlfcn.h>
// #include <link.h>
#include "backtraceFuntion.h"
// #include <execinfo.h>
#include <assert.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include <dlfcn.h>
#include <link.h>
// struct trace_arg
// {
//   void **array;
//   int cnt, size;
// };
// #define _GNU_SOURCE
#ifdef SHARED
static _Unwind_Reason_Code (*unwind_backtrace) (_Unwind_Trace_Fn, void *);
static _Unwind_Ptr (*unwind_getip) (struct _Unwind_Context *);

static void backtrace_init (void)
{
	void *handle = dlopen (LIBGCC_S_SO, RTLD_LAZY);

	if (handle == NULL
		|| ((unwind_backtrace = dlsym (handle, "_Unwind_Backtrace")) == NULL)
		|| ((unwind_getip = dlsym (handle, "_Unwind_GetIP")) == NULL)) {
		printf(LIBGCC_S_SO " must be installed for backtrace to work\n");
		abort();
	}
}
#else
# define unwind_backtrace _Unwind_Backtrace
# define unwind_getip _Unwind_GetIP
#endif
#if __ELF_NATIVE_CLASS == 32
# define WORD_WIDTH 8
#else
/* We assyme 64bits.  */
# define WORD_WIDTH 16
#endif
static _Unwind_Reason_Code
backtrace_helper (struct _Unwind_Context *ctx, void *a)
{
	struct trace_arg *arg = a;

	// assert (unwind_getip != NULL);

	/* We are first called with address in the __backtrace function. Skip it. */
	if (arg->cnt != -1)
		arg->array[arg->cnt] = (void *) unwind_getip (ctx);
	if (++arg->cnt == arg->size)
		return _URC_END_OF_STACK;
	return _URC_NO_REASON;
}

/*
 * Perform stack unwinding by using the _Unwind_Backtrace.
 *
 */
extern int backtraceF (void **array, int size)
{
	struct trace_arg arg = { .array = array, .size = size, .cnt = -1 };

#ifdef SHARED
	if (unwind_backtrace == NULL)
		backtrace_init();
#endif

	if (size >= 1)
		unwind_backtrace (backtrace_helper, &arg);

	return arg.cnt != -1 ? arg.cnt : 0;
}




extern char ** backtrace_symbolsF (void *const *array,  int size)
{
	Dl_info info[size];
	int status[size];
	int cnt;
	size_t total = 0;
	char **result;

	/* Fill in the information we can get from `dladdr'.  */
	for (cnt = 0; cnt < size; ++cnt) {
		status[cnt] = (int)dladdr (array[cnt], &info[cnt]);
		if (status[cnt] && info[cnt].dli_fname &&
			info[cnt].dli_fname[0] != '\0')
		/*
		 * We have some info, compute the length of the string which will be
		 * "<file-name>(<sym-name>) [+offset].
		 */
		total += (strlen (info[cnt].dli_fname ?: "") +
				  (info[cnt].dli_sname ?
				  strlen (info[cnt].dli_sname) + 3 + WORD_WIDTH + 3 : 1)
				  + WORD_WIDTH + 5);
		else
			total += 5 + WORD_WIDTH;
	}

	/* Allocate memory for the result.  */
	result = (char **) malloc (size * sizeof (char *) + total);
	if (result != NULL) {
		char *last = (char *) (result + size);
		for (cnt = 0; cnt < size; ++cnt) {
			result[cnt] = last;

			if (status[cnt] && info[cnt].dli_fname
				&& info[cnt].dli_fname[0] != '\0') {

				char buf[20];

				if (array[cnt] >= (void *) info[cnt].dli_saddr)
					sprintf (buf, "+%#lx",
							(unsigned long)(array[cnt] - info[cnt].dli_saddr));
				else
					sprintf (buf, "-%#lx",
					(unsigned long)(info[cnt].dli_saddr - array[cnt]));

				last += 1 + sprintf (last, "%s%s%s%s%s[%p]",
				info[cnt].dli_fname ?: "",
				info[cnt].dli_sname ? "(" : "",
				info[cnt].dli_sname ?: "",
				info[cnt].dli_sname ? buf : "",
				info[cnt].dli_sname ? ") " : " ",
				array[cnt]);
			} else
				last += 1 + sprintf (last, "[%p]", array[cnt]);
		}
		assert (last <= (char *) result + size * sizeof (char *) + total);
	}

	return result;
}

// char ** backtrace_symbols (void *const *array,  int size)
// {
// 	Dl_info info[size];
// 	int status[size];
// 	int cnt;
// 	size_t total = 0;
// 	char **result;

// 	/* Fill in the information we can get from `dladdr'.  */
// 	for (cnt = 0; cnt < size; ++cnt) {
// 		status[cnt] = dladdr (array[cnt], &info[cnt]);
// 		if (status[cnt] && info[cnt].dli_fname &&
// 			info[cnt].dli_fname[0] != '\0')
// 		/*
// 		 * We have some info, compute the length of the string which will be
// 		 * "<file-name>(<sym-name>) [+offset].
// 		 */
// 		total += (strlen (info[cnt].dli_fname ?: "") +
// 				  (info[cnt].dli_sname ?
// 				  strlen (info[cnt].dli_sname) + 3 + WORD_WIDTH + 3 : 1)
// 				  + WORD_WIDTH + 5);
// 		else
// 			total += 5 + WORD_WIDTH;
// 	}

// 	/* Allocate memory for the result.  */
// 	result = (char **) malloc (size * sizeof (char *) + total);
// 	if (result != NULL) {
// 		char *last = (char *) (result + size);
// 		for (cnt = 0; cnt < size; ++cnt) {
// 			result[cnt] = last;

// 			if (status[cnt] && info[cnt].dli_fname
// 				&& info[cnt].dli_fname[0] != '\0') {

// 				char buf[20];

// 				if (array[cnt] >= (void *) info[cnt].dli_saddr)
// 					sprintf (buf, "+%#lx",
// 							(unsigned long)(array[cnt] - info[cnt].dli_saddr));
// 				else
// 					sprintf (buf, "-%#lx",
// 					(unsigned long)(info[cnt].dli_saddr - array[cnt]));

// 				last += 1 + sprintf (last, "%s%s%s%s%s[%p]",
// 				info[cnt].dli_fname ?: "",
// 				info[cnt].dli_sname ? "(" : "",
// 				info[cnt].dli_sname ?: "",
// 				info[cnt].dli_sname ? buf : "",
// 				info[cnt].dli_sname ? ") " : " ",
// 				array[cnt]);
// 			} else
// 				last += 1 + sprintf (last, "[%p]", array[cnt]);
// 		}
// 		assert (last <= (char *) result + size * sizeof (char *) + total);
// 	}

// 	return result;
// }