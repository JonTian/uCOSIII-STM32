/*
*********************************************************************************************************
*                                             uC/GUI V3.98
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
----------------------------------------------------------------------
File        : GUI_TOUCH_X.C
Purpose     : Config / System dependent externals for GUI
---------------------------END-OF-HEADER------------------------------
*/


#include "GUI.h"
#include "GUI_X.h"
#include "ili9320.h"
#include "bsp.h"

//#define NEW8989_LCD	  // 如果触摸屏的触摸与指针的移动方向是反的请注释掉该宏定义

unsigned short int X,Y;

void GUI_TOUCH_X_ActivateX(void) {
}

void GUI_TOUCH_X_ActivateY(void) {
}


int  GUI_TOUCH_X_MeasureX(void) 
{
	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,X=0;	
 	
	while(/*GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0&&*/count<10)//循环读数10次
	{	   	  
#ifndef NEW8989_LCD
		databuffer[count]=TPReadX();
#else
		databuffer[count]=TPReadY();
#endif
		count++; 
	}  
	if(count==10)//一定要读到10次数据,否则丢弃
	{  
	    do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		X=(databuffer[3]+databuffer[4]+databuffer[5]+databuffer[6])>>2;	  
	}
#ifndef NEW8989_LCD
	return(X); 
#else
	return((X>100)?(X-100): 0); 
#endif
 
}

int  GUI_TOUCH_X_MeasureY(void) {
  	unsigned char t=0,t1,count=0;
	unsigned short int databuffer[10]={5,7,9,3,2,6,4,0,3,1};//数据组
	unsigned short temp=0,Y=0;	
 
    while(/*GPIO_ReadInputDataBit(GPIOB,GPIO_Pin_0)==0&&*/count<10)	//循环读数10次
	{	   	  
#ifndef NEW8989_LCD
		databuffer[count]=TPReadY();
#else
		databuffer[count]=TPReadX();
#endif
		count++;  
	}  
	if(count==10)//一定要读到10次数据,否则丢弃
	{  
	    do//将数据X升序排列
		{	
			t1=0;		  
			for(t=0;t<count-1;t++)
			{
				if(databuffer[t]>databuffer[t+1])//升序排列
				{
					temp=databuffer[t+1];
					databuffer[t+1]=databuffer[t];
					databuffer[t]=temp;
					t1=1; 
				}  
			}
		}while(t1); 	    		 	 		  
		Y=(databuffer[3]+databuffer[4]+databuffer[5]+databuffer[6])>>2;	  
	}
#ifndef NEW8989_LCD
	return(Y); 
#else
    Y = 4095-Y;
	return((Y>100)?(Y-100): 0); 
#endif
}

	 	 			 		    	 				 	  			   	 	 	 	 	 	  	  	      	   		 	 	 		  		  	 		 	  	  			     			       	   	 			  		    	 	     	 				  	 					 	 			   	  	  			 				 		 	 	 			     			 
