#ifndef QURT_TRACE_H
#define QURT_TRACE_H
/*=============================================================================

                 qurt_trace.h -- H E A D E R  F I L E

GENERAL DESCRIPTION
   Prototypes of system call tracing helpers API  

EXTERNAL FUNCTIONS
   None.

INITIALIZATION AND SEQUENCING REQUIREMENTS
   None.

      Copyright (c) 2010
                    by Qualcomm Technologies Incorporated.  All Rights Reserved.

=============================================================================*/



/*=============================================================================
												GLOBAL VARIABLES
=============================================================================*/

unsigned int qurt_trace_get_marker(void);
int qurt_trace_changed(unsigned int prev_trace_marker, unsigned int trace_mask);

/*=============================================================================
												CONSTANTS AND MACROS
=============================================================================*/

#ifndef QURT_DEBUG
#define QURT_TRACE(str, ...) __VA_ARGS__
#else
#define QURT_TRACE(str, ...) \
	do { \
		qurt_printf("%s:%d: %s: >>> calling %s\n",__FILE__,__LINE__,str,#__VA_ARGS__); \
		__VA_ARGS__; \
		qurt_printf("%s:%d: %s: <<< %s returned\n",__FILE__,__LINE__,str,#__VA_ARGS__); \
	} while (0);
#endif

#endif /* QURT_TRACE_H */
