#ifdef __cplusplus
extern "C" {
#endif

#include <qtrace.h>

#ifdef __cplusplus
}
#endif
#include "Logger_types_autogen.h"

#ifndef _LOGGER_INTERNAL_H
#define _LOGGER_INTERNAL_H

//#define TASK_INFO_CALLBACKS 10

typedef struct _EventParam{
	int param1;
	int param2;
	int param3;
}EventParam;

typedef struct _TaskOpaquePayload
{
LogEvents             eEventId;
EventParam opt_evt_param;
LogTasks    eTaskName;
uint8 analysis_required;
uint8 inst_id;
}TaskOpaquePayload;


typedef void (*log_event_callback)(LogEvents e, EventParam* opt_evt_param, uint8 analysis_required, uint8 inst_id);

typedef void (*REG_TASK_CB)(TaskOpaquePayload* task_opaque_payload, void* pUserData);

// Application will pass in this Struct LoggerRegTaskCallback
struct LoggerGenRegTaskInfo
{
  /** Each Task will pass its own TaskName and register once at bootup **/
 // LogTasks    eTaskName;
  /**Application callback function pointer*/
  REG_TASK_CB      pCallBackFunc;
  /**Application userdata. Will be returned in cb*/
  void*                 pUserData;
};
typedef struct LoggerGenRegTaskInfo LOGGER_GEN_REG_TASK_INFO;

// Store Task CB here
struct LoggerGenRegTaskInfoDB
{
  /** Each Task will pass its own TaskName and register once at bootup **/
  LogTasks    eTaskName;
  /**Application callback function pointer*/
  REG_TASK_CB      pCallBackFunc;
  /**Application userdata. Will be returned in cb*/
  void*                 pUserData;
};

/**
 * \var typedef FwGenRegTaskProfile FW_GENREG_TASK_PROFILE
 * Creates a type name for FwGenRegTaskProfile
 */

typedef struct _qtraceLogBuff{
	qtrace_buffer_cfg_s verbose_buff;
	qtrace_buffer_cfg_s non_verbose_buff;
}QTraceLogBuff;

#ifdef __cplusplus
extern "C" {
#endif

void MockLogger(int verbose, qtrace_level_e level, char* tags, int subid, char* filename, int lineno, char* fmtstring, ...);

#ifdef __cplusplus
}
#endif

#endif