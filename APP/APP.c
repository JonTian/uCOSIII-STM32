/*-------------------------------------------------------------------------

							软件主体

    
-------------------------------------------------------------------------*/

/*
*********************************************************************************************************
*                                             INCLUDE FILES
*********************************************************************************************************
*/
   
#include "bsp.h"
#include "App.h"
 
extern void MainTask(void);
extern void GUI_TOUCH_Exec(void);
 
static  OS_TCB      taskStartTCB;
static  CPU_STK     taskStartStk[STARTUP_TASK_STK_SIZE];         //启动任务的程序空间
 
static  OS_TCB      task1TCB;
static  CPU_STK     task1_stk[TASK1_STK_SIZE];
 
static  OS_TCB      task2TCB;
static  CPU_STK     task2_stk[TASK2_STK_SIZE];
 
static  OS_TCB      task3TCB;
static  CPU_STK     task3_stk[TASK3_STK_SIZE];
 
static  OS_TCB      dispTCB;
static  CPU_STK     dispStk[TASK4_STK_SIZE];
 
static  volatile    OS_SEM taskSem;
 
 

/*******************************************************************************
* Function Name :void TaskStart(void)
* Description   :任务启动
* Input         :
* Output        :
* Other         :
* Date          :2012.04.18  11:48:23
*******************************************************************************/
static void TaskStart(void)
{
    OS_ERR 		err;
    
    led_init();
    usart1_init(115200);
    SPI_Config() ;
    SysTickInit();
    
    
    OSTaskCreate(   (OS_TCB     *)&task1TCB,
                    (CPU_CHAR   *)"Task1",
                    (OS_TASK_PTR)Task1,
                    (void       *)0,
                    (OS_PRIO    )TASK1_PRIO,
                    (CPU_STK    *)&task1_stk[0],
                    (CPU_STK_SIZE)TASK1_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK1_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
    
    OSTaskCreate(   (OS_TCB     *)&task2TCB,
                    (CPU_CHAR   *)"Task2",
                    (OS_TASK_PTR)Task2,
                    (void       *)0,
                    (OS_PRIO    ) TASK2_PRIO,
                    (CPU_STK    *)&task2_stk[0],
                    (CPU_STK_SIZE)TASK2_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK2_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);	
    
    
    OSTaskCreate(   (OS_TCB     *)&task3TCB,
                    (CPU_CHAR   *)"Task3",
                    (OS_TASK_PTR)Task3,
                    (void       *)0,
                    (OS_PRIO    )TASK3_PRIO,
                    (CPU_STK    *)&task3_stk[0],
                    (CPU_STK_SIZE)TASK3_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK3_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);
    
    OSTaskCreate(   (OS_TCB     *)&dispTCB,
                    (CPU_CHAR   *)"LCD display",
                    (OS_TASK_PTR)MainTask,
                    (void       *)0,
                    (OS_PRIO    )Disp_PRIO,
                    (CPU_STK    *)&dispStk[0],
                    (CPU_STK_SIZE)TASK4_STK_SIZE / 10,
                    (CPU_STK_SIZE)TASK4_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);   
    
    OSTaskCreate(   (OS_TCB     *)&schedTaskEx1TCB,
                    (CPU_CHAR   *)"Sched Task Example1",
                    (OS_TASK_PTR)SchedEx1Task,
                    (void       *)0,
                    (OS_PRIO    )Disp_PRIO,
                    (CPU_STK    *)&dispStk[0],
                    (CPU_STK_SIZE)SCHED_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)SCHED_TASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);  
        
    OSTaskCreate(   (OS_TCB     *)&schedTaskEx2TCB,
                    (CPU_CHAR   *)"Sched Task Example1",
                    (OS_TASK_PTR)SchedEx2Task,
                    (void       *)0,
                    (OS_PRIO    )Disp_PRIO,
                    (CPU_STK    *)&dispStk[0],
                    (CPU_STK_SIZE)SCHED_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)SCHED_TASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);  


    
    
    OSSemCreate(    (OS_SEM     *)&taskSem, 
                    (CPU_CHAR   *)"taskSem", 
                    (OS_SEM_CTR )0, 
                    (OS_ERR     *)err);
    
    OSTaskDel(      (OS_TCB     *)&taskStartTCB, 
                    (OS_ERR     *)&err);
}

/**
  *     @brief:         Task1
  *     @param  in:     None
  *     @param out:     None
  *
  *     @retval:        None
  */

static void Task1(void *p_arg)
{
    OS_ERR err;
    
    while (1)
    {
        led_on(LED_4);
        printf("task1 is running\r\n");
        OSTimeDly(  (OS_TICK    )200, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
        
        led_off(LED_4);
        OSTimeDly(  (OS_TICK    )200, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
        
        OSSemPost(  (OS_SEM     *)&taskSem, 
                    (OS_OPT     )OS_OPT_POST_ALL, 
                    (OS_ERR     *)&err);
    }
}


/**
  *     @brief:         Task2
  *     @param  in:     None
  *     @param out:     None
  *
  *     @retval:        None
  */
static void Task2(void *p_arg)
{
    OS_ERR err;

    while (1)
    {
        led_on(LED_5);
        printf("task2 is running\r\n");
        OSSemPend(  (OS_SEM     *)&taskSem, 
                    (OS_TICK    )10000, 
                    (OS_OPT     )OS_OPT_PEND_BLOCKING, 
                    (CPU_TS     *)0, 
                    (OS_ERR     *)&err);
        
        led_off(LED_5);
        OSSemPend(  (OS_SEM     *)&taskSem, 
                    (OS_TICK    )10000, 
                    (OS_OPT     )OS_OPT_PEND_BLOCKING, 
                    (CPU_TS     *)0, 
                    (OS_ERR     *)&err);
    }
}


/**
  *     @brief:         Task3
  *     @param  in:     None
  *     @param out:     None
  *
  *     @retval:        None
  */
static void Task3(void *p_arg)
{
    OS_ERR err;
    static int tasCon = 0;
    while (1)
    {      
        GUI_TOUCH_Exec();
        printf("task3 is running\r\n");
        OSTimeDly(  (OS_TICK    )10, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);
        
        OSTimeDly(  (OS_TICK    )10, 
                    (OS_OPT     )OS_OPT_TIME_DLY, 
                    (OS_ERR     *)&err);

        if ((tasCon++ % 50) < 25)
            led_on(LED_3);
        else
            led_off(LED_3);
    }       
}


/**
  *     @brief:         SchedTaskEx1
  *     @param  in:     None
  *     @param out:     None
  *
  *     @retval:        None
  */
static void SchedEx1Task(void *p_arg)
{
  OS_ERR err;
  
  uint16_t i;
  uint16_t schedTask1Counter;
  
  
  SchedTaskExiHook();
  
  led_on(LED_2);
  
}


/**
  *     @brief:         SchedTaskEx2
  *     @param  in:     None
  *     @param out:     None
  *
  *     @retval:        None
  */
static void SchedEx2Task(void *p_arg)
{
  OS_ERR err;
  
  uint16_t i;
  uint16_t schedTask2Counter;
  
  
  SchedTaskExiHook();
  
  led_on(LED_2);
  
}


void KernelMain(void)
{
    OS_ERR err;
    //CPU Init :
    CPU_Init();
    //OS Init :
    //          1.Idle
    //          2.Tick
    //          3.Statistic
    //          4.Timer Manager
    //          5.debug
    //          
    //          6.parameter init
    OSInit(         (OS_ERR     *)&err);
    
    OSTaskCreate(   (OS_TCB     *)&taskStartTCB,
                    (CPU_CHAR   *)"Task Start",
                    (OS_TASK_PTR)TaskStart,
                    (void       *)0,
                    (OS_PRIO    ) STARTUP_TASK_PRIO,
                    (CPU_STK    *)&taskStartStk[0],
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE / 10,
                    (CPU_STK_SIZE)STARTUP_TASK_STK_SIZE,
                    (OS_MSG_QTY )0,
                    (OS_TICK    )0,
                    (void       *)0,
                    (OS_OPT     )(OS_OPT_TASK_STK_CHK | OS_OPT_TASK_STK_CLR), 
                    (OS_ERR     *)&err);    
    OSStart(        (OS_ERR     *)&err);
}


