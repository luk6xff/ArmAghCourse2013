#include<LPC21xx.h>
#include "led.h"
#include "keyboard.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define COUNTER_ENABLE   1<<0
#define COUNTER_RESET    1<<1
#define INTERRUPT_ON_MR0 1<<0
#define RESET_ON_MR0     1<<1
#define STOP_ON_MR0      1<<2   
#define MR0_INTERRUPT		 1<<0

void InitTimer0(void);
void WaitOnTimer0(unsigned int uiTime);
void InitTimer0Match0(unsigned int iDelayTime);
void WaitOnTimer0Match0(void);
void TestProg1(void);
void TestProg2(void);

int main(void)
{
   //TestProg1();
	  TestProg2();
  return 0;
}

//////////////////////////////////////////////////////////
void InitTimer0(void)
{
	T0TCR|=COUNTER_ENABLE; 
}
//////////////////////////////////////////////////////////
void WaitOnTimer0(unsigned int uiTime){
	T0TCR|=COUNTER_RESET;
	T0TCR&=~(COUNTER_RESET);
	while(T0TC<(uiTime*15));
}
//////////////////////////////////////////////////////////
void InitTimer0Match0(unsigned int iDelayTime){
	T0MR0=15*iDelayTime;
	T0TCR=COUNTER_ENABLE;
	T0TC=0;
	T0MCR|=INTERRUPT_ON_MR0|RESET_ON_MR0;;
}
//////////////////////////////////////////////////////////
void WaitOnTimer0Match0(void){
	while((T0IR&MR0_INTERRUPT)==0);
	T0IR=MR0_INTERRUPT;
}
//////////////////////////////////////////////////////////
void TestProg1(void){ 
		Led_Init();
		InitTimer0();
    while(1)
    {
			WaitOnTimer0(1000000);
      Led_StepLeft();
    }
}
//////////////////////////////////////////////////////////
void TestProg2(void){ 
		Led_Init();
		InitTimer0Match0(1000000);
    while(1)
    {
			WaitOnTimer0Match0();
      Led_StepLeft();
    }
}	