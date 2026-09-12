/*======================================================================

 Technology Selector Event Manager Header File

 GENERAL DESCRIPTION
  This file contains declaration of Event Manager Class
  
 EXTERNALIZED FUNCTIONS


 INITIALIZATION AND SEQUENCING REQUIREMENTS

 Copyright (c) 2016 Qualcomm Technologies, Inc. 
  Qualcomm Technologies Confidential and Proprietary. All Rights Reserved.
 Export of this technology or software is regulated by the U.S. Government.
 Diversion contrary to U.S. law prohibited.
======================================================================*/

/*=====================================================================

 EDIT HISTORY FOR MODULE

 This section contains comments describing changes made to the module.
 Notice that changes are listed in reverse chronological order.

  $Header: //components/rel/gnss8.mpss/11.1/gnss/sm/tm/tech_sel/src/event/inc/tech_sel_event_mgr.h#1 $

 when      who   what, where, why
 ----      ----  --- --------------------------------------------------- 
03/02/16   ss    Initial creation of file.
======================================================================*/

#ifndef TECH_SEL_EVENT_MGR_H
#define TECH_SEL_EVENT_MGR_H

#include "tech_sel_common.h"
#include "tech_sel_event.h"

#define TECHSEL_MAX_EVENT_REG_TABLE_SIZE 25 /*Max Event Table Registration Table Size*/

/*Prototype for TechSel Event Callback Handler.
This shall be the signature of callback function that shall be registered with Event Manager*/
typedef boolean (techSel_EventHandlerCbk)(TechSelEvent* pz_event);

/*Event Manager Class*/
/*This class is responsible for routing events to interested parties. 
It mainly exposes two function: One to register for an event and other to post an event*/
class TechSelEventMgr{
  public:
    /*Public Member Functions*/    
    ~TechSelEventMgr(){};
    /*Singleton Instance*/
    static TechSelEventMgr* getInstance();
    /*Function to Post Event. Event Manager will route the event to interested parties*/
    boolean postEvent(TechSelEvent* pz_event);
    /*Function to register for an event by providing a callback function*/
    boolean registerForEvent(techSelEventType e_EventType, techSel_EventHandlerCbk *p_EventHandler);
    
  private:  
      /*Private Member variables*/
      /*Singleton Instance*/
      static TechSelEventMgr* m_pInstance;

      /*Datatype of registeration table element*/
      typedef struct
      {
        /*Interested Event type*/
        techSelEventType e_EventType;
        /*Pointer to callback which will  be called by the Event Manager if interested event is posted by anyone*/
        techSel_EventHandlerCbk *p_EventHandle;
      }techSelEventRegType;

      /*Registeration Table*/
      techSelEventRegType mz_EventRegTable[TECHSEL_MAX_EVENT_REG_TABLE_SIZE];

      /*Private Member Functions*/

      /*Private Constructor for Singleton*/
      TechSelEventMgr();
};


#endif

