#include <LPC21xx.H>
#include "timer_interrupts.h"
#include "led.h"

// TIMER
#define mCOUNTER_ENABLE 0x00000001
#define mCOUNTER_RESET  0x00000002

// CompareMatch
#define mINTERRUPT_ON_MR0 0x00000001
#define mRESET_ON_MR0     0x00000002
#define mMR0_INTERRUPT    0x00000001

// VIC (Vector Interrupt Controller) VICIntEnable
#define VIC_TIMER0_CHANNEL_NR 4
#define VIC_TIMER1_CHANNEL_NR 5

// VICVectCntlx Vector Control Registers
#define mIRQ_SLOT_ENABLE 0x00000020


///////////////////////////////////////TIMER0///////////////////////////////////////////
void (*ptrTimer0InterruptFunction)(void);
/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer0IRQHandler(){

	T0IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer0InterruptFunction(); 	// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer0Interrupts_Init(unsigned int uiPeriod, void(*ptrInterruptFunction)(void)){ // microseconds

	ptrTimer0InterruptFunction=ptrInterruptFunction;
        // interrupts
  VICIntEnable |= (0x1 << VIC_TIMER0_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl0  = mIRQ_SLOT_ENABLE | VIC_TIMER0_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr0  =(unsigned long)Timer0IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 
				
				// match module
	T0MR0 = 15 * uiPeriod;                 	      // value 
	T0MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer
	T0TCR |=  mCOUNTER_ENABLE; // start 
}



///////////////////////////////////////TIMER1///////////////////////////////////////////
void (*ptrTimer1InterruptFunction)(void);
/**********************************************/
//(Interrupt Service Routine) of Timer 0 interrupt
__irq void Timer1IRQHandler(){
	T1IR=mMR0_INTERRUPT; 	// skasowanie flagi przerwania 
	ptrTimer1InterruptFunction(); 	// cos do roboty
	VICVectAddr=0x00; 	// potwierdzenie wykonania procedury obslugi przerwania
}
/**********************************************/
void Timer1Interrupts_Init(unsigned int uiPeriod, void(*ptrInterruptFunction)(void)){ // microseconds

	ptrTimer1InterruptFunction=ptrInterruptFunction;
        // interrupts
  VICIntEnable |= (0x1 << VIC_TIMER1_CHANNEL_NR);            // Enable Timer 0 interrupt channel 
	VICVectCntl1  = mIRQ_SLOT_ENABLE | VIC_TIMER1_CHANNEL_NR;  // Enable Slot 0 and assign it to Timer 0 interrupt channel
	VICVectAddr1  =(unsigned long)Timer1IRQHandler; 	   // Set to Slot 0 Address of Interrupt Service Routine 
				
				// match module
	T1MR0 = 15 * uiPeriod;                 	      // value 
	T1MCR |= (mINTERRUPT_ON_MR0 | mRESET_ON_MR0); // action 

        // timer
	T1TCR |=  mCOUNTER_ENABLE; // start 
}
