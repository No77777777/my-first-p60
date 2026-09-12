#include <stdio.h>
#include "dlfcn.h"
#include "qurt.h"

/* Set QuRT User Malloc Size to 0 so there is now heap initialization for the elf loader pd */
unsigned int qurt_usermalloc_size = 0;

unsigned int heapSize = 0x1000;

/* Create Header at spome point */
extern void malloc_imp_init(void);
extern void* malloc_cust (size_t size);
extern void free_cust (void* ptr);

/* Main for the elf loader PD */
int rtld_main(void){

   /* Initialize the libdl library */
   char* builtin[] = {NULL};
   malloc_imp_init();

   DL_vtbl cust_vtbl = {0};
   cust_vtbl.size = 40;
   cust_vtbl.malloc = malloc_cust;
   cust_vtbl.free = free_cust;

   dlinitex(0, builtin, &cust_vtbl);

   /* Tell QuRT to use QuRT RamFS Implementation */
   {
      void qurt_use_qurtfs(int);
      qurt_use_qurtfs(1);
   }

   /* Call dlopen with the User PD name to load the user process */
   //Hard coded 4 for RTLD_ELF_APP for now
   void* h;
   h = dlopen(USER_PD_FILENAME, 4);

   //If reached: error occured - Call Process Exit and don't throw err_fatal
   qurt_qdi_handle_invoke(QDI_HANDLE_GENERIC, QDI_OS_PROCESS_EXIT, 0);
   qurt_thread_stop();

   return 0;
}
