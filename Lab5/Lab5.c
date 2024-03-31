#include <stdio.h>
#include <stdint.h>


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

void PLL_Init(void);
void ADC0_GPIO_Init(void);
unsigned long ADC0_In(void);
unsigned long value;


int main(void)
{	PLL_Init();
	ADC0_GPIO_Init();
  
  SYSCTL_RCGC2_R	|=(1<<5);
	while((SYSCTL_PRGPIO_R&0x00000020)==0){} 
  GPIO_PORTF_DIR_R |=0X04;
	GPIO_PORTF_AFSEL_R &= ~0x04;
	GPIO_PORTF_DEN_R |= 0X04;
		
	GPIO_PORTF_DIR_R |=0X02;
	GPIO_PORTF_AFSEL_R &= ~0x02;
	GPIO_PORTF_DEN_R |= 0X02;
	GPIO_PORTF_DIR_R |=0X0A;
	GPIO_PORTF_AFSEL_R &= ~0x0A;
	GPIO_PORTF_DEN_R |= 0X0A;
		
	GPIO_PORTF_DIR_R |=0X08;
	GPIO_PORTF_AFSEL_R &= ~0x08;
	GPIO_PORTF_DEN_R |= 0X08;
	
GPIO_PORTF_PCTL_R = (GPIO_PORTF_PCTL_R&0XFFFFF0FF)+0X00000000;
GPIO_PORTF_AMSEL_R =0;
while(1){
	
	value = ADC0_In();	 	
	if(value >=0){
			
		GPIO_PORTF_DATA_R = 0x0A;
	}
		if(value>=1024){ GPIO_PORTF_DATA_R = 0x04;}
		if(value>=2048){ GPIO_PORTF_DATA_R = 0x08;}
		if(value>=3050){ GPIO_PORTF_DATA_R = 0x02;}		
		else GPIO_PORTF_DATA_R = 0x00;
} }
void ADC0_GPIO_Init(void){
	
  SYSCTL_RCGCADC_R |= 0x0001;  
  SYSCTL_RCGCGPIO_R |= 0x10;    
  while((SYSCTL_PRGPIO_R&0x10) != 0x10){};  
  GPIO_PORTE_DIR_R &= ~0x08;    //  make PE3 input
  GPIO_PORTE_AFSEL_R |= 0x08;   
  GPIO_PORTE_DEN_R &= ~0x08;    
  GPIO_PORTE_AMSEL_R |= 0x08;   

		
  ADC0_PC_R &= ~0xF;                
  ADC0_PC_R |= 0x1;                     // configure for 125K samples/sec
  ADC0_SSPRI_R = 0x0123;                                      
  ADC0_ACTSS_R &= ~0x0008;    // disable sample sequencer 3, prevents erroneous execution.
                                                                                                           
//   if a trigger event were to occur during the configuration process.(pot ayarlari)
 

	ADC0_EMUX_R &= ~0xF000;         //  seq3 is software trigger
  ADC0_SSMUX3_R &= ~0x000F;  
  ADC0_SSMUX3_R += 0;                   // set channel 0,which is PE3
  ADC0_SSCTL3_R = 0x0006;             //  not TS0 D0, clear because we do not wish to use differential  mode.
  ADC0_IM_R &= ~0x0008;         // disable SS3 interrupts
  ADC0_ACTSS_R |= 0x0008;       //  enable sample sequencer 3
}
unsigned long ADC0_In(void){  

unsigned long result;

  ADC0_PSSI_R = 0x0008;       //  initiate SS3,( to initiate a conversion on sequencer 3.)
  while((ADC0_RIS_R&0x08)==0){};   // wait for conversion done
  result = ADC0_SSFIFO3_R&0xFFF;  
  ADC0_ISC_R = 0x0008;             
  return result;
}

void PLL_Init(void){

  //  active RCC2

  SYSCTL_RCC2_R |=  0x80000000;  // USERCC2

  //  bypass PLL while initializing

  SYSCTL_RCC2_R |=  0x00000800;  // BYPASS2, PLL bypass

  //  select the crystal value and oscillator source

  SYSCTL_RCC_R = (SYSCTL_RCC_R &~0x000007C0) + 0x00000540;   // clear XTAL field, bits 10-6

                     // 10101, configure for 16 MHz crystal

  SYSCTL_RCC2_R &= ~0x00000070;  // configure for main oscillator source

  // activate PLL by clearing PWRDN

  SYSCTL_RCC2_R &= ~0x00002000;

  // set the desired system divider

  SYSCTL_RCC2_R |= 0x40000000;   // use 400 MHz PLL

  SYSCTL_RCC2_R = (SYSCTL_RCC2_R&~ 0x1FC00000)  // clear system clock divider
                  + (9<<22);      // configure for 40 MHz clock
  // wait for the PLL to lock by polling PLLLRIS

  while((SYSCTL_RIS_R & 0x00000040)==0){};  // wait for PLLRIS bit
  // enable use of PLL by clearing BYPASS

  SYSCTL_RCC2_R &= ~0x00000800;
 }
