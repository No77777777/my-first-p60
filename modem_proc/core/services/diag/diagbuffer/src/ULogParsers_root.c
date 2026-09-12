/*============================================================================
@file ULogParsers.c

This module implements the parsing of ULOG data from different ULOG interfaces.

Copyright (C) 2009-2013, 2016 by Qualcomm Technologies, Inc.
All Rights Reserved.
Qualcomm Confidential and Proprietary

$Header: //components/rel/core.mpss/10.0/services/diag/diagbuffer/src/ULogParsers_root.c#1 $
============================================================================*/
#include "ULogBuildFeatures.h"

#ifdef __cplusplus
extern "C" {
#endif

#ifndef DEFEATURE_ULOG_MSG_FORMAT
#include "DALSys.h"
#include "ULog.h"
#include "ULogCoreInternal.h"
#include "CoreString.h"
#include "CoreTime.h"
#include "diagbuffer_int.h"

/** A minimum realtime message has a len/fmt word + a 4 or 8 byte timestamp*/
#ifdef ULOG_64BIT_TIME
  #define MIN_REALTIME_MSG_LEN 12
#else
  #define MIN_REALTIME_MSG_LEN 8
#endif

/** For sprintfs of 32 bit numbers, what is a safe minimum number of available bytes needed */
#define SNPRINTF_MIN_SIZE 14


/**
 * Macros for adding characters to strings. These operations are done a lot,
 * so these help make the code a little more compact and hopefully easier to read.
 * To avoid lots of strcats and copies we mostly do snprintf at the end of the
 * existing strings.
 */
//no argument "...." string only type sprintf string
#define SNPRINTF_AND_MOVEPTRS1(x) {unsigned int bytesOut; bytesOut = core_snprintf(&(parse_data->outputsz[parse_data->outOffset]), parse_data->outputsz_size, x); parse_data->outOffset += bytesOut;  parse_data->outputsz_size -= bytesOut;}
//one argument  "..%x.",value type string
#define SNPRINTF_AND_MOVEPTRS2(x, y) {unsigned int bytesOut; bytesOut = core_snprintf(&(parse_data->outputsz[parse_data->outOffset]), parse_data->outputsz_size, x, y); parse_data->outOffset += bytesOut; parse_data->outputsz_size -= bytesOut;}
//add a single char to the string.
#define ADDCHAR_AND_MOVEPTRS(x) {parse_data->outputsz[parse_data->outOffset]=x; parse_data->outOffset+=1; parse_data->outputsz[parse_data->outOffset]=0; parse_data->outputsz_size--;}

/* Structs for diagbuffer header, log, event and f3 types */
typedef PACK( struct )
{
  uint16 format;
  uint16 length;
  uint64 timetick;
}
diagbuffer_diag_pkts_hdr_type;

typedef PACK( struct )
{
  uint16 event_id;
  uint8 version;
  uint8 pad;
}
diagbuffer_event_type;

typedef PACK( struct )
{
  uint16 log_code;
  uint8 version;
  uint8 pad;
}
diagbuffer_log_type;

typedef PACK( struct )
{
  uint8 num_args;
  uint8 version;
  uint16 pad;
}
diagbuffer_msg_type;

typedef PACK( struct )
{
  uint8 version;
  uint8 pad[3];
}
diagbuffer_msg_sprintf_type;

#define DIAGBUFFER_FULL_TIME_STAMP 0

void diagbuffer_parse_diag_data_event(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length, uint32 msgFormat);
void diagbuffer_parse_diag_data_log(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length);
void diagbuffer_parse_diag_data_msg_qsr4(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length);
void diagbuffer_parse_diag_data_msg_sprintf(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length);


/**
 * <!-- ULogCore_MsgFormat -->
 *
 * @brief Format an individual message retrieved with the ULogCore_Read function.
 *
 * @param h : A ULog handle to the log we are formatting.
 * @param msg : The unformatted binary msg data
 * @param msgString : The memory to be filled in with the output string.
 * @param msgStringSize : The size of the msgString memory.
 * @param msgConsumed : Number of bytes consumed from the msg data buffer.
 *
 * @return DAL_SUCCESS if the format was successful. Errors are
 *        defined in the ULOG_ERRORS enum.
 */
ULogResult ULogCore_MsgFormat( ULogHandle h,
                               char *  msg,
                               char *  msgString,
                               uint32 msgStringSize,
                               uint32 * msgConsumed )
{
  ULOG_TYPE * log = (ULOG_TYPE *)h;
  return(ULogParsers_FormatMsg(log, msg, msgString, msgStringSize, msgConsumed));
}


/**
 * <!-- isNotConversionChar -->
 *
 * @brief Used for finding the end of strings like %08x.
 *
 * When a format letter like x or d is found the format string is done.
 *
 * @param fmtChar : Character to check.
 *
 * @return TRUE if the character is not a conversion character
 */
static uint32 isNotConversionChar(char fmtChar)
{
  const char convChar[] = "%EGXcdefgimnopstux";
  int high, mid, low;

  low = 0;
  high = sizeof(convChar)-1;
  while (low <= high)
  {
    mid = (low + high) / 2;
    if (fmtChar == convChar[mid])
    {
      return(FALSE);
    }
    else if (fmtChar > convChar[mid])
    {
      low = mid+1;
    }
    else
    {
      high = mid-1;
    }
  }
  return(TRUE);
}


/**
 * <!-- Add_InsufficientSpaceMessage -->
 *
 * @brief Indicate there's text in the log message we couldn't add to the string.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct used to pass
 *                     around the msg data and character string being working on.
 */
static void Add_InsufficientSpaceMessage( ULOG_READ_TO_ASCII_DATA* parse_data )
{
  //Put an error-like message in the buffer if possible.
  if (parse_data->outputsz_size > strlen("<...>"))
  {
    SNPRINTF_AND_MOVEPTRS1("<...>");
    return;
  }
  return;
}

/**
 * <!-- Add_BadVectorParseMessage -->
 *
 * @brief Indicate there's an invalid formatstring in the log message
 *        we couldn't add to the string.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct used to pass
 *                     around the msg data and character string being working on.
 */
static void Add_BadVectorParseMessage( ULOG_READ_TO_ASCII_DATA * parse_data )
{
  //Put an error-like message in the buffer if possible.
  if ( parse_data->outputsz_size > strlen( "<--->" ) )
  {
    SNPRINTF_AND_MOVEPTRS1( "<--->" );
    return;
  }
  return;
}


/**
 * <!-- UlogReadNextMsgPart -->
 *
 * @brief Read the requested number of bytes from the msg data
 *
 * Because the data can come from 2 different arrays (part1 or part2) we use this
 * function to read the next amount of data from whatever source still has data.
 *
 * @param dest : Pointer to destination memory
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct used to pass
 *                     around the msg data and character string being working on
 * @param bytesToRead : Usually 1, 4 or 8 bytes.
 *
 * @return TRUE if data was read.
 */
static uint32 UlogReadNextMsgPart( unsigned char* dest,
                                   ULOG_READ_TO_ASCII_DATA* parse_data,
                                   unsigned int bytesToRead )
{
  if (parse_data->part1_size>=bytesToRead)
  {
    core_memcpy(dest, bytesToRead, parse_data->msgpart1, bytesToRead);
    parse_data->part1_size -= bytesToRead;
    parse_data->msgpart1 += bytesToRead;
    return TRUE;
  }
  if (parse_data->part2_size>=bytesToRead)
  {
    core_memcpy(dest, bytesToRead, parse_data->msgpart2, bytesToRead);
    parse_data->part2_size -= bytesToRead;
    parse_data->msgpart2 += bytesToRead;
    return TRUE;
  }
  return FALSE;
}


/**
 * <!-- UlogSnoopNextMsgPart -->
 *
 * @brief Read the next value out of the arrays, but leave the data in the queue.
 *
 * @param dest : Pointer to destination memory
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                    and character string being working on
 * @param bytesToRead : Usually 4 bytes
 *
 * @return TRUE if data was read.
 */
static uint32 UlogSnoopNextMsgPart( unsigned char* dest,
                                    ULOG_READ_TO_ASCII_DATA* parse_data,
                                    unsigned int bytesToRead )
{
  if (parse_data->part1_size>=bytesToRead)
  {
    core_memcpy(dest, bytesToRead, parse_data->msgpart1, bytesToRead);
    return TRUE;
  }
  if (parse_data->part2_size>=bytesToRead)
  {
    core_memcpy(dest, bytesToRead, parse_data->msgpart2, bytesToRead);
    return TRUE;
  }
  return FALSE;
}


/**
 * <!-- UlogMsgRemainingBytes -->
 *
 * @brief Add up the part1 and part2 sizes in the parse_data buffer.
 *
 * @param parse_data: Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                    and character string being working on
 * @return The number of bytes left to read in the parse_data structure.
 */
static unsigned int UlogMsgRemainingBytes(ULOG_READ_TO_ASCII_DATA* parse_data)
{
  return parse_data->part1_size + parse_data->part2_size;
}


/**
 * <!-- HandleNextMultipartMsg -->
 *
 * @brief Process a multipart message, somewhat recursively.
 *
 * Multipart Messages(abreviated MPMs) always take the form of a
 * ULogFront_RealTimePrintf with one or more %m format specifiers in the
 * format string, followed by the ULogFront_RealTime* log messages that
 * will fill in the %m's.
 *
 * All parse functions work on a ULOG_READ_TO_ASCII_DATA structure.
 * This function creates another one of these structures to isolate
 * individual MPM parts (single ULOG messages) and parse/add them together.
 * The function uses mpm_parse_data to point to the "next" part of the
 * composite MPM message.  Called again, it moves the next to the front
 * and sets up a new "next".
 *
 * MPM data is always in part1 of the ULOG_READ_TO_ASCII_DATA so that makes the
 * pointer moves possible in this MPM parsing.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on
 * @param mpm_parse_data : NULL at the start of a MPM, the next/remaining msg
 *                         data on subsequent calls.
 */
static void HandleNextMultipartMsg( ULOG_READ_TO_ASCII_DATA* parse_data,
                                    ULOG_READ_TO_ASCII_DATA* mpm_parse_data )
{
  uint32 padding = 0;
  uint32 length;

  // Used in the recursive calls to keep track of the total MPM and break out
  // individual msgs to parse.
  ULOG_READ_TO_ASCII_DATA new_parse_data;

  if (mpm_parse_data==NULL){
     //It's the start of a new MPM. The format and the TS words, something like:
     //[00600007] [1f7c1337 00000000]  have already been consumed.
     //The next bytes in the parse_data are the first msg (typically a printf)

    //Make sure there's at least one msg remaining to parse.
    if (UlogMsgRemainingBytes(parse_data) < MIN_REALTIME_MSG_LEN){
      return;
    }

    // Read the length of the next message to prepare to arrange the parse data structures
    UlogSnoopNextMsgPart((unsigned char*)&length, parse_data, 4);
    length = (length & ULOG_LENGTH_MASK) >> ULOG_LENGTH_SHIFT;

    //Start filling in a new structure to point to the message following the next one
    new_parse_data.msgpart1 = parse_data->msgpart1;
    new_parse_data.msgpart2 = 0;
    new_parse_data.part1_size = parse_data->part1_size;
    new_parse_data.part2_size = 0;
    new_parse_data.outputsz = parse_data->outputsz;
    new_parse_data.outputsz_size = parse_data->outputsz_size;  //the total size for the MPM
    new_parse_data.outOffset = parse_data->outOffset;
    new_parse_data.environment = ULOG_PARSE_READ_DATA;        //decoding a stored log, not a data stream.

    //Now that we've backed up the key info for this MPM, we repurpose the
    //parse_data structure to point to only the next ULog msg. Typically
    //this message is a ULogFront_RealTimePrintf with one or more "%m" in
    //the format string.
    //Set the length in parse_data to ONLY the length of the next msg.
    parse_data->part1_size = length;
    parse_data->part2_size = 0;

    // Advance the mpm parse data we filled in earlier to the end of the next
    // message.  msgs only start on word boundaries.
    if ((length&0x3)!=0)
    {
      padding = (length&0x3);  //messages always start on a word boundary.  Calculate if padding is needed.
    }
    length = length + padding;  // Find the start of the next message.

    //Set the length in new_parse_data parse_data for all of the remaining data
    //after the msg we're about to parse.
    new_parse_data.msgpart1 += length;
    new_parse_data.part1_size -= length;

    //parse_data points to the first part of the MPM (almost always a ULOG_REALTIME_SUBTYPE_PRINTF msg)
    //and new_parse_data is the log msg to fill the first %m msg afterwards.
    ULogParsers_FormatParseData(parse_data, &new_parse_data);
  }
  else
  {
     // A MPM has begun and we're now at a %m in a printf.
     // mpm_parse_data was previously setup to point to the next msg to process
     // move forward to parse the next part of the message and update pointers

    //make sure there's remaining msgs to process.
    if (UlogMsgRemainingBytes(mpm_parse_data) < MIN_REALTIME_MSG_LEN)
    {
      if (parse_data->outputsz_size > strlen("<Mutipart Err>"))
      {
        SNPRINTF_AND_MOVEPTRS1("<Mutipart Err>");
      }
      return;
    }

    //read the length in the next msg
    UlogSnoopNextMsgPart((unsigned char*)&length, mpm_parse_data, 4);
    length = (length & ULOG_LENGTH_MASK) >> ULOG_LENGTH_SHIFT;

    //create a parse data structure to setup the read of the next msg
    new_parse_data.msgpart1 = mpm_parse_data->msgpart1;  //next msg pointer that we recorded earlier
    new_parse_data.msgpart2 = NULL;
    new_parse_data.part1_size = length;
    new_parse_data.part2_size = 0;
    //pass the current string info for the next parse to add to.
    new_parse_data.outputsz = parse_data->outputsz;
    new_parse_data.outputsz_size = parse_data->outputsz_size;
    new_parse_data.outOffset = parse_data->outOffset;
    new_parse_data.environment = ULOG_PARSE_READ_DATA;        //decoding a stored log, not a data stream.

    // Update the total MPM data structure now that we're about to process
    // another msg from it. Advance the data pointer to the end of the
    // msg we're about to process.
    if ((length&0x3)!=0){
      padding = 4 - (length&0x3);  //messages always start on a word boundary.  Calculate if padding is needed.
    }
    length = length + padding;  // Find the start of the next message.

    //advance the pointer to the next start
    mpm_parse_data->msgpart1 = mpm_parse_data->msgpart1 + length;
    //reduce the remaining size by the amount we're about to process.
    mpm_parse_data->part1_size -= length;
    ULogParsers_FormatParseData(&new_parse_data, mpm_parse_data);

    //after the return, update the string values of the original parse_data
    parse_data->outOffset = new_parse_data.outOffset;
    parse_data->outputsz_size = new_parse_data.outputsz_size;
  }

  return;
}


/**
 * <!-- ULFE_RealTimePrintfParse -->
 *
 * @brief Do the character by character printf parse work.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param mpm : While parsing a MPM this additional data is passed around to
 *              hold MPM data.
 */
static void ULFE_RealTimePrintfParse( ULOG_READ_TO_ASCII_DATA* parse_data,
                                       ULOG_READ_TO_ASCII_DATA* mpm )
{
  char * fmtString;
  #define SUBFMT_MAXSIZE 128
  char subFmtString[SUBFMT_MAXSIZE];
  char * fmtPtr;      //pointer to read individual characters inside the format string
  char * subOutPtr;   //pointer to access individual characters inside the subformat string
  char is64BitValue = 0;
  uint32 localWord;
  uint64 local64Word;
  char *isRef;        //Indicate if a subformat is a reference data


  // Make sure the inputs look ok.
  if((parse_data->outputsz_size==0)||  //there's space to put the output string
     (parse_data->part1_size==0)||     //there's data to read (printfs are only ever in the part1 data
     (parse_data->msgpart1==NULL)||    //the input pointer looks reasonable
     (parse_data->outputsz==NULL))     //the outputsz pointer looks reasonable
  {
    return;
  }

  // Read in the pointer to the format string
  UlogReadNextMsgPart((unsigned char*)&localWord, parse_data, 4);
  if (localWord == 0)
  {
    localWord = (uint32)"<NULL REFERENCE FORMAT STRING>";
  }

  fmtString = (char *)localWord;

  // Initialize the pointer used to move around format the string.
  fmtPtr = fmtString;

  // Parse the string and add in other values.
  while ((*fmtPtr) &&                     //the string pointer is not NULL
         (parse_data->outputsz_size))    //there is space in the outputsz buffer
  {
    if (*fmtPtr == '%')
    {
      // -- We are starting a % sub format string.  Copy and parse it. --
      subOutPtr = subFmtString;

      //start with the assumption that anything we parse will be a normal 32 bit value.
      is64BitValue=0; //flag this value as a 32 bit value
      do
      {
        *subOutPtr++ = *fmtPtr++;
        //repeat the loop if the last character read is not a conversion char,
        //we haven't hit the end of the string, and we haven't exceeded our
        //subformat buffer for this copy
      } while(isNotConversionChar(*fmtPtr)&&(*fmtPtr)&&(((uint32)subOutPtr-(uint32)subFmtString)<(SUBFMT_MAXSIZE-1)));
      *subOutPtr++ = *fmtPtr;
      *subOutPtr = 0;  //null terminate subformat the string

      if (*fmtPtr == '%')
      {
        // There was % following the % instead of an expected conversion character.
        if (parse_data->outputsz_size > strlen(" ")){
           ADDCHAR_AND_MOVEPTRS(' ');
         }
      }
      else
      {
        if (*fmtPtr == 't')
        {
          // Format specifier for current time stamp. Current time requested
#ifdef ULOG_64BIT_TIME
          local64Word = CoreTimetick_Get64();
          core_strlcpy(subFmtString, "0x%016X", SUBFMT_MAXSIZE);
          is64BitValue=1; //flag this value as a 64 bit value
#else
          localWord = CoreTimetick_Get();
          core_strlcpy(subFmtString, "0x%08X", SUBFMT_MAXSIZE);
#endif
        }
        else if (*fmtPtr == 'm')
        {

          if (parse_data->environment == ULOG_PARSE_DATA_STREAM){
            if (parse_data->outputsz_size > strlen("<mpm>"))
            {
              SNPRINTF_AND_MOVEPTRS1("<mpm>");
              subFmtString[0] = 0;
            }
          }
          else if (mpm == NULL)
          {
            if (parse_data->outputsz_size > strlen("<Mutipart Err>"))
            {
              SNPRINTF_AND_MOVEPTRS1("<Mutipart Err>");
              subFmtString[0] = 0;
            }
          }
          else if (parse_data->environment == ULOG_PARSE_READ_DATA)
          {
            HandleNextMultipartMsg(parse_data, mpm);
            subFmtString[0] = 0;
          }
        }
        else
        {
          // The format char refers to something that has to be pointed to.  Make sure there are bytes
          // enough in this message where there could be a pointer.
          if (parse_data->part1_size >= 4)
          {
            /* Check whether the 64 bit value is an address or data */
            if ((strstr(subFmtString,"llu")!=0)||(strstr(subFmtString,"lld")!=0)||(strstr(subFmtString,"llx")!=0)||(strstr(subFmtString,"llX")!=0))
            {
              //It's a 64 bit value.
              isRef = strchr(subFmtString, '&');
              if (isRef != NULL)
              {
                unsigned int i;

                //read in the pointer
                UlogReadNextMsgPart((unsigned char*)&localWord, parse_data, 4);

                // Fix the string by removing the & from the subFmtString
                // isRef points to & in the string, so copy to that location
                // from one character later.
                for (i=0; i<strlen(isRef); i++){
                  isRef[i] = isRef[i+1];
                }

                /* Dereference the pointer to get the data */
                if (localWord != 0)
                {
                   local64Word = *(uint64 *)localWord;
                   is64BitValue=1; //flag this value as a 64 bit value
                }
                else
                {
                  if (parse_data->outputsz_size > strlen("<Format Specified Without Arg>"))
                  {
                    SNPRINTF_AND_MOVEPTRS1("<Format Specified Without Arg>");
                    return;
                  }
                  else
                  {
                    //no space to add an error message, just return.
                    return;
                  }
                }
              }else
                //read the 64 bit value from the buffer.
                UlogReadNextMsgPart((unsigned char*)&local64Word, parse_data, 8);
                is64BitValue=1; //flag this value as a 64 bit value
            }
            else
            {
              // Not a 64 bit value. Do a normal 32 bit read
              is64BitValue=0; //make sure the 64 bit flag is not set.
              // Read in the pointer referred to us by the % format character.
              UlogReadNextMsgPart((unsigned char*)&localWord, parse_data, 4);
              /* Check whether the 32 bit value is an address or data */
              isRef = strchr(subFmtString, '&');
              if (isRef != NULL)
              {
                unsigned int i;
                /* The 32-bit value is an address. Dereference it to get the data */
                if (localWord != 0)
                {
                  localWord = *(uint32 *)localWord;
                } else {
                  if (parse_data->outputsz_size > strlen("<NULL pointer & reference>"))
                  {
                    SNPRINTF_AND_MOVEPTRS1("<Format Specified Without Arg>");
                    return;
                  }
                  else
                  {
                    //no space to add an error message, just return.
                    return;
                  }
                }
                // Fix the string by removing the & from the subFmtString
                // isRef points to & in the string, so copy to that location
                // from one character later.
                // memcpy(isRef, isRef+1, strlen(isRef));
                for (i=0; i<strlen(isRef); i++){
                  isRef[i] = isRef[i+1];
                }
              }
            }
          }
          else
          {
            // Got a % format, but there's no space remaining for pointers. Must be a malformed printf.
            // Put an error message in the buffer if possible.
            if (parse_data->outputsz_size > strlen("<Format Specified Without Arg>"))
            {
              SNPRINTF_AND_MOVEPTRS1("<Format Specified Without Arg>");
              return;
            }
          }
        }
        if ((*fmtPtr == 's') && (localWord == 0))
        {
          localWord = (uint32)"<NULL REFERENCE>";
        }
        else if (*fmtPtr == 'f')
        {
          // We must print a floating point value.  Extend it appropriately.
          FloatConverter f;
          f.u.uiData[0] = localWord;
          if ((parse_data->outputsz_size) > SNPRINTF_MIN_SIZE) //ensure there's enough space to write the string
          {
            SNPRINTF_AND_MOVEPTRS2(subFmtString, f.u.fData);
          }
          else
          {
            Add_InsufficientSpaceMessage(parse_data);
            return;
          }
        }
        else
        {
          //unless the special 'f' or 's' handling above is used, this is where most of the format string printing is done.
          if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the smallest string
            //recheck that we really think this is a printable format string
            if (isNotConversionChar(*fmtPtr)==FALSE)
            {
              if (is64BitValue==0)
              {
                if(*fmtPtr=='s')
                {
                  if ((parse_data->outputsz_size) > strlen((char*)localWord)) //ensure there's enough space to write the string
                  {
                    SNPRINTF_AND_MOVEPTRS2(subFmtString, localWord);
                  } else {
                    Add_InsufficientSpaceMessage(parse_data);
                    return;
                  }
                }
                else
                {
                  if ((parse_data->outputsz_size) > SNPRINTF_MIN_SIZE) //ensure there's enough space to write the string
                  {
                    SNPRINTF_AND_MOVEPTRS2(subFmtString, localWord);
                  } else {
                    Add_InsufficientSpaceMessage(parse_data);
                    return;
                  }
                }
              }
              else
              {
                //it's a 64 bit value
                if ((parse_data->outputsz_size) > SNPRINTF_MIN_SIZE*2) //ensure there's enough space to write the string
                {
                  SNPRINTF_AND_MOVEPTRS2(subFmtString, local64Word);
                } else {
                  Add_InsufficientSpaceMessage(parse_data);
                  return;
                }
              }
            }
            else
            {
              if (parse_data->outputsz_size > strlen("<bad printf format>")){
                SNPRINTF_AND_MOVEPTRS1("<bad printf format>");
              }
            }
          }
          else
          {
            Add_InsufficientSpaceMessage(parse_data);
            return;
          }
        }
      }
      fmtPtr++;
    }
    else
    {
      // -- Not a % sub format string.  Copy the text to the outputsz buffer. --
      if (parse_data->outputsz_size > 2)
      {
        ADDCHAR_AND_MOVEPTRS(*fmtPtr);
      }
      fmtPtr++;
    }
  }

  return;
}


/**
 * <!-- ULFE_RealTimeCharArrayParse -->
 *
 * @brief Do the RealTimeCharArray to string parse work.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param mpm : While parsing a MPM this additional data is passed around to
 *              hold MPM data.
 */
static void ULFE_RealTimeCharArrayParse( ULOG_READ_TO_ASCII_DATA* parse_data,
                                         ULOG_READ_TO_ASCII_DATA* mpm )
{
  uint32 columns = 0;
  unsigned char value;

  while(UlogReadNextMsgPart((unsigned char*)&value, parse_data, 1))
  {
    if (columns == 24)
    {
      // Prep the next row of data.
      if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
        SNPRINTF_AND_MOVEPTRS1("\n            ");
      } else {
        Add_InsufficientSpaceMessage(parse_data);
        return;
      }
      columns = 0;
    }

    if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
      SNPRINTF_AND_MOVEPTRS2("%02X ", value);
    }
    else
    {
      Add_InsufficientSpaceMessage(parse_data);
      return;
    }
    columns++;

    // Put an extra space inbetween groups of four bytes
    if ((columns & 0x03) == 0)
    {
      if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
        SNPRINTF_AND_MOVEPTRS1(" ");
      }
      else
      {
        Add_InsufficientSpaceMessage(parse_data);
        return;
      }
    }
  }

  return;
}


/**
 * <!-- ULFE_RealTimeStringParse -->
 *
 * @brief Do the RealTimeString to string parse work.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param mpm : While parsing a MPM this additional data is passed around to
 *              hold MPM data.
 */
static void ULFE_RealTimeStringParse( ULOG_READ_TO_ASCII_DATA* parse_data,
                                      ULOG_READ_TO_ASCII_DATA* mpm )
{
  char value;

  while(UlogReadNextMsgPart((unsigned char*)&value, parse_data, 1))
  {
    if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
      SNPRINTF_AND_MOVEPTRS2("%c", value);
    } else {
      Add_InsufficientSpaceMessage(parse_data);
      return;
    }
  }

  return;
}


/**
 * <!-- ULFE_RealTimeWordArrayParse -->
 *
 * @brief Do the RealTimeWordArray to string parse work.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param msgFormat : ULOG_REALTIME_SUBTYPE_WORDDATA or ULOG_REALTIME_SUBTYPE_CSVDATA
 * @param mpm : While parsing a MPM this additional data is passed around to
 *              hold MPM data.
 */
static void ULFE_RealTimeWordArrayParse( ULOG_READ_TO_ASCII_DATA* parse_data,
                                         uint32 msgFormat,
                                         ULOG_READ_TO_ASCII_DATA* mpm )
{
  uint32 columns = 0;
  int wrapData = 0;
  unsigned int value;

  while(UlogMsgRemainingBytes(parse_data) &&
        (parse_data->outputsz_size > 11))
  {
    if (msgFormat == ULOG_REALTIME_SUBTYPE_WORDDATA)
    {
      // WordArray
      wrapData = 1;
      UlogReadNextMsgPart((unsigned char*)&value, parse_data, 4);
      if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
        SNPRINTF_AND_MOVEPTRS2("0x%08X ", value);
      }else{
        Add_InsufficientSpaceMessage(parse_data);
        return;
      }
    }
    else
    {
      // CSV
      UlogReadNextMsgPart((unsigned char*)&value, parse_data, 4);
      if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
        SNPRINTF_AND_MOVEPTRS2("0x%08X ", value);
      }else{
        Add_InsufficientSpaceMessage(parse_data);
        return;
      }
    }
    columns++;
    if ((columns == 8) && (UlogMsgRemainingBytes(parse_data)) && (wrapData))
    {
      // Prep the next row of data.
      if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
        SNPRINTF_AND_MOVEPTRS1("\n            ");
      }else{
        Add_InsufficientSpaceMessage(parse_data);
        return;
      }
      columns = 0;
    }
  }

  return;
}


/**
 * <!-- isAllowableVectorPreChar -->
 *
 * @brief Used to validate characters in format string for Vector prints
 *
 * Allows for trailing spaces, commas, or other punctuation after the vector
 * format string, such as "%d " or "%08u, "
 *
 * @param fmtChar : Character to check.
 *
 * @return TRUE if the character is allowed to precede a vector format string
 */
static boolean isAllowableVectorPreChar( char fmtChar )
{
  if((fmtChar == '0') || (fmtChar == 'x'))
    return  ( TRUE ) ;
  else
    return ( FALSE );
}

/**
 * <!-- isAllowableVectorPostChar -->
 *
 * @brief Used to validate characters in format string for Vector prints
 *
 * Allows for trailing spaces, commas, or other punctuation after the vector
 * format string, such as "%d " or "%08u, "
 *
 * @param fmtChar : Character to check.
 *
 * @return TRUE if the character is allowed to follow a vector format string
 */
static boolean isAllowableVectorPostChar( char fmtChar )
{
  if((fmtChar == ' ') || (fmtChar == ',') || (fmtChar == ';'))
    return  ( TRUE ) ;
  else
    return ( FALSE );
}


/**
 * <!-- isValidVectorFmtString -->
 *
 * @brief Validate a format string being used to parse a vector message.
 *        Expected format strings contain either %d, %u, %x, or $i.
 *        Minimal extra character are allowed. Some Examples of valid strings are:
 *        "%d "
 *        "0x%08x "
 *        "%u, "
 *
 * @param fmtString : Pointer to the Vector format string to validate
 */

static boolean isValidVectorFmtString(const char* fmtString)
{
  const char *fmtPtr;
/* Vector format strings are expected to be short.
   Will allow for up to 32 characters before failing
 */
#define VECTOR_FMT_MAXSIZE 32

  if(fmtString == NULL)
    return ( FALSE );

  fmtPtr = fmtString;

  /* Allow certain characters before the '%' in vector prints */
  while ( isAllowableVectorPreChar( *fmtPtr ) && (*fmtPtr) && (((uint32)fmtPtr - (uint32)fmtString) < (VECTOR_FMT_MAXSIZE - 1)) )
  {
    fmtPtr++;
  }

  /* Vector fmtString must begin with '%', after allowable pre characters */
  if ( *fmtPtr != '%' )
    return ( FALSE );

  do
  {
    fmtPtr++;
    /* continuing scanning if the last character read is not a conversion char,
     * we haven't hit the end of the string, and we haven't exceeded our
     * MAXSIZE
     */
  }
  while ( isNotConversionChar( *fmtPtr ) && (*fmtPtr) && (((uint32)fmtPtr - (uint32)fmtString) < (VECTOR_FMT_MAXSIZE - 1)) );

  /* Check that we ended on a supported conversion character for vector prints */
  if( !((*fmtPtr=='u') || (*fmtPtr=='x') || (*fmtPtr=='X')
      || (*fmtPtr=='d') || (*fmtPtr=='i')) )
    return ( FALSE );

  fmtPtr++; /* Skip the conversion character */

  /* Check for allowable post characters */
  while ( isAllowableVectorPostChar( *fmtPtr ) && (*fmtPtr) && (((uint32)fmtPtr - (uint32)fmtString) < (VECTOR_FMT_MAXSIZE - 1)) )
  {
    fmtPtr++;
  }

  /* Ensure string is null terminated at this point */
  if (*fmtPtr != NULL)
    return ( FALSE );
  else
    return ( TRUE );
}



/**
 * <!-- ULFE_RealTimeVectorParse -->
 *
 * @brief Do the RealTimeVector to string parse work.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param mpm : While parsing a MPM this additional data is passed around to
 *              hold MPM data.
 */
static void ULFE_RealTimeVectorParse( ULOG_READ_TO_ASCII_DATA* parse_data,
                                      ULOG_READ_TO_ASCII_DATA* mpm )
{
  char  localChar;
  uint16 localShort;
  uint32 localWord;
  const char * fmtString;
  uint16 entryByteCount;
  //uint16 vectorLength;

  if (UlogMsgRemainingBytes(parse_data) > 8)  //must have space for a fmtstring and size info
  {
    UlogReadNextMsgPart((unsigned char*)&localWord, parse_data, 4);
    fmtString = (const char *)localWord;

    UlogReadNextMsgPart((unsigned char*)&localWord, parse_data, 4);
    entryByteCount = localWord >> 16;
    //vectorLength = localWord & 0x0000FFFF;

    /* Check that fmtString is a valid format string for Vectors */
    if(!isValidVectorFmtString( fmtString ))
    {
      Add_BadVectorParseMessage( parse_data );
      return;
    }

    while((UlogMsgRemainingBytes(parse_data)) &&
          (parse_data->outputsz_size))
    {
      switch (entryByteCount)
      {
        case 1:
          UlogReadNextMsgPart((unsigned char*)&localChar, parse_data, entryByteCount);
          localWord = localChar;
          break;
        case 2:
          UlogReadNextMsgPart((unsigned char*)&localShort, parse_data, entryByteCount);
          localWord = localShort;
          break;
        case 4:
          UlogReadNextMsgPart((unsigned char*)&localWord, parse_data, entryByteCount);
          break;

        default:
          // This is an error.  Exit.
          break;
      }
      //ensure there's enough space to write the string the string + NULL + the number
      if (parse_data->outputsz_size > strlen(fmtString)+1+SNPRINTF_MIN_SIZE){
        SNPRINTF_AND_MOVEPTRS2(fmtString, localWord);
      } else {
        Add_InsufficientSpaceMessage(parse_data);
        return;
      }
    }
  }

  return;
}


/**
 * <!-- ULFE_RawParse -->
 *
 * @brief Do the raw message to string parse work.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param mpm : While parsing a MPM this additional data is passed around to
 *              hold MPM data.
 * @param datawidth : 1, 2, or 4 byte sized data.
 */
static void ULFE_RawParse( ULOG_READ_TO_ASCII_DATA* parse_data,
                           ULOG_READ_TO_ASCII_DATA* mpm,
                           unsigned char datawidth )
{
  unsigned char  localChar;
  uint16 localShort;
  uint32 localWord;

  if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
    SNPRINTF_AND_MOVEPTRS1("- ");
  } else {
    Add_InsufficientSpaceMessage(parse_data);
    return;
  }

  while((UlogMsgRemainingBytes(parse_data)) &&
          (parse_data->outputsz_size))
  {
    switch (datawidth)
    {
      case 1:
        UlogReadNextMsgPart((unsigned char*)&localChar, parse_data, datawidth);
        //localWord = localChar;
        if (parse_data->outputsz_size > 5){ //ensure there's enough space to write the string
          SNPRINTF_AND_MOVEPTRS2("%02X, ", localChar);
        } else {
          Add_InsufficientSpaceMessage(parse_data);
          return;
        }
        break;
      case 2:
        UlogReadNextMsgPart((unsigned char*)&localShort, parse_data, datawidth);
        //localWord = localShort;
        if (parse_data->outputsz_size > 7){ //ensure there's enough space to write the string
          SNPRINTF_AND_MOVEPTRS2("%04X, ", localShort);
        } else {
          Add_InsufficientSpaceMessage(parse_data);
          return;
        }
        break;
      case 4:
        UlogReadNextMsgPart((unsigned char*)&localWord, parse_data, datawidth);
        if (parse_data->outputsz_size > 12){ //ensure there's enough space to write the string
          SNPRINTF_AND_MOVEPTRS2("%8X, ", (unsigned int)localWord);
        } else {
          Add_InsufficientSpaceMessage(parse_data);
          return;
        }
        break;

      default:
        // This is an error.  Exit.
        break;
    }
  }
  return;
}
/**
 * <!-- diagbuffer_parse_diag_data_event -->
 *
 * @brief Do the parsing of event type packet in diagbuffer.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param length :  Length of the event data stored in the diagbuffer.
 *
 * @param msgFormat : msgFormat to determine if event is with payloa or without payload.
 */
void diagbuffer_parse_diag_data_event(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length, uint32 msgFormat)
{
  uint8 event_payload_length = 0;
  uint8 length_field = 0;
  uint8 event_field = 0;
  uint16 outgoing_event_length = 0;
  event_id_type diagbuffer_event_id;
  diagbuffer_event_type *diagbuffer_event_ptr = NULL;
  uint8* source_work_ptr;
  uint8* destination_diag_buf_ptr;
  diag_cmd_ext_v1_type ext_hdr;
  uint16 destination_copy_length =0;
  uint32 param;
  uint32 dest_size;
  uint32 copy_size;
  uint8 param_length=0;
  uint8* tstamp_ptr= NULL;

  // Make sure the inputs look ok.
  if((parse_data->outputsz_size==0)||  //there's space to put the output string
    (parse_data->part1_size==0)||     //there's data to read (printfs are only ever in the part1 data
    (parse_data->msgpart1==NULL)||    //the input pointer looks reasonable
    (parse_data->outputsz==NULL))     //the outputsz pointer looks reasonable
  {
    return;
  }

  dest_size = parse_data->outputsz_size;

  source_work_ptr = (uint8 *) parse_data->msgpart1;
  destination_diag_buf_ptr = (uint8 *)parse_data->outputsz;
  tstamp_ptr = (uint8 *) parse_data->msgpart1;

  /*source_work_ptr points to the timestamp, get past it */
  source_work_ptr += ULOG_TIMESTAMP_SIZE;

  /* get pointer to event specific stuffs */
  diagbuffer_event_ptr = (diagbuffer_event_type *)source_work_ptr;

  /* move source_work_ptr to point to PARAM (field is available) or DATA */
  source_work_ptr += (sizeof(diagbuffer_event_type));

  /* if not a version we undersatnd, skip processing it. */
  if ( (diagbuffer_event_ptr->version != DIAGBUF_VERSION_0) &&
       (diagbuffer_event_ptr->version != DIAGBUF_VERSION_1))
  {
     return;
  }

  if ( diagbuffer_event_ptr->version == DIAGBUF_VERSION_1 )
  {
    param = *(uint32 *)source_work_ptr;
    source_work_ptr += (sizeof(uint32)); /* As per version 1 of extnded pkt param is
                                            4 bytes long
                                         */

    param_length = sizeof(uint32); /* length of param field is 4 bytes, will use this
                                      variable to calculate payload length of the event.
                                   */

    /* It's an extended pkt, build and copy ext v1 pkt header */
    ext_hdr.cmd_code = DIAG_CMD_EXT_F;
    ext_hdr.version = diagbuffer_event_ptr->version;
    ext_hdr.proc_id = DIAG_MY_PROC_ID;
    ext_hdr.id = param;
    copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length),
                                                 (dest_size - destination_copy_length),
                                                 &ext_hdr, sizeof(diag_cmd_ext_v1_type));
    destination_copy_length += copy_size;
    parse_data->outOffset += copy_size;
    parse_data->outputsz_size -= copy_size;

    /* after this we need to put regular diag packet */
  }

  /* Building to regular diag event pkt */
  /* Build Event Report Header ->
   | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
   ---------------------
   Command Code - 1 byte long
  */
  *(destination_diag_buf_ptr + destination_copy_length) = DIAG_EVENT_REPORT_F; /* Copy cmd code DIAG_EVENT_REPORT_F */
  destination_copy_length++; /* Command code is 1 byte long */
  parse_data->outOffset++;
  parse_data->outputsz_size--;

  /* Length of the event */
  /* Build Event Report Header ->
   | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                       ----------
   Length - length of the whole event - Length field is 2 bytes long
  */
  /*Will calculate length and store in destination*/
  length_field = destination_copy_length;
  destination_copy_length += (sizeof(uint16)); /* Event length is 2 bytes long */
  parse_data->outOffset += (sizeof(uint16));
  parse_data->outputsz_size -= (sizeof(uint16));
  /* Event ID */
  /* Build Event ->
   | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                ------------
   EVENT ID - 2 bytes long.
  */
  /*Will copy event id at the end as by then we would have calculated payload length and its needs to
    payload length info is needed to store in bit 14 and bit 15 of the event id */
  event_field = destination_copy_length;
  destination_copy_length += (sizeof(uint16));
  parse_data->outOffset += (sizeof(uint16));
  parse_data->outputsz_size -= (sizeof(uint16));

  /* Convert timetick to network time */
  /* Build Event ->
   | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                           ---------
   TIME - 8 bytes long. Since we are sending one event per report.
  */
      /* Store timestamp */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), dest_size - destination_copy_length,
                                                     (void *)tstamp_ptr, ULOG_TIMESTAMP_SIZE);
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  /*
     Check if event is event with payload type, if is it, store length of payload in destination buffer and
     copy event payload.
  */
  if ( msgFormat == DIAGBUFFER_SUBTYPE_DIAG_EVENT_PAYLOAD )
  {
    /* Calculate event payload length*/
    event_payload_length = length - sizeof(diagbuffer_diag_pkts_hdr_type)
                           - sizeof(diagbuffer_event_type)
                           - param_length;

    /* Store Payload length */
    /* Build Event ->
     | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                                    ---------------
     PAYLOAD_LEN - 1 byte long.
    */
    *(destination_diag_buf_ptr + destination_copy_length) = event_payload_length;
    destination_copy_length += (sizeof(uint8));
    parse_data->outOffset += (sizeof(uint8));
    parse_data->outputsz_size -= (sizeof(uint8));

    /* Store payload data */
    /* Build Event ->
     | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                                                   ---------------
     PAYLOAD_DATA - N (event_payload_length) byets long.
    */
    copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length),
                                                 (dest_size - destination_copy_length),
                                                 source_work_ptr,
                                                 event_payload_length);

    destination_copy_length += copy_size;
    parse_data->outOffset += copy_size;
    parse_data->outputsz_size -= copy_size;
  } /* end of if (diagbuffer_pkt_hdr_ptr->format == ULOG_SUBTYPE_DIAG_EVENT_PAYLOAD) */

  /* Since now we know payload length, we can build event id, and copy it to destination */
  diagbuffer_event_id.id = diagbuffer_event_ptr->event_id;

  if ( event_payload_length <= EVENT_PAY_TWO_BYTE )
  {
    diagbuffer_event_id.payload_len = event_payload_length;
  }
  else
  {
    diagbuffer_event_id.payload_len = EVENT_PAY_LENGTH;
    /*
       add one (for 1 byte) to payload length to commodate 1 byte payload_length field in event (only for event_with_payload)
       we are doing this here to avoid taking one extra variable to track that and use in outgoing_event_length calculation
       below
    */
    event_payload_length += sizeof(uint8);
  }
  diagbuffer_event_id.reserved = 0; /* Reserved bit */
  diagbuffer_event_id.time_trunc_flag = DIAGBUFFER_FULL_TIME_STAMP; /* Since we are sending one event per report
                                                                                   time stamp will always be FULL for each
                                                                                   event.
                                                                                */

  /* Earlier we left hole in the destination buffer to fill EVENT ID now. */
  /* Build Event ->
   | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                                ------------
    Store EVENT_ID
  */
  copy_size = memscpy((destination_diag_buf_ptr + event_field),
                                               sizeof(event_id_type),
                                               &diagbuffer_event_id,
                                               sizeof(event_id_type));

  /* now update length for the event packet */
  /* length = payload length + 2 bytes of event (event id 12 bits, 1 bit reserved, 2 bits payload length flag, 1 bit time length) +
                               8 bytes of timestamp, since there is one event per report we will always have fill time stamp */
  outgoing_event_length = event_payload_length + sizeof(uint16) + sizeof(uint64);

  /*
     We calculated length_field above to copy event length now. We do not need to move destination now (as we have already done it above when calculated
     length_field
  */
  /* Build Event ->
   | Command Code 0x60 | LENGTH | EVENT ID | TIME | PAYLOAD_LEN | PAYLOAD_DATA |
                        ---------
    Storing LENGTH
  */
  copy_size = memscpy((destination_diag_buf_ptr + length_field), sizeof(uint16),
                                               &outgoing_event_length, sizeof(uint16));
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

}

/**
 * <!-- diagbuffer_parse_diag_data_log -->
 *
 * @brief Do the parsing of log type packet in diagbuffer.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param length :  Length of the log data stored in the diagbuffer.
 *
 */
void diagbuffer_parse_diag_data_log(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length)
{
  uint16 log_data_length = 0;
  diagbuffer_log_type *diagbuffer_log_ptr = NULL;
  uint8* source_work_ptr;
  uint8* destination_diag_buf_ptr;
  diag_log_rsp_type* log_rsp_ptr=NULL;
  log_hdr_type* log_hdr= NULL;
  uint8 param_length=0;
  uint32 dest_size;
  uint32 copy_size;
  uint16 destination_copy_length =0;
  diag_cmd_ext_v1_type ext_hdr;
  uint32 param;
  uint8* tstamp_ptr;

  // Make sure the inputs look ok.
  if((parse_data->outputsz_size==0)||  //there's space to put the output string
    (parse_data->part1_size==0)||     //there's data to read (printfs are only ever in the part1 data
    (parse_data->msgpart1==NULL)||    //the input pointer looks reasonable
    (parse_data->outputsz==NULL))     //the outputsz pointer looks reasonable
  {
    return;
  }

  dest_size = parse_data->outputsz_size;

  source_work_ptr = (uint8 *) parse_data->msgpart1;
  destination_diag_buf_ptr = (uint8 *)parse_data->outputsz;
  tstamp_ptr = (uint8 *) parse_data->msgpart1;

  /*source_work_ptr points to the timestamp, get past it */
  source_work_ptr += ULOG_TIMESTAMP_SIZE;

  /* get pointer to log specific stuffs */
  diagbuffer_log_ptr = (diagbuffer_log_type *)source_work_ptr;

  /* move source_work_ptr to point to PARAM (field is available) or DATA */
  source_work_ptr += (sizeof(diagbuffer_log_type));

/* if not a version we understand, skip processing it. */
  if ( (diagbuffer_log_ptr->version != DIAGBUF_VERSION_0) &&
       (diagbuffer_log_ptr->version != DIAGBUF_VERSION_1))
  {
     return;
  }

  if ( diagbuffer_log_ptr->version == DIAGBUF_VERSION_1 )
  {
    param = *(uint32 *)source_work_ptr;
    source_work_ptr += (sizeof(uint32)); /* As per version 1 of extnded pkt param is
                                            4 bytes long */

    param_length = sizeof(uint32); /* length of param field is 4 bytes, will use this
                                      variable to calculate data length in this log pkt
                                   */
    /* It's an extended pkt, build and copy ext v1 pkt header */
    ext_hdr.cmd_code = DIAG_CMD_EXT_F;
    ext_hdr.version = diagbuffer_log_ptr->version;
    ext_hdr.proc_id = DIAG_MY_PROC_ID;
    ext_hdr.id = param;
    copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length),
                                                 (dest_size - destination_copy_length),
                                                 &ext_hdr, sizeof(diag_cmd_ext_v1_type));
    destination_copy_length += copy_size;
    parse_data->outOffset += copy_size;
    parse_data->outputsz_size -= copy_size;

    /* after this we need to put regular diag packet */
  }

/* Building to regular diag log pkt */
  /* Build Log Header ->
   | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
   ---------------------
   Command Code - 1 byte long
  */
  log_rsp_ptr = (diag_log_rsp_type *)(destination_diag_buf_ptr + destination_copy_length);
  log_rsp_ptr->cmd_code = DIAG_LOG_F;
  log_rsp_ptr->more = 0;


  /* Calculate length of the data in the diagbuffer type log */
  log_data_length =   length
                    - sizeof(diagbuffer_diag_pkts_hdr_type)
                    - sizeof(diagbuffer_log_type)
                    - param_length;

  log_rsp_ptr->len = (sizeof(log_header_type) + log_data_length);
  destination_copy_length += LOG_DIAGPKT_OFFSET;
  parse_data->outOffset += LOG_DIAGPKT_OFFSET;
  parse_data->outputsz_size-= LOG_DIAGPKT_OFFSET;

  /* Store length which is part of LOG_ITEM */
  /* Build Log ->
   | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                        ---------
   Length - 2 byte long, its length of the rest of the packet i.e. length (next field) + Log Code + TS + Data
            or in other words sizeof(log_header_type) + DATA size.
  */
  log_hdr = (log_hdr_type *)(destination_diag_buf_ptr + destination_copy_length);
  log_hdr->len = (uint16) (sizeof(log_header_type) + log_data_length);
  destination_copy_length += (sizeof(uint16));
  parse_data->outOffset += (sizeof(uint16));
  parse_data->outputsz_size-= (sizeof(uint16));

  /* Store Log code field */
  /* Build Log ->
   | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                                ------------
   log code - 2 byte long.
  */
  log_hdr->code = (uint16)diagbuffer_log_ptr->log_code;
  destination_copy_length += (sizeof(uint16));
  parse_data->outOffset += (sizeof(uint16));
  parse_data->outputsz_size-= (sizeof(uint16));

    /* Store timestamp */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), dest_size - destination_copy_length,
                                                     (void *) tstamp_ptr, ULOG_TIMESTAMP_SIZE );
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  /* Store rest of the DATA */
  /* Build Log ->
   | Command Code 0x10 | MORE | LENGTH | LENGTH | LOG_CODE | TIMESTAMP | DATA |
                                                                       --------
   Data - N bytes long. length of data is in log_data_length
  */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length),
                                               (dest_size - destination_copy_length),
                                               source_work_ptr,
                                               log_data_length);

  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

}

/**
 * <!-- diagbuffer_parse_diag_data_msg_sprintf -->
 *
 * @brief Do the parsing of message sprintf type packet in diagbuffer.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param length :  Length of the message stored in the diagbuffer.
 *
 */
void diagbuffer_parse_diag_data_msg_sprintf(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length)
{
  uint16 destination_copy_length =0;
  uint32 dest_size =0;
  uint32 copy_size = 0;
  uint8* tstamp_ptr = NULL;
  uint8* source_work_ptr = NULL;
  uint8* destination_diag_buf_ptr =NULL;
  diagbuffer_msg_sprintf_type *diagbuffer_msg_ptr =NULL;
  uint32 param;
  char *fmt_str_ptr = NULL;
  char *fname_ptr = NULL;
  uint16 fmt_str_len = 0;
  uint16 fmt_file_len = 0;
  msg_desc_type msg_desc;
  msg_desc_type *const_access_ptr = NULL;
  diag_cmd_ext_v1_type ext_hdr;

  if((parse_data->outputsz_size==0)||  //there's space to put the output string
    (parse_data->part1_size==0)||     //there's data to read (printfs are only ever in the part1 data
    (parse_data->msgpart1==NULL)||    //the input pointer looks reasonable
    (parse_data->outputsz==NULL))     //the outputsz pointer looks reasonable
  {
    return;
  }

  dest_size = parse_data->outputsz_size;

  source_work_ptr = (uint8 *) parse_data->msgpart1;
  destination_diag_buf_ptr = (uint8 *)parse_data->outputsz;
  tstamp_ptr = (uint8 *) parse_data->msgpart1;

  /*source_work_ptr points to the timestamp, get past it */
  source_work_ptr += ULOG_TIMESTAMP_SIZE;

   /* get the 32 bits address at source_work_ptr */
  const_access_ptr = (msg_desc_type *)((void *)(*(uint32 *)(source_work_ptr)));

  /* Extract line number */
  msg_desc.line = const_access_ptr->line;

  /* Extract SSID */
  msg_desc.ss_id = const_access_ptr->ss_id;

  /* Extract SS_MASK */
  msg_desc.ss_mask = const_access_ptr->ss_mask;

  /* have processed const type/const hash - 4 bytes, move source_work_ptr by the same amount */
  source_work_ptr+= sizeof(uint32);

  /* get pointer to msg specific stuffs */
  diagbuffer_msg_ptr = (diagbuffer_msg_sprintf_type *)source_work_ptr;

  /* move source_work_ptr to point to PARAM (field is available) or DATA */
  source_work_ptr += sizeof(diagbuffer_msg_sprintf_type);

  /* if not a version we undersatnd, skip processing it. */
  if ( (diagbuffer_msg_ptr->version != DIAGBUF_VERSION_0) &&
             (diagbuffer_msg_ptr->version != DIAGBUF_VERSION_1))
  {
    return;
  }

  if ( diagbuffer_msg_ptr->version == DIAGBUF_VERSION_1 )
  {
    param = *(uint32 *)source_work_ptr;
    source_work_ptr += (sizeof(uint32)); /* As per version 1 of extnded pkt param is
                                                  4 bytes long */

    /* It's an extended pkt, add ext v1 pkt header */
    ext_hdr.cmd_code = DIAG_CMD_EXT_F;
    ext_hdr.version = diagbuffer_msg_ptr->version;
    ext_hdr.proc_id = DIAG_MY_PROC_ID;
    ext_hdr.id = param;
    copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length),
                                                 (dest_size - destination_copy_length),
                                                 &ext_hdr,
                                                 sizeof(diag_cmd_ext_v1_type));
    destination_copy_length += copy_size;
    parse_data->outOffset += copy_size;
    parse_data->outputsz_size -= copy_size;
    /* after this we need to put regular diag packet */
  }

  *(destination_diag_buf_ptr + destination_copy_length) = DIAG_EXT_MSG_F;
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

#ifdef DIAG_UIMAGE_MODE
  /* store TS type */
  if ( diag_wcdma_wl1_frame_cb != NULL )
  {
    *(destination_diag_buf_ptr + destination_copy_length) = MSG_TS_TYPE_GW;
  }
  else
#endif
  {
    *(destination_diag_buf_ptr + destination_copy_length) = MSG_TS_TYPE_CDMA_FULL;
  }
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

  /* Store number of args */
  *(destination_diag_buf_ptr + destination_copy_length) = 0;
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

  /* Store drop cnt, since reading from circular buffer drop_cnt will be 0 */
  *(destination_diag_buf_ptr + destination_copy_length) = 0;
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

  /* Store timestamp */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), dest_size - destination_copy_length,
                                                     (void *)tstamp_ptr, ULOG_TIMESTAMP_SIZE);
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  /* Store msg_desc i.e. line number, ss_id, ss_mask */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length),
                                               (dest_size - destination_copy_length),
                                               &msg_desc, sizeof(msg_desc_type));
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  /* Extract the format string */
  fmt_str_ptr = (char *)source_work_ptr;
  fmt_str_len = (int)strlen((char *)(fmt_str_ptr));
  source_work_ptr+= fmt_str_len +1;

  /* Store format string */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), (dest_size - destination_copy_length),
                                               fmt_str_ptr, fmt_str_len);
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  *(char *)(destination_diag_buf_ptr + destination_copy_length) = '\0';
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size--;

  /*source_work_ptr now points to file name */
  fname_ptr = (char *)source_work_ptr;
  fmt_file_len = (int)strlen((char *)(fname_ptr));
  source_work_ptr+= fmt_file_len +1;

  /* Store file name */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), (dest_size - destination_copy_length),
                                  fname_ptr, fmt_file_len);
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  *(char *)(destination_diag_buf_ptr + destination_copy_length) = '\0';
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size--;
}

/**
 * <!-- diagbuffer_parse_diag_data_msg_qsr4 -->
 *
 * @brief Do the parsing of message (QSHRINK4) type packet in diagbuffer.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param length :  Length of the message stored in the diagbuffer.
 *
 */
void diagbuffer_parse_diag_data_msg_qsr4(ULOG_READ_TO_ASCII_DATA* parse_data, uint32 length)
{
  diag_cmd_ext_v1_type ext_hdr;
  uint8* source_work_ptr;
  uint8* destination_diag_buf_ptr;
  uint32 param;
  diagbuffer_msg_type *diagbuffer_msg_ptr = NULL;
  uint32 const_hash_v4_ptr;
  uint32 const_hash_v4;
  uint16 destination_copy_length =0;
  uint32 dest_size;
  uint32 copy_size;
  uint8* tstamp_ptr;

   // Make sure the inputs look ok.
  if((parse_data->outputsz_size==0)||  //there's space to put the output string
    (parse_data->part1_size==0)||     //there's data to read (printfs are only ever in the part1 data
    (parse_data->msgpart1==NULL)||    //the input pointer looks reasonable
    (parse_data->outputsz==NULL))     //the outputsz pointer looks reasonable
  {
    return;
  }

  dest_size = parse_data->outputsz_size;

  source_work_ptr = (uint8 *) parse_data->msgpart1;
  destination_diag_buf_ptr = (uint8 *)parse_data->outputsz;
  tstamp_ptr = (uint8 *) parse_data->msgpart1;

  /*source_work_ptr points to the timestamp, get past it */
  source_work_ptr += ULOG_TIMESTAMP_SIZE;

  /* get the 32 bits address at source_work_ptr */
  const_hash_v4_ptr = (uint32)(*(uint32 *)(source_work_ptr));
  const_hash_v4 = msg_get_qsr4_hash(const_hash_v4_ptr);

  source_work_ptr += sizeof(uint32);

  /* get pointer to msg specific stuffs */
  diagbuffer_msg_ptr = (diagbuffer_msg_type *)source_work_ptr;

  /* move source_work_ptr to point to PARAM (field is available) or DATA */
  source_work_ptr += (sizeof(diagbuffer_msg_type));

  if ( (diagbuffer_msg_ptr->version != DIAGBUF_VERSION_0) &&
             (diagbuffer_msg_ptr->version != DIAGBUF_VERSION_1))
  {
    return;
  }

  if ( diagbuffer_msg_ptr->version == DIAGBUF_VERSION_1 )
  {
    param = *(uint32 *)source_work_ptr;
    source_work_ptr += (sizeof(uint32)); /* As per version 1 of extnded pkt param is
                                                  4 bytes long */

    /* It's an extended pkt, add ext v1 pkt header */
    ext_hdr.cmd_code = DIAG_CMD_EXT_F;
    ext_hdr.version = diagbuffer_msg_ptr->version;
    ext_hdr.proc_id = DIAG_MY_PROC_ID;
    ext_hdr.id = param;
    copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length),
                                                 (dest_size - destination_copy_length),
                                                 &ext_hdr,
                                                 sizeof(diag_cmd_ext_v1_type));
    destination_copy_length += copy_size;
    parse_data->outOffset += copy_size;
    parse_data->outputsz_size -= copy_size;
    /* after this we need to put regular diag packet */
  }

  *(destination_diag_buf_ptr + destination_copy_length) = DIAG_QSR4_EXT_MSG_TERSE_F;
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

 #ifdef DIAG_UIMAGE_MODE
  /* store TS type */
  if ( diag_wcdma_wl1_frame_cb != NULL )
  {
    *(destination_diag_buf_ptr + destination_copy_length) = MSG_TS_TYPE_GW;
  }
  else
 #endif
  {
    *(destination_diag_buf_ptr + destination_copy_length) = MSG_TS_TYPE_CDMA_FULL;
  }
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

  /* Store number of args */
  *(destination_diag_buf_ptr + destination_copy_length) = diagbuffer_msg_ptr->num_args;
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

  /* Store drop cnt, since reading from circular buffer drop_cnt will be 0 */
  *(destination_diag_buf_ptr + destination_copy_length) = 0; /* Drop count is 0 for diagbuffer */
  destination_copy_length++;
  parse_data->outOffset ++;
  parse_data->outputsz_size --;

  /* Store timestamp */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), dest_size - destination_copy_length,
                                                     (void *)tstamp_ptr, ULOG_TIMESTAMP_SIZE);
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  /*Store the message hash */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), dest_size - destination_copy_length,
                                                    &const_hash_v4, sizeof(uint32));
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  /*Store last 2 bytes of GUID */
  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), dest_size - destination_copy_length,
                                                    &msg_v4_dbase_guid[15], sizeof(uint8));
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), dest_size - destination_copy_length,
                                                    &msg_v4_dbase_guid[14], sizeof(uint8));
  destination_copy_length += copy_size;
  parse_data->outOffset += copy_size;
  parse_data->outputsz_size -= copy_size;

  /* Store ARGS */
  if ( diagbuffer_msg_ptr->num_args )
  {
    /* source_work_ptr is pointing at ARGS, ARGS are 4*NUM_ARGS long */
    copy_size = memscpy((destination_diag_buf_ptr + destination_copy_length), (dest_size - destination_copy_length),
                                                 source_work_ptr, (sizeof(uint32) * diagbuffer_msg_ptr->num_args));
    destination_copy_length += copy_size;
    parse_data->outOffset += copy_size;
    parse_data->outputsz_size -= copy_size;
  }
}


/**
 * <!-- ULogParsers_FormatParseData -->
 *
 * @brief Add the timestamp to the string and then pass the data to specific parsers.
 *
 * If mpm is not NULL it means we're in a MPM and the timestamp should not be added.
 *
 * @param parse_data : Pointer to a ULOG_READ_TO_ASCII_DATA struct of the msg data
 *                     and character string being working on.
 * @param mpm : While parsing a MPM this additional data is passed around to
 *              hold MPM data.
 */
ULogResult ULogParsers_FormatParseData( ULOG_READ_TO_ASCII_DATA* parse_data,
                                        ULOG_READ_TO_ASCII_DATA* mpm )
{
  // Parse the real-time log and print the results to the output buffer.
  // Input message fomat to this function:
  // <Timestamp> <Format> <Data...>
  uint32 msgFormat;
  uint32 msgData;
  uint32 msgLength;
#ifdef ULOG_64BIT_TIME
  uint64 timestamp;
#else
  uint32 timestamp;
#endif

  if(parse_data == NULL)
    return ( DAL_ERROR );

  // Check the size looks reasonable and read the format
  if (UlogMsgRemainingBytes(parse_data) < MIN_RAW_MSG_LEN){ //ensure that the message is at least big enough for a FORMAT field
    return(DAL_ERROR);
  }

  // Read the format of this message
  UlogReadNextMsgPart((unsigned char*)&msgData, parse_data, 4);

  //clear off the length field.
  msgFormat = msgData & ~ULOG_LENGTH_MASK;
  msgLength = (msgData & ULOG_LENGTH_MASK) >> ULOG_LENGTH_SHIFT;;


  // If it's a realtime message, read the timestamp.
  if ((msgFormat>0) && (msgFormat<ULOG_SUBTYPE_RESERVED1)){
    // The timestamp should be the next word in the parse_data
    UlogReadNextMsgPart((unsigned char*)&timestamp, parse_data, ULOG_TIMESTAMP_SIZE);

    // Add the timestamp to the output string if we're not in a MPM
    if (mpm == NULL)
    {
      if (msgFormat == ULOG_REALTIME_SUBTYPE_CSVDATA)
      {
#ifdef ULOG_64BIT_TIME
        if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE*2){ //ensure there's enough space to write the string
          SNPRINTF_AND_MOVEPTRS2("%llu: ", (uint64)timestamp);
        }
#else
        if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
          SNPRINTF_AND_MOVEPTRS2("%u: ", (unsigned int)timestamp);
        }
#endif
        else
        {
          Add_InsufficientSpaceMessage(parse_data);
          return(DAL_ERROR);
        }
      }
      else
      {
#ifdef ULOG_64BIT_TIME
        if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE*2){ //ensure there's enough space to write the string
          SNPRINTF_AND_MOVEPTRS2("0x%016llX: ", (uint64)timestamp);
        }
#else
        if (parse_data->outputsz_size > SNPRINTF_MIN_SIZE){ //ensure there's enough space to write the string
          SNPRINTF_AND_MOVEPTRS2("0x%08X: ", (unsigned int)timestamp);
        }
#endif
        else {
          Add_InsufficientSpaceMessage(parse_data);
          return(DAL_ERROR);
        }
      }
    }
  } //end of code to add TS


  switch (msgFormat)
  {
    case ULOG_REALTIME_SUBTYPE_WORDDATA:    // An array of uint32 data
    case ULOG_REALTIME_SUBTYPE_CSVDATA:     // An array of word data output as CSV
      ULFE_RealTimeWordArrayParse(parse_data, msgFormat, mpm);
      break;

    case ULOG_REALTIME_SUBTYPE_PRINTF:      // A printf style message
      ULFE_RealTimePrintfParse(parse_data, mpm);
      break;

    case ULOG_REALTIME_SUBTYPE_BYTEDATA:    // An array of byte data
      ULFE_RealTimeCharArrayParse(parse_data, mpm);
      break;

    case ULOG_REALTIME_SUBTYPE_STRINGDATA:  // An array of char data
      ULFE_RealTimeStringParse(parse_data, mpm);
      break;

    case ULOG_REALTIME_SUBTYPE_VECTOR:      // An vector of data
      ULFE_RealTimeVectorParse(parse_data, mpm);
      break;

    case ULOG_REALTIME_SUBTYPE_MULTIPART:   // A multipart message
      HandleNextMultipartMsg(parse_data, NULL);
      break;

    case ULOG_SUBTYPE_RAW8:
    case ULOG_REALTIME_SUBTYPE_RESERVED_FOR_RAW:   // An Raw 1 byte format message
      ULFE_RawParse(parse_data, mpm, 1);
      break;

    case ULOG_SUBTYPE_RAW16:
      ULFE_RawParse(parse_data, mpm, 2); // An Raw 2 byte format message
      break;

    case ULOG_SUBTYPE_RAW32:
      ULFE_RawParse(parse_data, mpm, 4); // An Raw 4 byte format message
      break;
    case DIAGBUFFER_SUBTYPE_DIAG_MSG_SPRINTF:
      diagbuffer_parse_diag_data_msg_sprintf(parse_data, msgLength);
      break;
    case DIAGBUFFER_SUBTYPE_DIAG_MSG_QSR4:
      diagbuffer_parse_diag_data_msg_qsr4(parse_data, msgLength);
      break;
    case DIAGBUFFER_SUBTYPE_DIAG_LOG:
      diagbuffer_parse_diag_data_log(parse_data, msgLength);
      break;
    case DIAGBUFFER_SUBTYPE_DIAG_EVENT:
    case DIAGBUFFER_SUBTYPE_DIAG_EVENT_PAYLOAD:
      diagbuffer_parse_diag_data_event(parse_data, msgLength, msgFormat );
      break;

    default:
      break;
  }

  if ((msgFormat>0) && (msgFormat<ULOG_SUBTYPE_RESERVED1))
  {
    if (mpm == NULL)
    {
      // Add the linefeed to the end of the message if there's space
      if (parse_data->outputsz_size > strlen("\n")){ //ensure there's enough space to write the string
        ADDCHAR_AND_MOVEPTRS('\n')
      }
    }
  }

  return(DAL_SUCCESS);
}


/**
 * <!-- ULogParsers_FormatMsg -->
 *
 * @brief Prepare the message into our convenient parse_data structure and call ULogParsers_FormatParseData.
 *
 * @param log : Pointer to the log the data came from
 * @param message : The binary log data to parse/convert to ascii.
 * @param outputsz : Pointer to memory to place the ascii decoded log.
 * @param outputSizeBytes : How much memory is available at outputsz.
 * @param wordsConsumed : How many bytes of the log message were decoded.
 */
ULogResult ULogParsers_FormatMsg( ULOG_TYPE * log,
                                  char *  message,
                                  char *  outputsz,
                                  uint32 outputSizeBytes,
                                  uint32 * wordsConsumed)
{
  uint32 msgLength;
  ULOG_READ_TO_ASCII_DATA parse_data;

  // Format the message as a string, and put it in the outputsz buffer.
  // Return the number of words used.
  if ((outputSizeBytes == 0) ||
      (outputsz == NULL) ||
      (message == NULL))
  {
    return(DAL_ERROR);
  }

  //NULL terminate the string
  outputsz[0] = 0;

  // Read the length of this message
  core_memcpy(&msgLength, 4, &message[0], 4);
  msgLength = (msgLength & ULOG_LENGTH_MASK) >> ULOG_LENGTH_SHIFT;

  //set the amount of message words we'll be handling
  *wordsConsumed = ULOGINTERNAL_PADCOUNT(msgLength);

  if ((msgLength < MIN_RAW_MSG_LEN) || (msgLength > DIAGBUFFER_MAX_MSG_SIZE))
  {
    // If it's too small or too large, we have an invalid formatted message.
    // Throw it away.
    *outputsz = 0;
    return(DAL_ERROR);
  }

  parse_data.msgpart1 = message;
  parse_data.part1_size = msgLength;
  parse_data.msgpart2=NULL;
  parse_data.part2_size = 0;
  parse_data.outputsz = outputsz;
  parse_data.outputsz_size = outputSizeBytes;
  parse_data.outOffset = 0;
  parse_data.environment = ULOG_PARSE_READ_DATA;  //decoding a stored log, not a data stream.
  ULogParsers_FormatParseData(&parse_data, NULL);

  *wordsConsumed = parse_data.outOffset;

  return(DAL_SUCCESS);
}

#endif // DEFEATURE_ULOG_MSG_FORMAT

#ifdef __cplusplus
}
#endif
