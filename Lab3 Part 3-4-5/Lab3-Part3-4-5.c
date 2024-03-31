#include<stdint.h>
#include<stdlib.h>


#define SYSCTL_RCGC2_R				  (*((volatile unsigned long *)0x400FE108)) 
#define SYSCTL_RCGC2_GPIOF			0x00000020 	 // Port F Clock Gating Control
#define SYSCTL_PRGPIO_R         (*((volatile unsigned long *)0x400FEA08)) 
#define GPIO_PORTF_LOCK_R     	(*((volatile unsigned long *)0x40025520)) 
#define GPIO_PORTF_CR_R         (*((volatile unsigned long *)0x40025524)) 
#define GPIO_PORTF_DIR_R				(*((volatile unsigned long *)0x40025400))  
#define GPIO_PORTF_PUR_R        (*((volatile unsigned long *)0x40025510)) 
#define GPIO_PORTF_DEN_R				(*((volatile unsigned long *)0x4002551C)) 
#define GPIO_PORTF_DATA_R				(*((volatile unsigned long *)0x400253FC)) 
#define sw1                 		GPIO_PORTF_DATA_R & 0x10





void delay (int ms);


int main(void){
		SYSCTL_RCGC2_R = 0x20;										//Activating clock of port F
		while((SYSCTL_PRGPIO_R & 0x00000020)==0){}	//Waiting until the peripheral is ready
		GPIO_PORTF_LOCK_R=0x4C4F434B; ///unlock Portf register
		GPIO_PORTF_CR_R=0xFF; ///set up my register
		GPIO_PORTF_DIR_R=0xEE; //define input and output
		GPIO_PORTF_DEN_R=0xFF; // I do my available  input and output.
		GPIO_PORTF_PUR_R=0x10; // Activate my pull up switch.	
		GPIO_PORTF_DATA_R=00; //Clear my output.	
		
		float run=0.2;
		float close=0.8;
		int count=0;		
		while(1)
			{
				
				
				if((sw1)==0x10) ///if my switch off....
				{		
					for(count=0;count<5;count++)
					{
					GPIO_PORTF_DATA_R = 0x02;     //Turn on my red led.           
					delay(125*run);                               //Wait 125ms*run    
					GPIO_PORTF_DATA_R = 0x00;	//Turn off my red led.			
					delay(125*close);		//Wait 125ms*close	
					run+=0.2;			//Run increases  %20.
					close-=0.2;			//Close decreases %20.	
						if(count==4)
						{
							run=1;			//If  we are end.My run=1.
							close=0;		//If  we are end.My close=0.
					}
			}
				for(count=0;count<5;count++)
				{
					GPIO_PORTF_DATA_R = 0x02;     //Turn on my red led.           
					delay(125*run);             		  //Wait 125ms*run
					GPIO_PORTF_DATA_R = 0x00;	//Turn off my red led.				
					delay(125*close);		//Wait 125ms*close	
					run-=0.2;			//Run decreases  %20.
					close+=0.2;			//Close increases %20.		
						if(count==4)
						{
							run=0;				//If  we are end.My run=0.
							close=1;			//If  we are end.My close=1.
						}
				}
			}
			if((sw1)==0)	///if my switch on .....
			{
				run=0.2;
				close=0.8;
				GPIO_PORTF_DATA_R = 0x02;    	 //Turn on my red led.           
				delay(125*run);             			  //Wait 125ms*0.2=25ms    
				GPIO_PORTF_DATA_R = 0x00;		//Turn off my red led.				
				delay(125*close);			//Wait 125ms*0.8=100ms.

				
				
			}
	}
	return 0;

}
//I define the function to calculate 1 ms.
void delay (int ms )
{	int c=1 , d=1 ;
	for(c=1 ; c<=ms ; c++) 
	{ 
		for(d=1 ; d<=8000 ; d++){}
	}
}	

