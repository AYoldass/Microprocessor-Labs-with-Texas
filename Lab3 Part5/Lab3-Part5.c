#include <stdint.h>
#include <stdlib.h>






#define SYSCTL_RCGC2_GPIOE			0x00000010 
// Port E Clock Control
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08)) 
#define GPIO_PORTE_DATA_R       (*((volatile unsigned long *)0x400243FC))
#define SYSCTL_RCGC2_R				  (*((volatile unsigned long *)0x400FE108)) 
#define GPIO_PORTE_PUR_R        (*((volatile unsigned long *)0x40024510))
#define GPIO_PORTE_DEN_R        (*((volatile unsigned long *)0x4002451C))
#define GPIO_PORTE_DIR_R        (*((volatile unsigned long *)0x40024400))
#define GPIO_PORTE_CR_R         (*((volatile unsigned long *)0x40024524))
#define GPIO_PORTE_LOCK_R       (*((volatile unsigned long *)0x40024520))






#define toggle  (GPIO_PORTE_DATA_R & 0x01)




void delay (int time);
	


int main (void)
  {	
		SYSCTL_RCGC2_R = 0x10;														
		//activating clock of port E
		while((SYSCTL_PRGPIO_R & 0x00000010)==0){}			
			//waiting until the peripheral is ready
		GPIO_PORTE_DIR_R = 0x02; 												
			//PE0 is input,PE1 is out
		GPIO_PORTE_DEN_R = 0xff;												
			//enabling pins as digital
		GPIO_PORTE_DATA_R = 0;													
			//intialize data to 0
		 
double start;												  	    				
			//this is our start (1=100%)
int period;																			    
			//period 1000/f.
int A=2;																						
			//number of iterations for each diffrent s.																					
		 
while(1)
	


    {
		if(toggle==1)                                   
			//reading logical state of the switch
		{	
			start=0.2; period=125;               //start=dutycycle      		
			//setting the proper duty cycle and period for 8Hz blinking
			GPIO_PORTE_DATA_R = 0x02;              				
			//litting the led
			delay(period*start);                  		   
			GPIO_PORTE_DATA_R = 0x00;									 			
			//turning off the RGB.
			delay(period*(1-start));								  // dutycyle bekleme süresi 1-duty  
		}
		
		
		
		
		if(toggle==0)
		{	
			start=0; period=10;                         						
			//setting the proper duty cycle and period for led breathing
	  	
			for( start=0; start<=1; start += 0.02)  								
			//loop in order to increase the deuty cycle (5%)
			{
				for(int i=0; i<=A; i++)                             	
				//loop for iterating each brightness level for N number of
				{																											
					//iterations in order to be visible
					GPIO_PORTE_DATA_R = 0x00;														
					//LED off
					delay(period*(1-start));														
					//acteving x% OFF 
					GPIO_PORTE_DATA_R = 0x02;								 					
					//litting the led
					delay(period*start);															
					//acteving x% ON 
				}	
			}
			
			
			
		
			for( start=1; start>= 0; start-=0.02) 	  						
			//loop in order to decrease the deuty cycle (5%)
			{
				for(int i=0; i<=A; i++)															
				//loop for iterating each brightness level for N number of
				{																									
					//iterations in order to be visible
					GPIO_PORTE_DATA_R = 0x02;                					
					//litting the LED
					delay(period*start);  														
					//acteving x% ON 
					GPIO_PORTE_DATA_R = 0x00;								 				
					//led off
					delay(period*(1-start));													
					//acteving x% OFF 
				}	
			}
		}
	}
		
	
return 0;
}
	

void delay (int time )
	

{
	int x=1 , y=1 ;
	for(x=1 ; x<=time ; x++) 
	{ 
		for(y=1 ; y<=9000 ; y++){}
	}
}