#ifndef DLPAGER_QUEUE_H
#define DLPAGER_QUEUE_H



typedef struct dlpager_queue_link_struct {
  struct dlpager_queue_link_struct  *next_ptr;
  struct dlpager_queue_link_struct  *prev_ptr;
} dlpager_queue_link_type;


typedef dlpager_queue_link_type dlpager_queue_head_link_type;

typedef struct dlpager_queue_struct {
  dlpager_queue_head_link_type link;
  int                          cnt;
} dlpager_queue_type;


typedef struct {
   dlpager_queue_link_type link;
} dlpager_queue_generic_item_type;



typedef int (*dlpager_queue_compare_func_type)( void* item_ptr, void* compare_val );



typedef void (*dlpager_queue_action_func_type)( void *item_ptr, void* param );



dlpager_queue_type *dlpager_queue_init ( dlpager_queue_type *dlpager_queue_ptr );


void dlpager_queue_put  ( dlpager_queue_type *dlpager_queue_ptr, dlpager_queue_link_type *link_ptr );


void *dlpager_queue_get ( dlpager_queue_type *dlpager_queue_ptr );



int dlpager_queue_cnt  ( dlpager_queue_type *dlpager_queue_ptr );





void dlpager_queue_delete  ( dlpager_queue_type *dlpager_queue_ptr, dlpager_queue_link_type *dlpager_queue_delete_ptr );




void* dlpager_queue_linear_search(
  dlpager_queue_type             *dlpager_queue_ptr,  
  dlpager_queue_compare_func_type compare_func,
  void                           *compare_val
);



void* dlpager_queue_next  ( dlpager_queue_type *dlpager_queue_ptr, dlpager_queue_link_type *link_ptr );

void* dlpager_queue_check ( dlpager_queue_type *dlpager_queue_ptr );

#endif