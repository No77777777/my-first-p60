

#ifndef _LOGGER_H
#define _LOGGER_H

#ifdef __cplusplus
extern "C" {
#endif

#include <qtrace.h>

#ifdef __cplusplus
}
#endif

#include "Logger_types_autogen.h"
#include "Logger_internal.h"

extern Loghandle log_grp_qtrace_hdl_id_tbl[LOG_GRP_MAX];
extern LogGroups log_mod_to_grp_tbl[LOG_MOD_MAX];
extern qtrace_context_id_t log_grp_qtrace_verbose_hdl_id_tbl[LOG_GRP_MAX];
extern qtrace_context_id_t log_grp_qtrace_nonverbose_hdl_id_tbl[LOG_GRP_MAX];

//ASSOC TAG MACRO
/*
#define ASSOC_TAG_1(A) #A
#define ASSOC_TAG_2(A,B) #A B##_GEN
#define ASSOC_TAG_3(A,B,C) #A B##_GEN C##_GEN
#define ASSOC_TAG_4(A,B,C,D) #A B##_GEN C##_GEN D##_GEN
#define ASSOC_TAG_5(A,B,C,D,E) #A B##_GEN C##_GEN D##_GEN E##_GEN
#define ASSOC_TAG_6(A,B,C,D,E,F) #A B##_GEN C##_GEN D##_GEN E##_GEN F##_GEN
*/

#define ASSOC_TAG_1(A) #A
#define ASSOC_TAG_2(A,B) #A "|" #B
#define ASSOC_TAG_3(A,B,C) #A "|" #B "|" #C
#define ASSOC_TAG_4(A,B,C,D) #A "|" #B "|" #C "|" #D
#define ASSOC_TAG_5(A,B,C,D,E) #A "|" #B "|" #C "|" #D "|" #E
#define ASSOC_TAG_6(A,B,C,D,E,F) #A "|" #B "|" #C "|" #D "|" #E "|" #F


#ifdef __cplusplus
extern "C" {
#endif

/**
Function : each task will call this. logger fwk will send CB to this func
		   if event is incoming from another task
**/
void LoggerRegTaskCallback(LogTasks tskid, LOGGER_GEN_REG_TASK_INFO* pGenRegInfo);

/**
Function : each module can call this API, but only the first module calling this API within a group will create
		   a handle for that group. All subsequent invocations in the group including the first one will have the group handle returned
**/
Loghandle LoggerRegModuleCallback(LogModules mod, log_event_callback LogEvtCbFunc); 

/**
Function : task will call this in its context so logger can process event received from other task. 
**/
void LoggerProcessExternalEvent(TaskOpaquePayload* task_opaque_payload);

void LoggerRaiseEvent(LogEvents evt, EventParam* opt_ev_param, uint8 analysis_required, uint8 inst_id);

char const* LoggerEvtToStr(LogEvents evt);

LogModules LoggerEvtToMod(LogEvents evt);

/**
Function : Get Verbose Handle from Module id. 
**/
qtrace_context_id_t getVerboseHdlFromMod(LogModules mod);

/**
Function : Get Non-Verbose Handle from Module id. 
**/
qtrace_context_id_t getNonVerboseHdlFromMod(LogModules mod);

#ifdef __cplusplus
}
#endif




#ifndef FEATURE_IMS_PC_TESTBENCH
#define LOG_VERBOSE( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_verbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	(_inst_id - 1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_nonverbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	(_inst_id-1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_VERBOSE_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#else
#define LOG_VERBOSE( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_MED, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_VERBOSE_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_MED, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_NON_VERBOSE( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_MED, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_NON_VERBOSE_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_MED, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#endif



//LEVEL MACROS

#ifndef FEATURE_IMS_PC_TESTBENCH
#define LOG_VERBOSE_DBG( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_verbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_DEBUG, \
	(_inst_id - 1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_DBG( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_nonverbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_DEBUG, \
	(_inst_id-1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_VERBOSE_DBG_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_DEBUG, \
	_inst_id , \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_DBG_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_DEBUG, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)
#else
#define LOG_VERBOSE_DBG( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_DEBUG, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_DBG( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_DEBUG,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_VERBOSE_DBG_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_DEBUG, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_DBG_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_DEBUG,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#endif



#ifndef FEATURE_IMS_PC_TESTBENCH
#define LOG_VERBOSE_MED( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_verbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	(_inst_id - 1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_MED( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_nonverbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	(_inst_id-1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_VERBOSE_MED_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id , \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_MED_WITH_HDL( \
	_hdl, \
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id , \
	_tag, \
	_summary, \
##__VA_ARGS__)
#else
#define LOG_VERBOSE_MED( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_MED, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_MED( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_MED,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_VERBOSE_MED_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_MED, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_NON_VERBOSE_MED_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_MED,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#endif


#ifndef FEATURE_IMS_PC_TESTBENCH
#define LOG_VERBOSE_HIGH( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_verbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_HIGH, \
	(_inst_id - 1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_HIGH( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_nonverbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_HIGH, \
	(_inst_id - 1), \
	_tag, \
	_summary, \
##__VA_ARGS__)
#define LOG_VERBOSE_HIGH_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_HIGH, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_HIGH_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_HIGH, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)
#else
#define LOG_VERBOSE_HIGH( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_HIGH, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_HIGH( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_HIGH,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_VERBOSE_HIGH_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_HIGH, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_HIGH_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_HIGH,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#endif


#ifndef FEATURE_IMS_PC_TESTBENCH
#define LOG_VERBOSE_ERR( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_verbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_ERROR, \
	(_inst_id - 1), \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_ERR( \
	_mod,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	log_grp_qtrace_nonverbose_hdl_id_tbl[log_mod_to_grp_tbl[_mod]], \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_ERROR, \
	(_inst_id-1), \
	_tag, \
	_summary, \
##__VA_ARGS__)
#define LOG_VERBOSE_ERR_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_ERROR, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)
#define LOG_NON_VERBOSE_ERR_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_ERROR, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)
#else
#define LOG_VERBOSE_ERR( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_ERROR, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_ERR( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_ERROR,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_VERBOSE_ERR_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(1, QTRACE_LEVEL_ERROR, _tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_ERR_WITH_HDL( \
	_loghandle, \
	_tags, \
	_inst_id, \
	_summary, \
	...\
	) \
    MockLogger(0, QTRACE_LEVEL_ERROR,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#endif


//LEVEL MACROS

//INTERNAL QTRACE LOGGING
#ifndef FEATURE_IMS_PC_TESTBENCH
#define LOG_VERBOSE_INT_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_VERBOSE_INT_MED_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_VERBOSE_INT_HIGH_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_HIGH, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_VERBOSE_INT_ERR_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_ERROR, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_INT_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_INT_MED_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_MED, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_INT_HIGH_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_HIGH, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#define LOG_NON_VERBOSE_INT_ERR_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
	QTRACE_INT(\
	_hdl, \
	QSH_CLT_OTHER, \
	QTRACE_LEVEL_ERROR, \
	_inst_id, \
	_tag, \
	_summary, \
##__VA_ARGS__)

#else

#define LOG_VERBOSE_INT_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(1, QTRACE_LEVEL_MED,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_VERBOSE_INT_MED_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(1, QTRACE_LEVEL_MED,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_VERBOSE_INT_HIGH_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(1, QTRACE_LEVEL_HIGH,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_VERBOSE_INT_ERR_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(1, QTRACE_LEVEL_ERROR,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#define LOG_NON_VERBOSE_INT_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(0, QTRACE_LEVEL_MED,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_NON_VERBOSE_INT_MED_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(0, QTRACE_LEVEL_MED,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_NON_VERBOSE_INT_HIGH_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(0, QTRACE_LEVEL_HIGH,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)
#define LOG_NON_VERBOSE_INT_ERR_WITH_HDL( \
	_hdl,\
	_tag, \
	_inst_id, \
	_summary, \
	... \
	) \
    MockLogger(0, QTRACE_LEVEL_ERROR,_tags, _inst_id, __FILENAME__, __LINE__, _summary, ##__VA_ARGS__)

#endif


#endif
