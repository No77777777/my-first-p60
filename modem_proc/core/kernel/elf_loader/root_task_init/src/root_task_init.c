#include "root_task_init.h"
#include "pd_mon_qurt.h"
#include "qurt.h"
#include "DALSys.h"
#include "timer.h"
#include "rcinit.h"
#include <stdio.h>

//Globals
int ramfs_load_done = 0;
timer_type os_timer;

//This routine should periodically  check the status of RMB09 register
void ramfs_detect_timer_cb( void *arg )
{
	unsigned long cmd = RMB09_COMMAND_NOT_SET;
	cmd = HWIO_MSS_RELAY_MSG_BUFFER_09_IN;

	if(cmd != RMB09_COMMAND_NOT_SET)
	{
		//RAMFS init done
		ramfs_load_done = 1;
	}
	else
	{
		//Reload timer
		timer_set_64( &os_timer, RMB09_POLL_PERIOD, 0, T_USEC);
	}
}

void ramfs_detect_task(void)
{

	uint32 rmb_poll_start_time_us, rmb_poll_curr_time_us;
	timer_error_type error_code;

	//rcinit handshake
	rcinit_handshake_startup();

	//init timer
	error_code = timer_def_osal( &os_timer,
                               	 &timer_non_defer_group,
                               	 TIMER_FUNC1_CB_TYPE,
                                 ramfs_detect_timer_cb,
                                 NULL);
	//Start timer
	timer_set_64( &os_timer, RMB09_POLL_PERIOD, 0, T_USEC);

	//Get current time ticks
	rmb_poll_start_time_us= qurt_timer_timetick_to_us(qurt_sysclock_get_hw_ticks_32());

	//Wait till RMB9 register indicates that RAMFS data ELF is loaded
	do
	{
		if(0 == ramfs_load_done)
		{
				//Capture current time ticks
				rmb_poll_curr_time_us = qurt_timer_timetick_to_us(qurt_sysclock_get_hw_ticks_32());

				//If we went past RMB09_POLL_EXPIRY_TIME, and still waiting for SBL, we should bail
				if(rmb_poll_curr_time_us - rmb_poll_start_time_us >= RMB09_POLL_EXPIRY_TIME)
				{
					//Release the timer object
					timer_undef(&os_timer);

					//kill this thread
					qurt_thread_stop();
				}
		}

		//sleep for while
		timer_sleep(RAMFS_DETECT_TASK_SLEEP_PERIOD, T_USEC, 1);

	}while(1 != ramfs_load_done);


	//If we reach this point we detected that SBL is done loading RAMFS
	//Safe to assume RAMFS is ready. Call QuRT Spawn on Elf Loader

	//Release the timer object
	timer_undef(&os_timer);

   	PD_MON_HANDLE hPDElfLoader;
   	hPDElfLoader = pd_mon_spawn(NULL, ELF_LOADER_THREAD_NAME);

}
