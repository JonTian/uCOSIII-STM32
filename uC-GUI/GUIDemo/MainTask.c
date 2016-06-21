/*
*********************************************************************************************************
*                                                uC/GUI
*                        Universal graphic software for embedded applications
*
*                       (c) Copyright 2002, Micrium Inc., Weston, FL
*                       (c) Copyright 2002, SEGGER Microcontroller Systeme GmbH
*
*              µC/GUI is protected by international copyright laws. Knowledge of the
*              source code may not be used to write a similar product. This file may
*              only be used in accordance with a license and should not be redistributed
*              in any way. We appreciate your understanding and fairness.
*
----------------------------------------------------------------------
File        : MainTask.c
Purpose     : Init GUI & call of GUI-demo
--------------------END-OF-HEADER-------------------------------------
*/
#include "bsp.h"
#include "App.h"
#include "GUI.h"
#include "GUIDEMO.h"

static  volatile    OS_SEM taskSem;

void MainTask(void *p_arg) {
  OS_ERR err;
  
  GUI_Init();
  while(1) {
    GUIDEMO_main();
   
    OSSemPost(      (OS_SEM     *)&taskSem, 
                    (OS_OPT     )OS_OPT_POST_ALL, 
                    (OS_ERR     *)&err);
  }
}
	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
