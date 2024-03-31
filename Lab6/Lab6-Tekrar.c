#include <stdio.h>
#include <stdint.h>
#include "TM4C123GH6PM.h"



	#define SYSCTL_RCGCGPIO_R       			(*((volatile unsigned long *)0x400FE608))
	#define SYSCTL_RCGCADC_R              (*((volatile unsigned long *)0x400FE638))
	#define SYSCTL_PRGPIO_R               (*((volatile unsigned long *)0x400FEA08))
	#define SYSCTL_PRADC_R0                0x00000001  // ADC Module 0 Peripheral Ready
	#define SYSCTL_RCGC2_R                (*((volatile unsigned long *)0x400FE108))
	#define SYSCTL_RCC2_R                 (*((volatile unsigned long *)0x400FE070))
	#define SYSCTL_RCC_R                  (*((volatile unsigned long *)0x400FE060))
	#define SYSCTL_RIS_R                  (*((volatile unsigned long *)0x400FE050))
	#define GPIO_PORTE_DIR_R       			  (*((volatile unsigned long *)0x40024400))
	#define GPIO_PORTE_AFSEL_R      		 	(*((volatile unsigned long *)0x40024420))
	#define GPIO_PORTE_AMSEL_R            (*((volatile unsigned long *)0x40024528))
	#define GPIO_PORTE_DEN_R              (*((volatile unsigned long *)0x4002451C))
	#define ADC0_ACTSS_R                  (*((volatile unsigned long *)0x40038000))
	#define ADC0_EMUX_R                   (*((volatile unsigned long *)0x40038014))
	#define ADC0_SSMUX3_R                 (*((volatile unsigned long *)0x400380A0))
	#define ADC0_SSCTL3_R                 (*((volatile unsigned long *)0x400380A4))
	#define ADC0_RIS_R                    (*((volatile unsigned long *)0x40038004))
	#define ADC0_PSSI_R                   (*((volatile unsigned long *)0x40038028))
	#define ADC0_SSFIFO3_R        			  (*((volatile unsigned long *)0x400380A8))
	#define ADC0_ISC_R                    (*((volatile unsigned long *)0x4003800C))
	#define ADC0_IM_R                     (*((volatile unsigned long *)0x40038008))
	#define ADC0_PC_R                     (*((volatile unsigned long *)0x40038FC4))
	#define ADC0_SSPRI_R              		(*((volatile unsigned long *)0x40038020))
	#define GPIO_PORTF_DATA_R     				(*((volatile unsigned long *)0x400253FC))
	#define GPIO_PORTF_DIR_R   	    			(*((volatile unsigned long *)0x40025400))
	#define GPIO_PORTF_DEN_R   	    			(*((volatile unsigned long *)0x4002551C))
	#define GPIO_PORTF_AFSEL_R    				(*((volatile unsigned long *)0x40025420))
	#define GPIO_PORTF_PCTL_R       			(*((volatile unsigned long *)0x4002552C))
	#define GPIO_PORTF_AMSEL_R   					(*((volatile unsigned long *)0x40025528))
	

uint32_t MeasDist(void);                                        //Measure disatance
	
	void UART5_init(void);
	void UART5_Transmitter(unsigned char data);
	void Timer0ACapture_init(void);
	void delay_ms(int time);
	void delay(unsigned long counter);
	void printstring(char *str);        
// global variables to store and display distance in cm 

	
	uint32_t time;                                            
	//pulse on time
	uint32_t distance;                                       
	//measured distance value 
	char mesg[20];                                          
	// string format of distance value 

// main code to take distance measurement and send data to UART terminal 
int main(void)
{
SYSCTL_RCGCGPIO_R |=(1<<5) ;
	while((SYSCTL_RCGCGPIO_R && 0x00000020 ==0) ) {}
		
GPIO_PORTF_DIR_R = 0x1E;
GPIO_PORTF_DEN_R = 0x1E;

	

Timer0ACapture_init();                                   
UART5_init();                                          



		while(1)

	{

time = MeasDist();                                   
		
distance = (time * 10625)/10000000;                  
		// convert pulse duration into distance 
		
    
    		if(distance <= 10 ) {
					GPIO_PORTF_DATA_R= 0x02; 			
				}
				else if(distance >10 && distance <=20 ) {
					GPIO_PORTF_DATA_R= 0x04;
				}
				else if(distance >20 && distance <=30 ) {
					GPIO_PORTF_DATA_R= 0x0A;
				}
				else 
					GPIO_PORTF_DATA_R= 0x06;
			

				sprintf(mesg, "\r\nDist: = %d cm", distance); 												  //convert float type distance data into string 
				printstring(mesg); 																										 //transmit data to computer 

delay(2000);

	}
	
}



uint32_t MeasDist(void)
{
    int LastEdge, FirstEdge;
	
	  // Given 20us trigger pulse 
	  GPIOA->DATA &= ~(1<<4);                                 // make trigger  pin high 
	  delay_ms(20);                                          //20 seconds delay 
	  GPIOA->DATA |= (1<<4);                                // make trigger  pin high 
	  delay_ms(20);                                        //20 seconds delay 
	  GPIOA->DATA &= ~(1<<4);                             // make trigger  pin low 

 	while(1)
	{
    TIMER0->ICR = 4;                                            // clear timer0A capture flag 
    while((TIMER0->RIS & 4) == 0) ;                            
		// wait till captured 
	  if(GPIOB->DATA & (1<<6))                                  //check if rising edge occurs 
		{
    LastEdge = TIMER0->TAR;                                      
		// detect falling edge 
    TIMER0->ICR = 4;                                            // clear timer0A capture flag 
    while((TIMER0->RIS & 4) == 0) ;                            
			// wait till captured 
    FirstEdge = TIMER0->TAR;    														  
		return (FirstEdge - LastEdge);													 
		}
	}
}



// Create one microsecond second delay using Timer block 1 and sub timer A 
void delay_ms(int time)
{
    int i;
    SYSCTL->RCGCTIMER |= 2;                          
  // enable clock to Timer Block 1 
    TIMER1->CTL = 0;                                
  // disable Timer before initialization 
    TIMER1->CFG = 0x04;                             
	// 16-bit option 
    TIMER1->TAMR = 0x02;                          
  // periodic mode and down-counter 
    TIMER1->TAILR = 16 - 1;                       
	// TimerA interval load value reg 
    TIMER1->ICR = 0x1;         									  
	// clear the TimerA timeout flag 
    TIMER1->CTL |= 0x01;     									   
	// enable Timer A after initialization 

    for(i = 0; i < time; i++)
    {
        while ((TIMER1->RIS & 0x1) == 0) ;      // wait for TimerA timeout flag 
        TIMER1->ICR = 0x1;                     // clear the TimerA timeout flag 
    }
}


    
void Timer0ACapture_init(void)
{
    SYSCTL->RCGCTIMER |= 1;    																
  // enable clock to Timer Block 0 
    SYSCTL->RCGCGPIO |= 2;   																  
	// enable clock to PORTB 
    
    GPIOB->DIR &= ~0x40;     														  
  // make PB6 an input pin 
    GPIOB->DEN |= 0x40;        														
	// make PB6 as digital pin 
    GPIOB->AFSEL |= 0x40;       												
	// use PB6 alternate function 
    GPIOB->PCTL &= ~0x0F000000; 											
	// configure PB6 for T0CCP0 
    GPIOB->PCTL |= 0x07000000;
    
	  SYSCTL->RCGCGPIO |= 1;     														
	// enable clock to PORTA 
	  GPIOA->DIR |=(1<<4);       													 
	// set PB2 as a digial output pin 
	  GPIOA->DEN |=(1<<4);      												  
	// make PB2 as digital pin 

    TIMER0->CTL &= ~1;       													
		// disable timer0A during setup 
    TIMER0->CFG = 4;         													 
		// 16-bit timer mode 
    TIMER0->TAMR = 0x17;         											
		// up-count, edge-time, capture mode 
    TIMER0->CTL |=0x0C;        											
		// capture the rising edge 
    TIMER0->CTL |= (1<<0);           								
		// enable timer0A 
}


void UART5_init(void)
{
	  SYSCTL->RCGCUART |= 0x20;                          
  // enable clock to UART5 
    SYSCTL->RCGCGPIO |= 0x10;                        
	// enable clock to PORTE for PE4/Rx and PE5/Tx 
    
    UART5->CTL = 0;        															 // UART5 module disbable 
    UART5->IBRD = 104;    															// for 9600 baud rate, integer = 104 
    UART5->FBRD = 11;      														 // for 9600 baud rate, fractional = 11
    UART5->CC = 0;         													  //select system clock/
    UART5->LCRH = 0x60;   													 // data lenght 8-bit, not parity bit, no FIFO 
    UART5->CTL = 0x301;   													// Enable UART5 module, Rx and Tx 

    GPIOE->DEN = 0x30;      											
	// set PE4 and PE5 as digital 
    GPIOE->AFSEL = 0x30;    								 				
	// Use PE4,PE5 alternate function 
    GPIOE->AMSEL = 0;    															
    GPIOE->PCTL = 0x00110000;     								
	// configure PE4 and PE5 for UART 
}
void UART5_Transmitter(unsigned char data)  
{
    while((UART5->FR & (1<<5)) != 0);                
    UART5->DR = data;                              
}

void printstring(char *str)
{
  while(*str)
	{
		UART5_Transmitter(*(str++));
	}
}
void delay(unsigned long counter)
{
	unsigned long i = 0;
	
	for(i=0; i< counter*1000; i++);
}