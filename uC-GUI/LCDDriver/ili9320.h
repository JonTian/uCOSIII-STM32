#include "stm32f10x.h"

//定义驱动IC是ILI9320还是ILI9325
#define  ILI9325 //ILI9325

//屏幕旋转定义 数字按照 ID[1:0]AM 按照PDF中的配置定义
#define ID_AM  110

//硬件相关的子函数
#define Lcd_Light_ON   GPIO_SetBits(GPIOC, GPIO_Pin_7);
#define Lcd_Light_OFF  GPIO_ResetBits(GPIOC, GPIO_Pin_7);
/*
#define nCsPin  GPIO_Pin_8
#define RsPin   GPIO_Pin_9
#define nWrPin  GPIO_Pin_10
#define nRdPin  GPIO_Pin_11
#define nRstPin GPIO_Pin_12
#define Lcd_LightPin GPIO_Pin_13

#define Set_nWr (*((volatile unsigned long *) 0x40011010) = nWrPin)
#define Clr_nWr (*((volatile unsigned long *) 0x40011014) = nWrPin)

#define Set_Cs  (*((volatile unsigned long *) 0x40011010) = nCsPin)
#define Clr_Cs  (*((volatile unsigned long *) 0x40011014) = nCsPin)

#define Set_Rs  (*((volatile unsigned long *) 0x40011010) = RsPin)
#define Clr_Rs  (*((volatile unsigned long *) 0x40011014) = RsPin)

#define Set_nRd (*((volatile unsigned long *) 0x40011010) = nRdPin)
#define Clr_nRd (*((volatile unsigned long *) 0x40011014) = nRdPin)

#define Set_Rst (*((volatile unsigned long *) 0x40011010) = nRstPin)
#define Clr_Rst (*((volatile unsigned long *) 0x40011014) = nRstPin)

#define Lcd_Light_ON   (*((volatile unsigned long *) 0x40011010) = Lcd_LightPin)
#define Lcd_Light_OFF  (*((volatile unsigned long *) 0x40011014) = Lcd_LightPin)
*/
/* LCD Registers */
#ifndef     R0 

#define R0             0x00
#define R1             0x01
#define R2             0x02
#define R3             0x03
#define R4             0x04
#define R5             0x05
#define R6             0x06
#define R7             0x07
#define R8             0x08
#define R9             0x09
#define R10            0x0A
#define R12            0x0C
#define R13            0x0D
#define R14            0x0E
#define R15            0x0F
#define R16            0x10
#define R17            0x11
#define R18            0x12
#define R19            0x13
#define R20            0x14
#define R21            0x15
#define R22            0x16
#define R23            0x17
#define R24            0x18
#define R25            0x19
#define R26            0x1A
#define R27            0x1B
#define R28            0x1C
#define R29            0x1D
#define R30            0x1E
#define R31            0x1F
#define R32            0x20
#define R33            0x21
#define R34            0x22
#define R36            0x24
#define R37            0x25
#define R40            0x28
#define R41            0x29
#define R43            0x2B
#define R45            0x2D
#define R48            0x30
#define R49            0x31
#define R50            0x32
#define R51            0x33
#define R52            0x34
#define R53            0x35
#define R54            0x36
#define R55            0x37
#define R56            0x38
#define R57            0x39
#define R59            0x3B
#define R60            0x3C
#define R61            0x3D
#define R62            0x3E
#define R63            0x3F
#define R64            0x40
#define R65            0x41
#define R66            0x42
#define R67            0x43
#define R68            0x44
#define R69            0x45
#define R70            0x46
#define R71            0x47
#define R72            0x48
#define R73            0x49
#define R74            0x4A
#define R75            0x4B
#define R76            0x4C
#define R77            0x4D
#define R78            0x4E
#define R79            0x4F
#define R80            0x50
#define R81            0x51
#define R82            0x52
#define R83            0x53
#define R96            0x60
#define R97            0x61
#define R106           0x6A
#define R118           0x76
#define R128           0x80
#define R129           0x81
#define R130           0x82
#define R131           0x83
#define R132           0x84
#define R133           0x85
#define R134           0x86
#define R135           0x87
#define R136           0x88
#define R137           0x89
#define R139           0x8B
#define R140           0x8C
#define R141           0x8D
#define R143           0x8F
#define R144           0x90
#define R145           0x91
#define R146           0x92
#define R147           0x93
#define R148           0x94
#define R149           0x95
#define R150           0x96
#define R151           0x97
#define R152           0x98
#define R153           0x99
#define R154           0x9A
#define R157           0x9D
#define R192           0xC0
#define R193           0xC1
#define R229           0xE5
#endif

/* LCD Control pins */
#define CtrlPin_NCS    GPIO_Pin_2   /* PB.02 */
#define CtrlPin_RS     GPIO_Pin_7   /* PD.07 */
#define CtrlPin_NWR    GPIO_Pin_15  /* PD.15 */

/* LCD color */
#ifndef 	White
#define White          0xFFFF
#define Black          0x0000
#define Grey           0xF7DE
#define Blue           0x001F
#define Blue2          0x051F
#define Red            0xF800
#define Magenta        0xF81F
#define Green          0x07E0
#define Cyan           0x7FFF
#define Yellow         0xFFE0
#endif

#define HyalineBackColor 0x0001  // Lihao 透明背景

#ifndef Line0
#define Line0          0
#define Line1          24
#define Line2          48
#define Line3          72
#define Line4          96
#define Line5          120
#define Line6          144
#define Line7          168
#define Line8          192
#define Line9          216


#define Horizontal     0x00
#define Vertical       0x01
#endif

extern u16 DeviceCode;

void LCD_DB_AS_InPut(void);
void LCD_DB_AS_OutPut(void);

extern void ili9320_Initializtion(void);
void ili9320_SetCursor(u16 x,u16 y);
void ili9320_SetWindows(u16 StartX,u16 StartY,u16 EndX,u16 EndY);
void ili9320_DrawPicture(u16 StartX,u16 StartY,u16 EndX,u16 EndY,u16 *pic);
void ili9320_SetPoint(u16 x,u16 y,const u16 point);
void ili9320_PutChar(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
void ili9320_Clear(u16 dat);
void ili9320_Delay(u32 nCount);
u16 ili9320_GetCode(void);;

void ili9320_BackLight(u8 status);

u16 ili9320_BGR2RGB(u16 c);

u16 ili9320_GetPoint(u16 x,u16 y);;
void ili9320_PutChar_16x24(u16 x,u16 y,u8 c,u16 charColor,u16 bkColor);
u16 GUI_Color565(u32 RGB);  // RGB颜色转为16位(565)

void GUI_Text(u16 x, u16 y, u8 *str, u16 len,u16 Color, u16 bkColor);
void GUI_Line(u16 x0, u16 y0, u16 x1, u16 y1,u16 color);  // 画线
void GUI_Circle(u16 cx,u16 cy,u16 r,u16 color,u8 fill);  // 画园
void GUI_Rectangle(u16 x0, u16 y0, u16 x1, u16 y1,u16 color,u8 fill); // 画矩形
void GUI_Square(u16 x0, u16 y0, u16 with, u16 color,u8 fill);  // 画正方形


void Touch_Initializtion(void);
u16  Touch_GetPhyX(void);
u16  Touch_GetPhyY(void);
u16  Touch_MeasurementX(void);
u16  Touch_MeasurementY(void);


