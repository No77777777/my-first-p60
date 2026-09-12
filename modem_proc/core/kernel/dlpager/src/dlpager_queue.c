#include "assert.h"
#include "dlpager_queue.h"
#include <stdlib.h>




dlpager_queue_type* dlpager_queue_init(dlpager_queue_type  *dlpager_queue_ptr)
{
   ASSERT( dlpager_queue_ptr != NULL );
   dlpager_queue_ptr->link.next_ptr = (dlpager_queue_link_type *)(&dlpager_queue_ptr->link);  /* Points to q link. */
   dlpager_queue_ptr->link.prev_ptr = (dlpager_queue_link_type *)(&dlpager_queue_ptr->link);  /* Points to q link. */

   dlpager_queue_ptr->cnt = 0;

   return dlpager_queue_ptr;
}





void dlpager_queue_put(dlpager_queue_type *dlpager_queue_ptr, dlpager_queue_link_type *link_ptr)
{

   ASSERT( dlpager_queue_ptr != NULL );
   ASSERT( link_ptr != NULL );
   link_ptr->next_ptr = (dlpager_queue_link_type *)&dlpager_queue_ptr->link;
   link_ptr->prev_ptr = dlpager_queue_ptr->link.prev_ptr;

   if(dlpager_queue_ptr->link.prev_ptr != NULL)
   {
    dlpager_queue_ptr->link.prev_ptr->next_ptr = link_ptr;
   }
   dlpager_queue_ptr->link.prev_ptr           = link_ptr;
   dlpager_queue_ptr->cnt++;


   return;
}




void *dlpager_queue_get(dlpager_queue_type *dlpager_queue_ptr)
{
   dlpager_queue_link_type  *link_ptr;

   dlpager_queue_link_type  *ret_ptr = NULL;

   ASSERT( dlpager_queue_ptr != NULL );

   /* Get ptr to 1st queue item. */
   link_ptr = dlpager_queue_ptr->link.next_ptr;

   /* Can only get an item if the queue is non empty */
   if( dlpager_queue_ptr->cnt > 0 )
   {
      dlpager_queue_ptr->link.next_ptr = link_ptr->next_ptr;

      if(link_ptr->next_ptr != NULL)
      {	
         link_ptr->next_ptr->prev_ptr = &dlpager_queue_ptr->link;
      }
     
      dlpager_queue_ptr->cnt--;

      /* Mark item as no longer in a queue. */
      link_ptr->next_ptr = NULL;
      ret_ptr = link_ptr;
      
   }

   return (void *)ret_ptr;
}






void dlpager_queue_delete(dlpager_queue_type *dlpager_queue_ptr, dlpager_queue_link_type *dlpager_queue_delete_ptr)
{
   ASSERT( dlpager_queue_ptr        != NULL );
   ASSERT( dlpager_queue_delete_ptr != NULL );

   if(dlpager_queue_delete_ptr->prev_ptr != NULL)
   {
      dlpager_queue_delete_ptr->prev_ptr->next_ptr = dlpager_queue_delete_ptr->next_ptr;
   }
   if(dlpager_queue_delete_ptr->next_ptr != NULL)
   {
      dlpager_queue_delete_ptr->next_ptr->prev_ptr = dlpager_queue_delete_ptr->prev_ptr;
   }

   dlpager_queue_ptr->cnt--;
   dlpager_queue_delete_ptr->next_ptr = NULL;

   return;
}



int dlpager_queue_cnt(dlpager_queue_type  *dlpager_queue_ptr)
{
   ASSERT( dlpager_queue_ptr != NULL );
   return dlpager_queue_ptr->cnt;
}



void* dlpager_queue_linear_search(dlpager_queue_type *dlpager_queue_ptr, dlpager_queue_compare_func_type compare_func, void *compare_val)
{
   dlpager_queue_generic_item_type *item_ptr = NULL;

   ASSERT( compare_func != NULL );
   item_ptr = (dlpager_queue_generic_item_type*)dlpager_queue_check( dlpager_queue_ptr );

   while( item_ptr != NULL )
   {
      if( compare_func( item_ptr, compare_val ) != 0 )
      {
         return item_ptr;
      }
      item_ptr = (dlpager_queue_generic_item_type*)dlpager_queue_next( dlpager_queue_ptr, &item_ptr->link );
   }

   return NULL;
}






void* dlpager_queue_check(dlpager_queue_type  *dlpager_queue_ptr)
{
   dlpager_queue_link_type  *link_ptr;

   dlpager_queue_link_type  *ret_ptr = NULL;
   
   ASSERT( dlpager_queue_ptr != NULL );

   link_ptr = dlpager_queue_ptr->link.next_ptr;

   if( dlpager_queue_ptr->cnt > 0 )
   {
      ret_ptr = link_ptr;
   }
   
   return (void *)ret_ptr;
}



void* dlpager_queue_next(dlpager_queue_type *dlpager_queue_ptr, dlpager_queue_link_type *dlpager_queue_link_ptr)
{
   void *dlpager_queue_temp_ptr = NULL;
   ASSERT( dlpager_queue_link_ptr != NULL );
   if( (void *) dlpager_queue_link_ptr->next_ptr != (void *) dlpager_queue_ptr )
   {
      dlpager_queue_temp_ptr = dlpager_queue_link_ptr->next_ptr;
   }
   return dlpager_queue_temp_ptr;
}
