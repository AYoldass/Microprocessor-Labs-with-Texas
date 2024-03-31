#include<stdint.h>
#include<stdlib.h>

#define SYSCTL_RCGC2_R  (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_RCGC2_GPIOF  0x00000020 //PORTF Clock Gating Control
#define GPIO_PORTF_DATA_R (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R (*((volatile unsigned long *)0x4002551C))

void delay(int sec);
int cond;

int main(void){
		SYSCTL_RCGC2_R = SYSCTL_RCGC2_GPIOF;
		GPIO_PORTF_DIR_R = 0x0E;
		GPIO_PORTF_DEN_R =0x0E;
		cond=0;
		while(1){
			if(cond==0){
				GPIO_PORTF_DATA_R=0x02; //Turn on red for 3 second
				delay(3);
				cond=1;
			}
			else if(cond==1){
				GPIO_PORTF_DATA_R=0x0A; //Turn on yellow for 1 second
				delay(1);
				cond=2;
			}
			else if(cond==2){
				GPIO_PORTF_DATA_R=0x08; //Turn on green for 2 second
				delay(2);
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
 