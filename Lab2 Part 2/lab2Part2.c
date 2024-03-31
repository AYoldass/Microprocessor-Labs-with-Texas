#include<stdint.h>
#include<stdlib.h>


#define SYSCTL_RCGC2_R				(*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF			0x00000020 	 // Port F Clock Gating Control
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08))
#define SYSCTL_RCGCGPIO_R       (*((volatile unsigned long *)0x400FE608))
#define GPIO_PORTF_LOCK_R     	(*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524))
#define GPIO_PORTF_AMSEL_R      (*((volatile unsigned long *)0x40025528))
#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
#define GPIO_PORTF_DIR_R				(*((volatile unsigned long *)0x40025400)) 
#define GPIO_PORTF_AFSEL_R      (*((volatile unsigned long *)0x40025420))
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510))
#define GPIO_PORTF_DEN_R				(*((volatile unsigned long *)0x4002551C))
#define GPIO_PORTF_DATA_R				(*((volatile unsigned long *)0x400253FC))
#define sw2                 		GPIO_PORTF_DATA_R & 0x01
#define sw1                 		GPIO_PORTF_DATA_R & 0x10


void delay(int sec);
int cond=0;
int key2=0;
int num=0;

int main(void){
		SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
  	while((SYSCTL_PRGPIO_R & 0x00000020)==0){};  // check clock 
		GPIO_PORTF_LOCK_R = 0x4C4F434B;   //  unlock GPIO Port F	
		GPIO_PORTF_CR_R = 0x1F;           // allow changes to PF4-0	
		GPIO_PORTF_AMSEL_R = 0x00;        //  disable analog on PF	
		GPIO_PORTF_PCTL_R = 0x00000000;   //  PCTL GPIO on PF4-0	
		GPIO_PORTF_DIR_R = 0x0E;					//define my inputs and outputs
		GPIO_PORTF_PUR_R = 0x11;          // enable pull-up on PF0 and PF4	
		
		GPIO_PORTF_DEN_R = 0x1F; 	
	
	
		while(1){
			
				
			if((sw1)==0x00)
			{
				num=num+1;
				
			if(num==4)
				{
					num=0;
				}
			}
			if((sw2)==0)
			{
				key2=~key2;
			}
					
				if(cond==0){
					GPIO_PORTF_DATA_R=0x02; //Turn on red for 3+num second
					delay(3+num);
					cond=1;
					}
				else if(cond==1){
					if((key2)==0){
						GPIO_PORTF_DATA_R=0x0A; //Turn on yellow for 1+num second
						delay(1+num);
					}
					cond=2;
					}
				else if(cond==2){
					GPIO_PORTF_DATA_R=0x08; //Turn on green for 2+num second
					delay(2+num);
					cond=0;	
				}	

				
		}
		
		
		return 0;
}
 void delay(int sec)
 {
		int d=1, c=1;
		for(c=1;c<=sec;c++)
			for(d=1;d<=4000000;d++){}

 }
 


