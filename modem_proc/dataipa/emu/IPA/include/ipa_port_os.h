#ifndef PORT_OS_H
#define PORT_OS_H
/*
===========================================================================

FILE:         ipa_port_os.h

$Header: //components/rel/dataipa.mpss/3.2/emu/IPA/include/ipa_port_os.h#1 $

===========================================================================

===========================================================================
Copyright © 2012 Qualcomm Technologies Incorporated.
All Rights Reserved.
QUALCOMM Proprietary/GTDR
===========================================================================
*/

/* -----------------------------------------------------------------------
**                           INCLUDES
** ----------------------------------------------------------------------- */

#include "ipa_port.h"
#include <string.h>
#include <stdio.h>


/* -----------------------------------------------------------------------
**                           EXTERNS
** ----------------------------------------------------------------------- */

/*
* C++ wrapper
*/
#ifdef __cplusplus
extern "C" {
#endif

   /**
* @brief   Unaligned Assignment. This type of assignment is not perceived universally
*          by all compilers which are used in VI.
*          This macros protect a RT error in case when Unaligned Assignment is needed.
*          +--------------------------------------------------------------------------+
*          | Unaligned Assignment Example:                                            |
*          |      (*((uint32*)(&qmapHeader))) = QMAP_HEADER_TRANSLATOR(pAddr);        |
*          |          ~~~~~~~ <-- force casting to uint32* is a one of cases.         |   
 *          +-------------------------------------------------------------------------+
* @param   type            the type of the left side of assignment (like uint32 in example).
* @param   toAddress       is address of left object  (like &qmapHeader in example).
* @param   fromValue       the assigned value (like QMAP_HEADER_TRANSLATOR(pAddr) in example).
*
*          +--------------------------------------------------------------------------+
*          | Example:                                                                 |
*          |   CAST_ASSIGN_V2A( uint32, &qmapHeader, QMAP_HEADER_TRANSLATOR(pAddr) ); |
*          +--------------------------------------------------------------------------+
*/
#define CAST_ASSIGN_V2A(type, toAddress, fromValue) \
   {\
      type temporaryVar = fromValue;\
      os_memcpy( toAddress, &temporaryVar, sizeof(type));\
   }

/**
* @brief   Unaligned Assignment. This type of assignment is not perceived universally
*          by all compilers which are used in VI.
*          This macros protect a RT error in case when Unaligned Assignment is needed.
*          +--------------------------------------------------------------------------+
*          | Unaligned Assignment Example:                                            |
*          |       expH = *((uint32*)(&expectedQmapHeader));                          |
*          |               ~~~~~~~ <-- force casting to uint32* is a one of cases.    |   
*          +--------------------------------------------------------------------------+
* @param   type            the type of the left side of assignment (like uint32 in example).
* @param   toAddress       is address of left object  (like &qmapHeader in example).
* @param   fromAddress     the assigned value (like QMAP_HEADER_TRANSLATOR(pAddr) in example).
*
*          +--------------------------------------------------------------------------+
*          | Example:                                                                 |
*          |   CAST_ASSIGN_A2A( uint32, &expH, &expectedQmapHeader );                 |
*          +--------------------------------------------------------------------------+
*/
#define CAST_ASSIGN_A2A(type, toAddress, fromAddress) \
   {\
      os_memcpy( toAddress, fromAddress, sizeof(type));\
   }

/**
 * @fn   int ipa_os_memcmp(const void *s1, const void *s2, uint32 n)
 *
 * @brief   Operating system memcmp. 
 *
 * @param   s1 The first const void *. 
 * @param   s2 The second const void *. 
 * @param   n  Number of bytes to compare. 
 *
 * @return  less than 0 	buffer1 is less than buffer2
 *          equal to 0 	buffer1 is equal to buffer2
 *          greater than 0 	buffer1 is greater than buffer2
*/
int ipa_os_memcmp(const void *s1, const void *s2, uint32 n);

/**
 * @fn   uint32 os_strlen(const char *str)
 *
 * @brief   Operating system strlen. 
 *
 * @param   str   The string. 
 *
 * @return  the length of the string. 
*/
uint32 os_strlen(const char *str);

/**
 * @fn   void *os_memcpy(void *dst, const void *src, uint32 n)
 *
 * @brief   Operating system memcpy. 
 *
 * @param [in,out]   dst   If non-null, destination for the. 
 * @param   src            Source for the. 
 * @param   n              Number of bytes to copy. 
 *
 * @return  null if it fails, else. 
*/
void *os_memcpy(void *dst, const void *src, uint32 n);

/**
 * @brief   Local wrapper for C standard library memmove. Note, it is different from memcpy in tha way that source and destination can overlap.
 *
 * @param   dst   Destination buffer. 
 * @param   src   Source buffer. 
 * @param   size  Number of bytes to copy. 
 */
#define ipa_os_memmove(dst, src, size) memmove(dst, src, size)

/**
 * @fn   void ipa_os_task_entry(thread_func func, void* arg)
 *
 * @brief   Starts thread handling according to the operating system. 
 *
 * @param        func  The thread func. 
 * @param [in]   arg   If non-null, the argument. 
*/
void ipa_os_task_entry(thread_func func, void* arg);

/**
 * @fn   void ipa_os_task_exit(void)
 *
 * @brief   Stops the handling of the thread according to the operating system. 
*/
void ipa_os_task_exit(void);

/**
 * @fn   boolean ipa_os_kbhit(void)
 *
 * @brief   Reports whether a keyboard has been pressed according to the operating system.
 *
 * @return  true if it succeeds, false if it fails. 
*/
boolean ipa_os_kbhit(void);

/**
 * @fn   void ipa_os_getline(char* string, int len)
 *
 * @brief   Reads a string of characters from stdin of the operating system. 
 *
 * @param [in]       string   pointer  to an array of chars where the C string is stored. 
 * @param            len      string length.     
*/
void ipa_os_getline(char* string, int len);

/**
 * @fn   void *os_memset(void *b, int c, uint32 len)
 *
 * @brief   Operating system memset. 
 *
 * @param [out]   b  If non-null, the. 
 * @param   c           The. 
 * @param   len         The length. 
 *
 * @return  null if it fails, else. 
 */
void *os_memset(void *b, int c, uint32 len);

/**
 * @brief   Operating system strcpy. 
 *          Copies the C string pointed by source into the array pointed by destination, 
 *          including the terminating null character (and stopping at that point).
 *
 * @note    To avoid overflows, the size of the array pointed by destination shall be long enough 
 *          to contain the same C string as source (including the terminating null character), 
 *          and should not overlap in memory with source.
 *
 * @param [out]   destination   If non-null, the pointer to destination stream. 
 * @param         source        If non-null, the pointer to source stream.
 *
 * @return  destination is returned, or NULL in case of an error. 
 */
char * os_strcpy ( char * destination, const char * source );

/**
 * @fn   char *ipa_os_strstr(char *instr, char *substr)
 *
 * @brief Searches substr inside instr according to the operating system.
 *
 * The strstr function locates the first occurrence of the string instr in the string substr 
 * and returns a pointer to the beginning of the first occurrence.
 *
 * @param [in]   instr    If non-null, the instr. 
 * @param [in]   substr   If non-null, the substr. 
 *
 * @return  A pointer to the beginning of the first occurrence or null if not found. 
*/
char *ipa_os_strstr(char *instr, char *substr);

/**
 * @brief	Opens the filename pointed to by filename using the given mode.
 *
 * @param [in]   filename	The name of the file to be opened. 
 * @param [in]   mode		The file access mode.
 *
 * @return  A FILE pointer. Otherwise, NULL is returned.
*/
FILE* os_fopen(const char *filename, const char *mode);

/**
 * @brief	Writes data from the array pointed to by ptr to the given stream.
 *
 * @param [in]   ptr		   Pointer to the array of elements to be written.
 * @param [in]   size		Size in bytes of each element to be written.
 * @param [in]   nmemb		Number of elements, each one with a size of size bytes.
 * @param [in]   stream		Pointer to a FILE object that specifies an output stream.
 *
 * @return  The total number of elements successfully written is returned as a size_t object,
 *			which is an integral data type.If this number differs from the nmemb parameter, it will show an error.
*/
size_t os_fwrite(const void *ptr, size_t size, size_t nmemb, FILE *stream);

/**
 * @brief	Gets the next character (an unsigned char) from the specified stream and advances
 *			   the position indicator for the stream.
 *
 * @param [in]	stream      Pointer to a FILE object that identifies the stream on which the operation is to be performed.
 *
 * @return	The character read as an unsigned char cast to an int or EOF on end of file or error.
*/
int os_fgetc(FILE *stream);

/**
 * @brief	Closes the stream. All buffers are flushed.
 *
 * @param [in]	stream		Pointer to a FILE object that specifies the stream to be closed.
 *
 * @return	Returns zero if the stream is successfully closed.On failure, EOF is returned.
*/
int os_fclose(FILE *stream);

/**
 * @brief	Closes the stream. All buffers are flushed.
 *
 * @param [in,out] dest		Pointer to the destination array, which should contain a C string,
 *								   and be large enough to contain the concatenated resulting string.
 * @param			 src		This is the string to be appended. This should not overlap destination.
 *
 * @return	A pointer to the resulting string dest.
*/
char* os_strcat(char *dest, const char *src);

/**
 * @brief   Operating system string compare. 
 *
 * @param   s1 The first const char *. 
 * @param   s2 The second const char *. 
 * @param   n  The length of the strings. 
 *
 * @return  less than 0 	buffer1 is less than buffer2
 *          equal to 0 	buffer1 is equal to buffer2
 *          greater than 0 	buffer1 is greater than buffer2 
*/
int os_strncmp(const char *s1, const char *s2, const uint32 n);

/**
 * @brief   Tests the end-of-file indicator for the given stream. 
 *
 * @param   stream   This is the pointer to a FILE object that identifies the stream.
 *
 * @return  This function returns a non-zero value when End-of-File indicator associated with the stream is set, else zero is returned.
*/
int os_feof(FILE *stream);

/**
 * @brief   Writes any unwritten data in output buffer to the file.
 *          If stream is a null pointer, all streams are flushed.
 *
 * @param   stream   This is the pointer to a FILE object that identifies the stream.
 *
 * @return  A zero value indicates success.
*/
int os_fflush(FILE *stream);

/**
 * @brief   Writes a string to the specified stream up to but not including the null character. 
 *
 * @param   str      This is an array containing the null-terminated sequence of characters to be written.
 * @param   stream   This is the pointer to a FILE object that identifies the stream where the string is to be written.  
 *
 * @return  This function returns a non-negative value else, on error it returns EOF.
*/
int os_fputs(const char *str, FILE *stream);

/**
 * @brief   Gets the current working directory.
 *
 * @param   buffer   Storage location for the path.
 * @param   maxlen   Maximum length of the path in characters 
 *
 * @return  Returns a pointer to buffer. A NULL return value indicates an error.
*/
char* os_getcwd(char *buffer, int maxlen);

/**
 * @brief   Passes the command name or program name specified by command to the host
 *          environment to be executed by the command processor and returns after the command has been completed.
 *
 * @param   command  This is the C string containing the name of the requested variable.
 *
 * @return  The value returned is -1 on error, and the return status of the command otherwise.
*/
int os_system(const char *command);

/**
 * @brief	Reads windows end of line.
 *
 * @param [in]	stream      Pointer to a FILE object that identifies the stream on which the operation is to be performed.
 *
 * @return	EOF on end of file or error otherwise success.
*/
int os_read_windows_EOL(FILE *stream);

/**
 * @brief   Operating system print into a string. 
 *
 * @param [out]   buf   If non-null, the buffer. 
 * @param   size           The size. 
 * @param   fmt            Describes the format to use. 
 *
 * @return  0 upon success, otherwise an error. 
*/
int32 ipa_os_snprintf(char *buf, uint32 size, const char *fmt, ...);

int ipa_os_services_init(void);
void ipa_os_services_uninit(void);
void ipa_os_bootstrap(void);

#ifdef __cplusplus
}
#endif

#endif  /* PORT_OS_H */
