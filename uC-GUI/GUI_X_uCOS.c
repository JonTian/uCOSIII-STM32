/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              礐/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
---Author-Explanation
* 
* 1.00.00 020519 JJL    First release of uC/GUI to uC/OS-II interface
* 
*
* Known problems or limitations with current version
*
*    None.
*
*
* Open issues
*
*    None
*********************************************************************************************************
*/

#include "os.h"
#include "os_Cfg_app.h"
#include "GUI_Private.H"
#include "stdio.H"


/*
*********************************************************************************************************
*                                         GLOBAL VARIABLES
*********************************************************************************************************
*/

static  int        KeyPressed;
static  char       KeyIsInited;

static  OS_SEM      dispSem;
static  OS_SEM      eventSem;
static  OS_SEM      keySem;

/*
*********************************************************************************************************
*                                        TIMING FUNCTIONS
*
* Notes: Some timing dependent routines of uC/GUI require a GetTime and delay funtion. 
*        Default time unit (tick), normally is 1 ms.
*********************************************************************************************************
*/

int GUI_X_GetTime(void)
{
    OS_ERR err;
    
    return ((int)OSTimeGet( (OS_ERR *)&err));
}

void GUI_X_Delay(int period)
{
    OS_TICK  ticks;
    OS_ERR err;
    
    ticks = period * OS_CFG_TICK_RATE_HZ / 1000;
    OSTimeDly(  (OS_TICK	)ticks, 
                (OS_OPT		)OS_OPT_TIME_DLY, 
                (OS_ERR 	*)&err);
}


/*
*********************************************************************************************************
*                                          GUI_X_ExecIdle()
*********************************************************************************************************
*/
/*WM空闲时调用*/
void GUI_X_ExecIdle(void) 
{
    OS_ERR err;
    
    OSTimeDly(  (OS_TICK    )50, 
    			(OS_OPT     )OS_OPT_TIME_DLY, 
    			(OS_ERR     *)&err);
}


/*
*********************************************************************************************************
*                                    MULTITASKING INTERFACE FUNCTIONS
*
* Note(1): 1) The following routines are required only if uC/GUI is used in a true multi task environment, 
*             which means you have more than one thread using the uC/GUI API.  In this case the #define 
*             GUI_OS 1   needs to be in GUIConf.h
*********************************************************************************************************
*/

void GUI_X_InitOS (void)
{ 
    OS_ERR err;
    
    OSSemCreate(    (OS_SEM     *)&dispSem, //建立一个互斥型信号量
                    (CPU_CHAR   *)"dispSem", 
                    (OS_SEM_CTR )1, 
                    (OS_ERR     *)&err   );
    
    OSSemCreate(    (OS_SEM     *)&eventSem,
                    (CPU_CHAR   *)"eventSem", 
                    (OS_SEM_CTR )1, 
                    (OS_ERR 	*)&err   );
}


void GUI_X_Lock(void)
{ 
    OS_ERR err;
    CPU_TS ts;
    
    OSSemPend(  (OS_SEM 	*)&dispSem, 
                (OS_TICK	)0, 
                (OS_OPT		)OS_OPT_PEND_BLOCKING, 
                (CPU_TS 	*)&ts, 
                (OS_ERR 	*)&err);
}


void GUI_X_Unlock(void)
{ 
    OS_ERR err;
    
    OSSemPost(  (OS_SEM 	*)&dispSem, 
                (OS_OPT		)OS_OPT_POST_1, 
                (OS_ERR 	*)&err);
}


U32 GUI_X_GetTaskId(void)
{ 
  return ((U32)(OSTCBCurPtr->Prio));
}

/*
*********************************************************************************************************
*                                        GUI_X_WaitEvent()
*                                        GUI_X_SignalEvent()
*********************************************************************************************************
*/


void GUI_X_WaitEvent(void)
{
    OS_ERR err;
    CPU_TS ts;
    
    OSSemPend(  (OS_SEM 	*)&eventSem, 
                (OS_TICK	)0, 
                (OS_OPT		)OS_OPT_PEND_BLOCKING, 
                (CPU_TS 	*)&ts, 
                (OS_ERR 	*)&err);
}


void GUI_X_SignalEvent(void)
{
    OS_ERR err;

    OSSemPost(  (OS_SEM 	*)&eventSem, 
                (OS_OPT		)OS_OPT_POST_1, 
                (OS_ERR 	*)&err);
}

/*
*********************************************************************************************************
*                                      KEYBOARD INTERFACE FUNCTIONS
*
* Purpose: The keyboard routines are required only by some widgets.
*          If widgets are not used, they may be eliminated.
*
* Note(s): If uC/OS-II is used, characters typed into the log window will be placed	in the keyboard buffer. 
*          This is a neat feature which allows you to operate your target system without having to use or 
*          even to have a keyboard connected to it. (useful for demos !)
*********************************************************************************************************
*/

static void CheckInit(void)
{
    if(KeyIsInited==DEF_FALSE)
    {
        KeyIsInited = DEF_TRUE;
        GUI_X_Init();
    }
}


/*被GUI_Init()调用,用来初始化一些GUI运行之前需要用的硬件,如键盘或者鼠标之类的.如果不需要的话,可以为空*/
void GUI_X_Init(void)
{
    OS_ERR err;
    
    OSSemCreate(    (OS_SEM     *)&keySem,
                    (CPU_CHAR   *)"keySem", 
                    (OS_SEM_CTR )0, 
                    (OS_ERR 	*)&err   );
}


int GUI_X_GetKey(void)
{
    int r;
    r = KeyPressed;
    CheckInit();
    KeyPressed = 0;
    return (r);
}


int GUI_X_WaitKey(void)
{
    int  r;
    OS_ERR err;
    CPU_TS ts;
    
    CheckInit();
    if(KeyPressed==0)
    {
        OSSemPend(  (OS_SEM     *)&keySem,      //等待信号量
                    (OS_TICK    )0, 
                    (OS_OPT     )OS_OPT_PEND_BLOCKING, 
                    (CPU_TS     *)&ts, 
                    (OS_ERR     *)&err);
    }
    r= KeyPressed;
    KeyPressed = 0;
    return (r);
}


void GUI_X_StoreKey(int k)
{
    OS_ERR err;
    
    KeyPressed = k;
    OSSemPost(  (OS_SEM     *)&keySem,      //释放信号量
                (OS_OPT     )OS_OPT_POST_1, 
                (OS_ERR     *)&err);
}

void GUI_X_Log(const char *s) 
{ 
    GUI_USE_PARA(s); 
}

void GUI_X_Warn(const char *s) 
{
    GUI_USE_PARA(s); 
}

void GUI_X_ErrorOut(const char *s)
{ 
    GUI_USE_PARA(s); 
}
