/* these are the definitions included at the beginning
 * of every compiled-to-C module
 */
#ifndef STGDEFS_H
#define STGDEFS_H

/* machine/compiler/system dependencies :-(  Must be first! */
/* OLD: #include "platform.h"  generated by configure */
#include "config.h"   /* generated by configure */
#include "error.h"
#include "StgMachDeps.h"

#include "COptRegs.h"

/* our own ASSERT macro (for C) */
#ifndef DEBUG
#define NDEBUG 1 /* for assert.h */
#define ASSERT(predicate) /*nothing*/

#else
#undef NDEBUG	/* for assert.h */

void _stgAssert PROTO((char *, unsigned int));

#define ASSERT(predicate)			\
	if (predicate)				\
	    /*null*/;				\
	else					\
	    _stgAssert(__FILE__, __LINE__)
#endif

#if macos7_TARGET_OS
#define NON_POSIX_SOURCE
#endif

/* If _NEXT_SOURCE is defined, certain header files make more 
 * constants visible to us.
 * Perhaps it would have been wise, also to define NON_POSIX_SOURCE.
 * Things seemed to work better without it however, so I have not
 * done it. Nevertheless we do the signal stuff in a NON_POSIX way,
 * see StgMacros.lh.
 * CaS
 */
#ifdef nextstep3_TARGET_OS
#define _NEXT_SOURCE
#endif
 
#ifdef NON_POSIX_SOURCE
#undef _POSIX_SOURCE
#undef _POSIX_C_SOURCE
#else
#define _POSIX_SOURCE 1
#define _POSIX_C_SOURCE 199301L
/* Alphas set _POSIX_VERSION (unistd.h) */
/* ditto _POSIX2_C_VERSION
	 _POSIX2_VERSION
	 _POSIX_4SOURCE
*/
#include <unistd.h>
#include <signal.h>
#endif

/* these are all ANSI C headers */
#include <stdlib.h>
#include <string.h>
#include <math.h>
#include <assert.h>
#include <errno.h>
#include <stdio.h>

#ifndef STDC_HEADERS
/* hack: though the following are defined in the ANSI C library,
    they may not be defined in a systems stdio.h (Suns, for example).
    GCC is schizophrenic about whether it adds these into its
    own headers for you; so we have no choice but to throw them in.
*/
int 	fclose PROTO((FILE *));
int 	fflush PROTO((FILE *));
int 	fgetc PROTO((FILE *));
int 	fprintf PROTO((FILE *, const char *, ...));
int 	fputc PROTO((int, FILE *));
int 	fputs PROTO((const char *, FILE *));
size_t	fread PROTO((void *, size_t, size_t, FILE *));
int 	fscanf PROTO((FILE *, const char *, ...));
int 	fseek PROTO((FILE *, long int, int));
size_t	fwrite PROTO((const void *, size_t, size_t, FILE *));
void	perror PROTO((const char *));
int 	printf PROTO((const char *, ...));
int 	puts PROTO((const char *));
int 	remove PROTO((const char *));
int 	rename PROTO((const char *, const char *));
void	rewind PROTO((FILE *));
int	scanf PROTO((const char *, ...));
void	setbuf PROTO((FILE *, char *));
int	setvbuf PROTO((FILE *, char *, int, size_t));
int	sscanf PROTO((const char *, const char *, ...));
/* end of hack */
#endif /* STDC_HEADERS */

#if ! defined(EXIT_SUCCESS) || ! defined(EXIT_FAILURE)
/* "stdlib.h" should have defined these; but at least
   on SunOS 4.1.3, this is not so.
*/
#define EXIT_SUCCESS 0
#define EXIT_FAILURE 1
#endif

/* Make sure that *some* register map is defined */
#ifdef __STG_GCC_REGS__
# if !(defined(NULL_REG_MAP) || defined(FLUSH_REG_MAP) || defined(MAIN_REG_MAP) \
    || defined(MARK_REG_MAP) || defined(SCAN_REG_MAP)  || defined(SCAV_REG_MAP))
yikes! no register map defined!
# endif
#endif

/* Make sure that *some* garbage-collector is expected; GCap is the default */
#if !(defined(GCap) || defined(GC2s) || defined(GC1s) || defined(GCdu) || defined(GCgn))
#define GCap 1
#endif

#ifdef IN_GHC_RTS
#define IF_RTS(x) x
#else
#define IF_RTS(x) /*nothing*/
#endif

/* GNU multi-precision arith */
#include "gmp.h"

/* typedefs for the basic entities */
#include "StgTypes.h"

/* constants about the system known to *everybody* (compiler & RTS) */
#include "GhcConstants.h"

/* macros to deal with stacks (no longer heap) growing in either dirn */
#include "StgDirections.h"

/* declarations for all the runtime flags for the RTS */
#ifdef IN_GHC_RTS
#include "RtsFlags.h"
#endif
/* and those that are visible *everywhere* (RTS + Haskell code) */
struct ALL_FLAGS {
#ifdef TICKY_TICKY
    W_ doUpdEntryCounts;    /* if true, we cannot short-circuit Inds,
				 common-up {Int,Char}Likes or Consts
			    */
#endif
    W_ dummy_entry; /* so there is *something* in it... */
};
extern struct ALL_FLAGS AllFlags;

/* declarations for garbage collection routines */
#include "SMinterface.h"

/* Macros for declaring "registers" and other Optimising stuff */
#include "COptJumps.h"
#include "COptRegs.h"
#include "COptWraps.h"

/* these will come into play if you use -DTICKY_TICKY (default: off) */
#include "Ticky.h"

hash_t hash_str   PROTO((char *str));
hash_t hash_fixed PROTO((char *data, I_ len));

/* ullong (64bit) formatting */
char *ullong_format_string PROTO((ullong x, char *s, rtsBool with_commas));

/* declarations of macros for "high-level instructions" */
#include "StgMacros.h"

/* You always need these */
#include "Info.h"

/* You need these if you are using the threads package or a parallel machine... */
#include "Threads.h"
#include "Parallel.h"

/* Things will happen in here if the driver does -DPROFILING */
#include "CostCentre.h"

/* GRAN and PAR stuff */
#include "GranSim.h"

#if defined(PROFILING) || defined(CONCURRENT)
char * time_str(STG_NO_ARGS);
#endif

/* declarations for runtime-system entry points */
void miniInterpret PROTO((StgFunPtr));
void miniInterpret_debug PROTO(( StgFunPtr, void(*)(STG_NO_ARGS) ));
void miniInterpretEnd(STG_NO_ARGS);

StgFunPtr evaluateMain(STG_NO_ARGS);
StgFunPtr returnMain(STG_NO_ARGS);
StgFunPtr impossible_jump_after_switch(STG_NO_ARGS);

/* hooks: user might write some of their own */
void ErrorHdrHook	PROTO((FILE *));
void OutOfHeapHook	PROTO((W_, W_));
void StackOverflowHook	PROTO((I_));
#ifdef CONCURRENT
void NoRunnableThreadsHook (STG_NO_ARGS);
#endif
void MallocFailHook	PROTO((I_, char *));
void PatErrorHdrHook	PROTO((FILE *));
void PreTraceHook	PROTO((FILE *));
void PostTraceHook	PROTO((FILE *));
void defaultsHook	(STG_NO_ARGS);
void initEachPEHook	(STG_NO_ARGS);

EXTFUN(startStgWorld);
#ifdef CONCURRENT
EXTFUN(CheckHeapCode);
EXTFUN(Continue);
EXTFUN(resumeThread);
#endif

extern char **prog_argv; /* from runtime/main/main.lc */
extern int    prog_argc;
extern char **environ; /* we can get this one straight */

EXTDATA(STK_STUB_closure);

/* now these really *DO* need to be somewhere else... */
char   *time_str(STG_NO_ARGS);
I_	stg_exit PROTO((I_));
I_	_stg_rem PROTO((I_, I_));
char   *stgMallocBytes PROTO((I_, char *));
char   *stgMallocWords PROTO((I_, char *));

/* definitions for closures */
#include "SMClosures.h"

/* definitions for info tables */
#include "SMInfoTables.h"

/* declarations for Update & Indirection stuff */
#include "SMupdate.h"

/* declarations for runtime flags/values */
#define MAX_RTS_ARGS 32

/* Saving and restoring registers */
#include "StgRegs.h"


/*
 * threadWaitWrite# uses FD_SETSIZE to distinguish
 * between read file descriptors and write fd's.
 * Hence we need to include <sys/types.h>, but
 * is this the best place to do it?
 * (the following has been moved from libposix.h)
 */

#ifdef HAVE_SYS_TYPES_H
#include <sys/types.h>
#endif  /* HAVE_SYS_TYPES_H */

#ifndef FD_SETSIZE
# ifdef __FD_SETSIZE
#  define FD_SETSIZE __FD_SETSIZE
# else /* cop out */
#  define FD_SETSIZE 1024
# endif
#endif

#endif /* ! STGDEFS_H */
