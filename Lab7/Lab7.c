#include "TM4C123GH6PM.h"
#include <stdio.h>



		#define ADC0_ACTSS_R            (*((volatile uint32_t *)0x40038000))
		#define ADC0_EMUX_R             (*((volatile uint32_t *)0x40038014))
		#define ADC0_SSMUX3_R           (*((volatile uint32_t *)0x400380A0))
		#define ADC0_SSCTL3_R           (*((volatile uint32_t *)0x400380A4))
		#define ADC0_PSSI_R             (*((volatile uint32_t *)0x40038028))
		#define ADC0_RIS_R              (*((volatile uint32_t *)0x40038004))
		#define ADC0_SSFIFO3_R          (*((volatile uint32_t *)0x400380A8))
		#define ADC0_ISC_R              (*((volatile uint32_t *)0x4003800C))
	

		#define SYSCTL_RCGCGPIO_R       (*((volatile uint32_t *)0x400FE608))
		#define SYSCTL_RCGCADC_R        (*((volatile uint32_t *)0x400FE638))
		#define GPIO_PORTE_AFSEL_R      (*((volatile uint32_t *)0x40024420))
		#define GPIO_PORTE_DEN_R        (*((volatile uint32_t *)0x4002451C))
		#define GPIO_PORTE_AMSEL_R      (*((volatile uint32_t *)0x40024528))
		#define GPIO_PORTE_PCTL_R       (*((volatile uint32_t *)0x4002452C))

		
		#define GPIO_PORTF_DATA_R     	(*((volatile unsigned long *)0x400253FC))
		#define GPIO_PORTF_DIR_R   	    (*((volatile unsigned long *)0x40025400))
		#define GPIO_PORTF_DEN_R   	    (*((volatile unsigned long *)0x4002551C))
		#define GPIO_PORTF_AFSEL_R    	(*((volatile unsigned long *)0x40025420))
		#define GPIO_PORTF_PCTL_R       (*((volatile unsigned long *)0x4002552C))
		#define GPIO_PORTF_AMSEL_R   		(*((volatile unsigned long *)0x40025528))		
		#define SYSCTL_RCGCUART_R       (*((volatile uint32_t *)0x400FE618))
		
		
		#define UART5_CTL_R             (*((volatile uint32_t *)0x40011030))
		#define UART5_IBRD_R            (*((volatile uint32_t *)0x40011024))
		#define UART5_FBRD_R            (*((volatile uint32_t *)0x40011028))
		#define UART5_CC_R              (*((volatile uint32_t *)0x40011FC8))
		#define UART5_LCRH_R            (*((volatile uint32_t *)0x4001102C))
		#define UART5_FR_R              (*((volatile uint32_t *)0x40011018)) 
		#define UART5_DR_R              (*((volatile uint32_t *)0x40011000))
		
		
		#define UART0_CTL_R             (*((volatile unsigned long *)0x4000C030))
		#define UART0_IBRD_R            (*((volatile unsigned long *)0x4000C024))
		#define UART0_FBRD_R            (*((volatile unsigned long *)0x4000C028))
		#define UART0_CC_R              (*((volatile unsigned long *)0x4000CFC8))
		#define UART0_LCRH_R            (*((volatile unsigned long *)0x4000C02C))
		#define UART0_FR_R              (*((volatile unsigned long *)0x4000C018))	
		#define UART0_DR_R              (*((volatile unsigned long *)0x4000C000))

		void ADC_Init(void);
		void ADC_Read(void);
		void delay_ms(int n);
		void printChar(char c); 
    void UART5_TX(char c);
		void UART5_init(void);
		void UART5_puts(char* str);
		void UART0_init(void);
    void printString(char *str);

    char Read_Char(void);
		char mesg[1];

		unsigned int adc_value;
		signed int temperature;
    signed int Avrg=0;

		int i;
    int cnt;
		int a;
		int Total_Avrg[30];
		int p_indx;                     //index
		float Total=0;
		char entr;




int main(void)
{
    
    ADC_Init();
		UART5_init();
	  UART0_init();
    while(1)
    {
        while(1){
				if(cnt>=29) break;	
				
					
			  for(i=0;i<30;i++){GPIO_PORTF_DATA_R=0x0F;	
					ADC_Read();
					
					
					Total_Avrg[i]=((ADC0->SSFIFO3 * 330.00) / 4096.00);	
					Total=Total+Total_Avrg[i];	
					
					cnt=cnt+1;	
					delay_ms(1000);}
					
				  GPIO_PORTF_DATA_R=0x00;}
					Avrg=Total/30;
					entr = Read_Char();
				
					
					if(entr==0x0D)          //'enter' hex form
					{
						sprintf(mesg, "\r\nLAST 30 SECOND : = %d C",Avrg);
					UART5_puts(mesg);
					}
					cnt=cnt+1;
					p_indx=cnt%30;
					
					ADC_Read();
					temperature=((ADC0->SSFIFO3 * 330.00) / 4096.00);	 
							
					Total=Total-Total_Avrg[p_indx];	
					Total_Avrg[p_indx]=temperature; 
					Total=Total+Total_Avrg[p_indx];
					
					
					Avrg=Total/30;				 
					delay_ms(1000); 
					
					
				if(Total_Avrg[p_indx]<25)                //'A-H-M-E' colour rank
        {GPIO_PORTF_DATA_R=0x04;}                 // BLUE
				
				else if((Total_Avrg[p_indx]<26)&&(Total_Avrg[p_indx]>=25)) 
        {GPIO_PORTF_DATA_R=0x0A;}                 // YELLOW
				
				else if((Total_Avrg[p_indx]<27)&&(Total_Avrg[p_indx]>=26)) 
        {GPIO_PORTF_DATA_R=0x02;}               	// RED
				
			  else if(Total_Avrg[p_indx]>=27) 
        {GPIO_PORTF_DATA_R=0x08;}                 //GREEN
		}
}


void ADC_Init(){
    SYSCTL->RCGCGPIO |= (1<<4);   
    SYSCTL->RCGCADC |= (1<<0);   


    GPIOE->AFSEL |= (1<<3);       
    GPIOE->DEN &= ~(1<<3); 
	
    ADC0_ACTSS_R &= ~(1<<3);       
    ADC0_EMUX_R &= ~0xF000;   
    ADC0_SSMUX3_R = 0;        
    ADC0_SSCTL3_R |= (1<<1)|(1<<2);        
    ADC0_ACTSS_R |= (1<<3);        

      

   SYSCTL->RCGCGPIO |= 0x20; 
   GPIO_PORTF_DIR_R       |= 0x1E; 
   GPIO_PORTF_DEN_R        |= 0x1F; 

}
void ADC_Read(){

        ADC0_PSSI_R |= (1<<3);       
        while((ADC0_RIS_R & 8) == 0) ;  
        adc_value = ADC0_SSFIFO3_R; 
        ADC0_ISC_R = 8;          
            


}
void UART5_init(void)
{
    SYSCTL_RCGCUART_R |= 0x20;         
    SYSCTL_RCGCGPIO_R |= 0x10;      
 
    UART5_CTL_R = 0;                
    UART5_IBRD_R = 104;             
    UART5_FBRD_R = 11;              
    UART5_CC_R = 0;                 
    UART5_LCRH_R = 0x60;            
    UART5_CTL_R = 0x301;            

    GPIO_PORTE_DEN_R = 0x30;        
    GPIO_PORTE_AFSEL_R = 0x30;      
		GPIO_PORTE_AMSEL_R = 0;
    GPIO_PORTE_PCTL_R = 0x00110000;      
}
 
void UART0_init(void)
{
    SYSCTL_RCGCUART_R |= 1; 
	/* provide clock to UART0 */
    SYSCTL_RCGCGPIO_R |= 1; 
	/* enable clock to PORTA */
    delay_ms(20); 
	/* wait for output line to stabilize */
   



		/* UART0 initialization */ 
    UART0_CTL_R = 0; 
		/* disable UART0 */
    UART0_IBRD_R =  104; 
		/* 16MHz/16=1MHz, 1MHz/104=9600 baud rate */
    UART0_FBRD_R =  11;
		/* fraction part, see Example 4-4 */
    UART0_CC_R = 0;
		/* use system clock */
    UART0_LCRH_R = 0x60; 
		/* 8-bit, no parity, 1-stop bit, no FIFO */
    UART0_CTL_R = 0x301; 
		/* enable UART0, TXE, RXE */
		/* UART0 TX0 and RX0 use PA0 and PA1. Set them up. */

    GPIOA->DEN = 0x03; 
		/* Make PA0 and PA1 as digital */
    GPIOA->AFSEL = 0x03; 
		/* Use PA0,PA1 alternate function */
    GPIOA->PCTL = 0x11;    
}




void UART5_TX(char a)
{
    while((UART5_FR_R & 0x20) != 0); 
    UART5_DR_R = a;                 
}
 
void UART5_puts(char* string)
{
    while (*string != 0)                 
        UART5_TX(*string++);              
}
void delay_ms(int n)
{
    int32_t i, y;
    for(i = 0 ; i < n; i++)
        for(y = 0; y < 3180; y++)
            {;}  
}
char Read_Char(void)  
{
    char a;
    while((UART0_FR_R & (1<<4)) != 0){}
      a = UART0_DR_R;                 
    return a;                    
}

void printChar(char a)  
{
  while((UART0_FR_R & (1<<5)) != 0){
    UART0_DR_R = a;           }
}
void printString(char * string)
{
  while(*string!='\n')
  {
    printChar(*(string++));
  }
}
