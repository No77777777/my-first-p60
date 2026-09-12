/*  $NetBSD: rtld.h,v 1.71 2004/07/05 11:50:07 cube Exp $    */

/*
 * Copyright 1996 John D. Polstra.
 * Copyright 1996 Matt Thomas <matt@3am-software.com>
 * All rights reserved.
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 * 1. Redistributions of source code must retain the above copyright
 *    notice, this list of conditions and the following disclaimer.
 * 2. Redistributions in binary form must reproduce the above copyright
 *    notice, this list of conditions and the following disclaimer in the
 *    documentation and/or other materials provided with the distribution.
 * 3. All advertising materials mentioning features or use of this software
 *    must display the following acknowledgement:
 *      This product includes software developed by John Polstra.
 * 4. The name of the author may not be used to endorse or promote products
 *    derived from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR ``AS IS'' AND ANY EXPRESS OR
 * IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE IMPLIED WARRANTIES
 * OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE DISCLAIMED.
 * IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY DIRECT, INDIRECT,
 * INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT
 * NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE OF
 * THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

/*==============================================================================
  Copyright (c) 2013, 2020 Qualcomm Technologies, Inc.
  All rights reserved. Qualcomm Proprietary and Confidential.
==============================================================================*/

#ifndef RTLD_H
#define RTLD_H

typedef char* caddr_t;
typedef long _Int32_t;
typedef unsigned int u_int32_t;
typedef unsigned int u_int;
typedef unsigned long long u_quad_t;

#define __strong_alias(x,y) __asm__(".global "  #x  ";\n .set "  #x  ", "  #y ";");
#define __weak_alias(x,y) __asm__(".weak "  #x  ";\n .set "  #x  ", "  #y ";");

#include "dlfcn.h"

#include <yvals.h>
#include <dlfcn.h>
#include <stddef.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/queue.h>
#include "exec_elf.h"
#include "link_elf.h"
#include <sys/mman.h>
#include <stdio.h>

//#ifndef FARF_HIGH
//#define FARF_HIGH 1
//#endif
#ifndef FARF_ERROR
#define FARF_ERROR 1
#endif

#include "HAP_farf.h"

//Debug logging
#define DEBUG_MSG_MAX_LEN 64

#define RTLD_DEBUG(a,b) rtld_debug_log((int)(a),(char*)(b))

#define RTLD_TABLE_MAX_INDEX 15        // Must be one less than a power of two;


#define RTLD_MEM_ALLOC (-39)
#define RTLD_THREAD_CREAT (-40)
#define RTLD_MEM_ALLOC_SERVREG_CB (-41)
#define RTLD_MEM_ALLOC_FSTAT_BUF (-42)
#define RTLD_DALTIMER_ATTACH_FAIL (-43)
#define RTLD_WLAN_IMG_NOT_FOUND (-44)
#define RTLD_RFS_SERVER_ERR (-45)


typedef struct rtld_debug {
   unsigned long long pcycle;
   int err;
   char msg[DEBUG_MSG_MAX_LEN]; //failed msg in some cases
}rtld_debug_info_t;

extern rtld_debug_info_t RTLD_DEBUG_TABLE[RTLD_TABLE_MAX_INDEX+1];

extern unsigned volatile rtld_debug_index;

static inline void rtld_debug_log(int a, char* b)
{
   unsigned long long pcycle;
   int nj;
   int j;

   asm volatile("   %3 = c31:30\n"
                "1: %0 = memw_locked(%4)\n"
                "   %1 = add(%0,#1)\n"
                "   %1 = and(%1,%5)\n"
                "   memw_locked(%4, p0) = %1\n"
                "   if (!p0) jump 1b\n"
                : "=&r" (j) , "=&r" (nj), "+m" (rtld_debug_index), "=&r" (pcycle)
                : "r" (&rtld_debug_index), "r" (RTLD_TABLE_MAX_INDEX)
                : "p0");

   RTLD_DEBUG_TABLE[j].pcycle = pcycle;
   RTLD_DEBUG_TABLE[j].err = a;  //err code
   strlcpy(RTLD_DEBUG_TABLE[j].msg , b , DEBUG_MSG_MAX_LEN); //err msg   
}

#define farf(...)  \
    do { \
        if (_rtld_vtbl.msg) {\
            _rtld_vtbl.msg(__VA_ARGS__); \
        } \
    } while (0)

#define DLERROR(...) \
    do { \
        if (_rtld_vtbl.msg) {\
            _rtld_vtbl.msg( FARF_ERROR_LEVEL, __FILENAME__, __LINE__, ##__VA_ARGS__); \
        } \
        _rtld_error(__VA_ARGS__); \
    } while (0)

#define TRY(exception, func) \
   if (TEST_SUCCESS != (exception = func)) {\
      goto exception##bail; \
   }

#define THROW(exception, errno) \
   exception = errno; \
   goto exception##bail;

#define CATCH(exception) exception##bail: if (exception != 0)

#define VERIFY(exception, val) \
  do { \
    if(0 == (val)) { \
      FARF(ALWAYS,#val"exception: %d", exception);\
      exception = exception == 0 ? -1 : exception; \
      RTLD_DEBUG(exception , "Assertion " #val " failed" ); \
      goto exception##bail; \
    } \
  } while (0)

#define VERIFYM(exception, val, fmt, ...) \
  do { \
    if(0 == (val)) { \
        if (_rtld_vtbl.msg) {\
            _rtld_vtbl.msg( FARF_ERROR_LEVEL, __FILENAME__, __LINE__, fmt, ##__VA_ARGS__); \
        }\
        _rtld_error(fmt, ##__VA_ARGS__);\
		FARF(ALWAYS,fmt, ##__VA_ARGS__);\
		FARF(ALWAYS,"exception: %d", exception);\
      exception = exception == 0 ? -1 : exception; \
      RTLD_DEBUG(exception , fmt ); \
      goto exception##bail; \
    } \
  } while (0)

#define __predict_true(exp) __builtin_expect((exp) != 0, 1)
#define __predict_false(exp)    __builtin_expect((exp) != 0, 0)

#ifndef RTLD_DEFAULT_LIBRARY_PATH
#define RTLD_DEFAULT_LIBRARY_PATH   ""
#endif

extern int _rtld_pagesz;

#ifndef QASSERT
#define QASSERT(st) (void)0
#endif
#ifndef MALLOC
#define MALLOC _rtld_vtbl.malloc
#endif
#ifndef FREE
#define FREE _rtld_vtbl.free
#endif

#ifndef FREEIF
#define FREEIF(p) do { if (p) FREE((void*)(p)), (p) = 0; } while(0)
#endif

#define round_down(x)   ((x) & ~(_rtld_pagesz - 1))
#define round_up(x) round_down((x) + _rtld_pagesz - 1)

/*
 * C++ has mandated the use of the following keywords for its new boolean
 * type.  We might as well follow their lead.
 */
typedef enum {
  false = 0,
  true = 1
}
bool;

struct Struct_Obj_Entry;

typedef struct Struct_Objlist_Entry
{
  SIMPLEQ_ENTRY(Struct_Objlist_Entry) link;
  struct Struct_Obj_Entry* obj;
} Objlist_Entry;

typedef SIMPLEQ_HEAD(Struct_Objlist, Struct_Objlist_Entry) Objlist;

typedef struct Struct_Needed_Entry
{
  struct Struct_Needed_Entry* next;
  struct Struct_Obj_Entry* obj;
  unsigned long name;   /* Offset of name in string table */
} Needed_Entry;

typedef struct _rtld_search_path_t
{
  struct _rtld_search_path_t* sp_next;
  const char* sp_path;
  size_t sp_pathlen;
} Search_Path;

/*
 * Shared object descriptor.
 *
 * Items marked with "(%)" are dynamically allocated, and must be freed
 * when the structure is destroyed.
 */

#define RTLD_MAGIC  0xd550b87a
#define RTLD_VERSION    1
#define RTLD_MAIN   0x800

typedef struct Struct_mmap_info
{
  Elf32_Word va; /* ELF virtual address of segment */
  caddr_t mapbase_x; /* Virtual address of mapped RX region */
  caddr_t mapbase_w;  /* Virtual address of mapped RW region */ // TODO don't need to keep around
  size_t mapsize; /* Size of mapped region in bytes */
  int (*munmap)(void *addr, size_t length, int ctx);
  int (*mprotect)(void* addr, size_t length, int, int ctx);
  int ctx;
} mmap_info;

typedef struct seg_info {
  caddr_t addr; /* virtual address of segment */
  Elf64_Xword memsz; /* Size of segment */
  Elf32_Word flags; /* memory protection flags */
  int (*mprotect)(void* addr, size_t length, int, int ctx);
  int ctx;
} seg_info;

typedef struct Struct_Obj_Entry
{
  struct Struct_Obj_Entry* next;
  char* path; /* Pathname of underlying file (%) */
  int uimg; /* loaded in micro-image */
  int is_static_elf; /*statically linked ELF*/
  int is_dynamic_elf;/*statically linked dynamic ELF*/
  int refcount;
  int dl_refcount; /* Number of times loaded by dlopen */

  /* These items are computed by map_object() or by digest_phdr(). */
  mmap_info* mapbases; /* list of mapped regions */
  int nmapbases;
  seg_info* segs; /* list of load segments */
  int nsegs;
  Elf_Dyn* dynamic; /* Dynamic section */
  caddr_t entry; /* Entry point */
  size_t pathlen; /* Pathname length */

  /* Items from the dynamic section. */
  Elf_Addr* pltgot; /* PLTGOT table */
  const Elf_Rel* rel; /* Relocation entries */
  const Elf_Rel* rellim; /* Limit of Relocation entries */
  const Elf_Rela* rela; /* Relocation entries */
  const Elf_Rela* relalim; /* Limit of Relocation entries */
  const Elf_Rel* pltrel; /* PLT relocation entries */
  const Elf_Rel* pltrellim; /* Limit of PLT relocation entries */
  const Elf_Rela* pltrela; /* PLT relocation entries */
  const Elf_Rela* pltrelalim; /* Limit of PLT relocation entries */
  const Elf_Sym* symtab; /* Symbol table */
  const char* strtab; /* String table */
  unsigned long   strsize; /* Size in bytes of string table */

  const Elf_Word* buckets; /* Hash table buckets array */
  unsigned long   nbuckets; /* Number of buckets */
  const Elf_Word* chains; /* Hash table chain array */
  unsigned long   nchains; /* Number of chains */

  Search_Path* rpaths; /* Search path specified in object */
  Needed_Entry* needed; /* Shared objects needed by this (%) */

  void (*init)(void); /* Initialization function to call */
  void (*fini)(void); /* Termination function to call */

  u_int32_t mainprog: 1, /* True if this is the main program */
            rtld: 1, /* True if this is the dynamic linker */
            textrel: 1, /* True if there are relocations to text seg */
            symbolic: 1, /* True if generated with "-Bsymbolic" */
            printed: 1, /* True if ldd has printed it */
            isdynamic: 1, /* True if this is a pure PIC object */
            mainref: 1, /* True if on _rtld_list_main */
            globalref: 1; /* True if on _rtld_list_global */

  struct link_map linkmap; /* for GDB */

  /* These items are computed by map_object() or by digest_phdr(). */
  const char* interp; /* Pathname of the interpreter, if any */
  Objlist dldags; /* Object belongs to these dlopened DAGs (%) */
  Objlist dagmembers; /* DAG has these members (%) */
  dev_t dev; /* Object's filesystem's device */
  ino_t ino; /* Object's inode number */
  char *core_api_component_name; /* Save CORE API COMPONENT NAME*/
  char *core_api_major_num; /* Save CORE API version major # */
} Obj_Entry;

extern struct r_debug _rtld_debug;
extern Search_Path* _rtld_default_paths;
extern Obj_Entry* _rtld_objlist;
extern Obj_Entry** _rtld_objtail;
extern Obj_Entry* _rtld_objmain;
extern Obj_Entry _rtld_objself;
extern Search_Path* _rtld_paths;
extern bool _rtld_trust;
extern Objlist _rtld_list_global;
extern Objlist _rtld_list_main;
extern Elf_Sym _rtld_sym_zero;
extern DL_vtbl _rtld_vtbl;

/* rtld.c */
int _rtld_set_vtbl(DL_vtbl* vtbl);
void _rtld_error_clear(void);
void _rtld_error(const char*, ...);
extern void _rtld_debug_state(void) __attribute__((used, noinline));

void _rtld_linkmap_add(Obj_Entry *);
void _rtld_linkmap_delete(Obj_Entry *);
void _rtld_objlist_remove(Objlist*, Obj_Entry* obj);
int _rtld_objlist_add(Objlist *, Obj_Entry *);
Objlist_Entry *_rtld_objlist_find(Objlist *, const Obj_Entry *);
const caddr_t _rtld_elfva_to_sysva(const Obj_Entry* obj, Elf32_Word va);
const caddr_t _rtld_elfva_to_sysva_w(const Obj_Entry* obj, Elf32_Word va);

/* headers.c */
int _rtld_digest_dynamic(Obj_Entry *);

/* load.c */
struct map_object_fns {
   size_t (*read)(int ctx, void *, size_t);
   off_t (*lseek)(int ctx, off_t, int);
   void* (*mmap)(void *, size_t, int, int, int, off_t, int ctx);
   int (*munmap)(void *addr, size_t length, int ctx);
   int (*mprotect)(void *, size_t, int, int ctx);
};
Obj_Entry *_rtld_load_object(const char *, int);
int _rtld_load_object_ex(const char*, struct map_object_fns*, int, Obj_Entry**, Elf_Addr*);
int _rtld_load_needed_objects(Obj_Entry *, int);
int _rtld_preload(const char *);
int _rtld_init_builtin(int libc, char *libv[]);
void _rtld_deinit_builtin(void);

/* path.c */
void _rtld_add_paths(Search_Path **, const char *);

/* reloc.c */
int _rtld_relocate_objects(Obj_Entry *, bool);
int _rtld_relocate_nonplt_objects(const Obj_Entry *);
int _rtld_relocate_plt_lazy(const Obj_Entry *);
int _rtld_relocate_plt_objects(const Obj_Entry *);
void _rtld_setup_pltgot(const Obj_Entry *);

/* search.c */
Obj_Entry *_rtld_load_library(const char *, const Obj_Entry *, int);

/* symbol.c */
unsigned long _rtld_elf_hash(const char *);
const Elf_Sym *_rtld_symlook_obj(const char *, unsigned long,
                                 const Obj_Entry *, bool);
const Elf_Sym *_rtld_find_symdef(unsigned long, const Obj_Entry *,
                                 const Obj_Entry **, bool);
const Elf_Sym *_rtld_symlook_list(const char *, unsigned long,
                                  const Objlist *, const Obj_Entry **, bool);
const Elf_Sym *_rtld_symlook_default(const char *, unsigned long,
                                     const Obj_Entry *, const Obj_Entry **, bool);

/* map_object.c */
Obj_Entry *_rtld_map_object(const char *, int, const struct stat*, int);
Obj_Entry *_rtld_map_object_ex(DL_vtbl *, const char *, int, const struct stat *);
Obj_Entry *_rtld_map_object_static_ex(DL_vtbl *, const char *, int, const struct stat *);

void _rtld_obj_free(Obj_Entry*);
Obj_Entry* _rtld_obj_new(void);

#endif /* RTLD_H */
