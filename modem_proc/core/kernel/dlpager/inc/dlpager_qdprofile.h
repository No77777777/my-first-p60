#ifndef DLPAGER_QDPROFILE_H
#define DLPAGER_QDPROFILE_H

#include <qurt.h>

#define DLPAGER_QDPROFILE 1 // Enable/disable qdprofile

void dlpager_qdprofile_fault_to_ipa_start(qurt_thread_t task_id);
void dlpager_qdprofile_fault_to_ipa_stop(qurt_thread_t task_id);
void dlpager_qdprofile_ipa_to_resume_start(qurt_thread_t task_id);
void dlpager_qdprofile_ipa_to_resume_stop(qurt_thread_t task_id);

#endif
