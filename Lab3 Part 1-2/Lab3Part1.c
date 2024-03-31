#include <stdint.h>
#include <stdlib.h>



//Define my adresses.
#define GPIO_PORTF_DATA_R  (*((volatile unsigned long *)0x400253FC))
#define GPIO_PORTF_DIR_R   (*((volatile unsigned long *)0x40025400))
#define GPIO_PORTF_DEN_R   (*((volatile unsigned long *)0x4002551C))
#define SYSCTL_RCGC2_R     (*((volatile unsigned long *)0x400FE108))
#define SYSCTL_PRGPIO_R		 (*((volatile unsigned long *)0x400FEA08))
#define GPIO_PORTF_LOCK_R  (*((volatile unsigned long *)0x40025520))
#define GPIO_PORTF_CR_R    (*((volatile unsigned long *)0x40025524))




void delay (int time);
	


int main (void)
 


{	
	 
	SYSCTL_RCGC2_R = 0x20;												  	//activating clock of port F
	while((SYSCTL_PRGPIO_R & 0x00000020)==0){}		   	//waiting until the peripheral is ready
	GPIO_PORTF_LOCK_R=0x4C4F434B; 									  //unlock port F register
	GPIO_PORTF_CR_R=0xFF; 													  //set up my register
	GPIO_PORTF_DIR_R=0xEE; 												  	//define input and output
	GPIO_PORTF_DEN_R=0xFF;												   // I do my available  input and output.
	GPIO_PORTF_DATA_R=00;														 //clear my output.	
		
		
		
	double start=0.2;
  double stop=0.8;
	int counter=0;		
	while(1)
		
	
	
	{
		for(counter=0;counter<5;counter++)
		{
			GPIO_PORTF_DATA_R = 0x04;     //turn on green led       
			delay(125*start);              //wait 125ms*0.2=25ms    
			GPIO_PORTF_DATA_R = 0x00;			//turn off my green led				 
			delay(125*stop);							//wait 125ms*(1-start)	
			start+=0.2;
			stop-=0.2;
			if(counter==4)
				
			
			{
				start=1;
				stop=0;
				
			}
		}
			
		
		 for(counter=0;counter<5;counter++)
			{
				
				
				GPIO_PORTF_DATA_R = 0x04;    		 //turn on green led           
				delay(125*start);               //wait 125ms*0.2=25ms    
				GPIO_PORTF_DATA_R = 0x00;				//turn off my red led					 
				delay(125*stop);								//wait 125ms*(1-start)=100ms	
				start-=0.2;
				stop+=0.2;
				
			if(counter==4)
			{
				start=0;
				stop=1;		
			}
		}	
  }
	return 0;
}




void delay (int time )
{	int x=1 , y=1 ;
	for(x=1 ; x<=time ; x++) 
	
	{ 
		for(y=1 ; y<=9000 ; y++){}
	}
	
}

