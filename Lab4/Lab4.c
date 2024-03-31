#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include "tm4c123gh6pm.h"

/* Declaring functions we may use */
#define SYSCTL_RCGC2_R          (*((volatile unsigned long *)0x400FE108)) 
#define SYSCTL_RCGC2_GPIOF			0x00000020 	 // Port F Clock Gating Control
#define GPIO_PORTB_DATA_R       (*((volatile unsigned long *)0x400053FC))
#define GPIO_PORTB_AMSEL_R      (*((volatile unsigned long *)0x40005528))
#define GPIO_PORTB_PCTL_R       (*((volatile unsigned long *)0x4000552C))
#define GPIO_PORTB_DIR_R        (*((volatile unsigned long *)0x40005400))
#define GPIO_PORTB_AFSEL_R      (*((volatile unsigned long *)0x40005420))
#define GPIO_PORTB_DEN_R        (*((volatile unsigned long *)0x4000551C))
#define GPIO_PORTF_DATA_R       (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define GPIO_PORTF_DIR_R        (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_DEN_R        (*((volatile unsigned long *)0x4002551C))
#define NVIC_ST_CTRL_R          (*((volatile unsigned long *)0xE000E010))
#define NVIC_ST_RELOAD_R        (*((volatile unsigned long *)0xE000E014))
#define NVIC_ST_CURRENT_R       (*((volatile unsigned long *)0xE000E018))
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define GPIO_PORTE_AMSEL_R      (*((volatile unsigned long *)0x40024528))
#define GPIO_PORTE_PCTL_R       (*((volatile unsigned long *)0x4002452C))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_AFSEL_R      (*((volatile unsigned long *)0x40024420))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))



void init_ports(void);
void SysTick_Init(void);
void SysTick_Wait(unsigned long delay);
void SysTick_ms(unsigned long A);

unsigned long SW, Walk, SW1;

void init_ports(void){

	volatile unsigned long delay;
  SYSCTL_RCGC2_R |= 0x00000032;   
  delay = SYSCTL_RCGC2_R; 
	
	GPIO_PORTE_AMSEL_R = 0x00;							   
	//Specifie PE0-5 output (South and West traffic lights)
	GPIO_PORTE_PCTL_R = 0x00000000;            
	//Enabling pins as digital
	GPIO_PORTE_DIR_R = 0x3F;					        
	//Intialize data to 0
	GPIO_PORTE_AFSEL_R = 0x00;
	GPIO_PORTE_DEN_R = 0x3F;
	
	GPIO_PORTF_AMSEL_R = 0x00;						     
	//walk LED outputs PF1, PF3
	GPIO_PORTF_PCTL_R = 0x00000000;
	GPIO_PORTF_DIR_R = 0x0A;					
	GPIO_PORTF_AFSEL_R = 0x00;	
	GPIO_PORTF_DEN_R = 0x0A;	
	
  GPIO_PORTB_AMSEL_R = 0x00;                
	//inputs PB0, PB1, PB2
	GPIO_PORTB_PCTL_R = 0x00000000;						
	//Specifie PB0-2 inputs (walk bottn-south sensor-west sensor) respectively
	GPIO_PORTB_DIR_R = 0x00;					 				
	//Enabling pins as digital
	GPIO_PORTB_AFSEL_R = 0x00;								
	//Intialize data to 0
	GPIO_PORTB_DEN_R = 0x07;
	
}

// initialization SysTick 
void SysTick_Init(void){
  NVIC_ST_CTRL_R = 0;                   
  NVIC_ST_RELOAD_R = 0x00FFFFFF;        
  NVIC_ST_CURRENT_R = 0;                             
  NVIC_ST_CTRL_R = 0x00000005;          
}

void SysTick_Wait(unsigned long delay){
  NVIC_ST_RELOAD_R = delay-1;                
  NVIC_ST_CURRENT_R = 0;                    
  while((NVIC_ST_CTRL_R&0x00010000)==0){ 		
		
  }
}

void SysTick_ms(unsigned long A){ 
  unsigned long i;
  for(i=0; i<A; i++){
    SysTick_Wait(80000000);  
  }
}

int main(void){

init_ports();
SysTick_Init();
	
	//State values
	int WGreen =	  0;
	int WYellow =   1;
	int SGreen = 	  2;
	int SYellow =   3;
	int walkGreen = 4;
	int walkRun = 	5;
	int walkStop =  6;
	int walkRun1 =  7;
	int walkStop1 = 8;
	int State = WGreen; 

	
while(1){
	
	switch (State)
		{
		
		case 0:
			
			GPIO_PORTE_DATA_R = 0x21;
			GPIO_PORTF_DATA_R = 0x02;
			
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
		
			if(Walk == 0x04 && SW1 == 0x00){ 
			SysTick_ms(10);
			}
			else if(Walk == 0x04 && SW1 != 0x00){
			SysTick_ms(11);
			}
			else SysTick_ms(11);
			
			if(SW == 0x00 || SW == 0x01) 
			State = WGreen;
			else State = WYellow;
		
				break;
		
			
			
			
		case 1:
			
			GPIO_PORTE_DATA_R = 0x22;
			GPIO_PORTF_DATA_R = 0x02;
			SysTick_ms(5);
		
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
			
			if(SW == 0x00 || SW == 0x01 || SW == 0x02 || SW == 0x03 || SW == 0x07) State = SGreen;
			else State = walkGreen;
		
				break;
		
		
		
		
		case 2:
			
			GPIO_PORTE_DATA_R = 0x0C;
			GPIO_PORTF_DATA_R = 0x02;
			
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
		
			if(Walk == 0x04 && SW1 == 0x00){ 
			SysTick_ms(10);
			}
			else if(Walk == 0x04 && SW1 != 0x00){
			SysTick_ms(11);
			}
			else SysTick_ms(11);
			
			if(SW == 0x00 || SW == 0x02) 
			State = SGreen;
			else State = SYellow;
		
				break;
		
			
			
		case 3:
			
			GPIO_PORTE_DATA_R = 0x14;
			GPIO_PORTF_DATA_R = 0x02;
			SysTick_ms(5);
		
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
				
			if(SW == 0x00 || SW == 0x01 || SW == 0x02 || SW == 0x03) 
			State = WGreen;
			else State = walkGreen;
		
				break;
		
			
		case 4: 
			
			GPIO_PORTE_DATA_R = 0x24;
			GPIO_PORTF_DATA_R = 0x08;
			
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
		
			if(Walk == 0x04 && SW1 == 0x00){ 
			SysTick_ms(10);
			}
			else if(Walk == 0x04 && SW1 != 0x00){
			SysTick_ms(11);
			}
			else SysTick_ms(11);
			
			if(SW == 0x00 || SW == 0x04) 
			State = walkGreen;
			else State = walkRun;
		
				break;
		
			
			
			
		case 5:
			
			GPIO_PORTE_DATA_R = 0x24;
			GPIO_PORTF_DATA_R = 0x02;
			SysTick_ms(5);
		
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
			
			if(SW == 0x04) 
			State = walkGreen;
			else State = walkStop;
		
				break;
		
			
			
			
		case 6:
			
			GPIO_PORTE_DATA_R = 0x24;
			GPIO_PORTF_DATA_R = 0x00;
			SysTick_ms(5);
		
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
					
			if(SW == 0x04) 
			State = walkGreen;
			else State = walkRun1;
		
				break;
		
			
			
		case 7:
			
			GPIO_PORTE_DATA_R = 0x24;
			GPIO_PORTF_DATA_R = 0x02;
			SysTick_ms(5);
		
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
			
			if(SW == 0x04) 
			State = walkGreen;
			else State = walkStop1;
		
				break;
		
			
			
			
		case 8:
			
			GPIO_PORTE_DATA_R = 0x24;
			GPIO_PORTF_DATA_R = 0x00;
			SysTick_ms(5);
		
			Walk = GPIO_PORTB_DATA_R&0x04;
			SW1 = GPIO_PORTB_DATA_R&0x03; 
			SW = Walk | SW1;
		
			if(SW == 0x02 || SW == 0x06) 
			State = SGreen;
			else if(SW == 0x04) 
			State = walkGreen;
			else State = WGreen;
		
				break;
		
		} 
		

	
} 
} 
