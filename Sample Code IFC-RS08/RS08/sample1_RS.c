//=====================================================================================
//
//	Author				:Cytron Technologies		
//	Project				:Interface Free Controller-RC Servo Card (IFC-RS08)
//	Project description	:Sample1 for IFC-RS08
//						 8 servos will rotate continues left to right and vice versa with servo position reporting command 
//  Date				: 19 March 2009
//
//=====================================================================================

//=====================================================================================
//	include h file for cards
//	User has to include h file for all the cards used 
//=====================================================================================
#include<p18f2685.h>
#include "iic.h"
#include "iic_rs.h"
#include "iic_cp.h"
//=====================================================================================
//	configuration bit, User is advised not to change the setting here
//=====================================================================================
#pragma config OSC = HSPLL
#pragma config FCMEN = OFF 
#pragma config IESO = OFF 
#pragma config PWRT = OFF 
#pragma config BOREN = OFF 
#pragma config WDT = OFF 
#pragma config MCLRE = ON
#pragma config PBADEN = OFF 
#pragma config DEBUG = OFF
#pragma config XINST = OFF
#pragma config LVP = OFF 

//=====================================================================================
//	define input or ouput of board here
//	For example: 
//				1.led1 was defined for first LED on MB00
//				2.buzzer was defined for buzzer on MB00
//	The address for input/output devide on board is fixed, hence user is 
//	advised not to change the defined address for input/output 
//=====================================================================================

#define	led1		LATBbits.LATB0 	// led1 defined as output for first LED on MB00	
#define	led2		LATBbits.LATB1	// led2 defined as output for second LED on MB00
#define	led3		LATBbits.LATB2	// led3 defined as output for third LED on MB00
#define	led4		LATBbits.LATB3	// led4 defined as output for forth LED on MB00
#define	led5		LATBbits.LATB4	// led5 defined as output for fifth LED on MB00
#define	led6		LATBbits.LATB5	// led6 defined as output for sixth LED on MB00
#define	led7		LATBbits.LATB6	// led7 defined as output for seventh LED on MB00
#define	led8		LATBbits.LATB7	// led8 defined as output for eighth LED on MB00
#define	led			LATB			// led defined as output for ALL LED on MB00

#define	sw1			PORTAbits.RA2	// sw1 defined as input for SW1 on MB00
#define	sw2			PORTAbits.RA1	// sw2 defined as input for SW2 on MB00
#define	sw3			PORTAbits.RA0	// sw3 defined as input for SW3 on MB00
#define	buzzer		LATCbits.LATC5	// buzzer defined as output for buzzer on MB00
#define busy_led	LATCbits.LATC1	// busy_led defined as output for Busy LED on MB00
#define error_led	LATCbits.LATC2	// error_led defined as output for Error LED on MB00
#define slave_reset	LATCbits.LATC0	// slave_reset define as output to reset slave cards

//=====================================================================================
//	card address
//	User can define the address of slave card here
//=====================================================================================
#define add_rs 		0b100001	// Please make sure the communication address on 
								// IFC-RS08 is compatible with the address 

//=====================================================================================
//	function prototype
//	User needs to include all the function prototype here
//	The first 4 functions prototype are fixed, user is also advised not to
//	change it.
//=====================================================================================
void ISRHigh(void);
void ISRLow(void);
void delay(unsigned long data);
void init(void);
	
//=====================================================================================
//	Main Function
//	This is the main function where program start to execute 
//=====================================================================================
void main(void)
{
	unsigned int pos,position_a,speed_a,speed_b;
	//--------------------------------------------------------------------------------
	//	User is advised not to change or remove the initialization function called 
	// 	call initialization function of the setting in program
	init();						
	//--------------------------------------------------------------------------------
	
	//--------------------------------------------------------------------------------
	//	Program start
	//	User can start to write the program here
	//--------------------------------------------------------------------------------

					
	position_a=4500;  	//position value 

	speed_a=100;  		//define speed
	speed_b=20;			//define speed 
	
	while(1)
	{	
		//before sw1 is pressed, servo motors is at the loose condition
		//servo motors will not run unless is enabled
		if(sw1==0)
		{
			rs_en(add_rs,0,1);			//enable all servo motors
									
		}
		
		else if(sw2==0)					//servo will only rotate after the sw1 at Main Board is pressed
		{
	
			while(sw2==0);							
			while(1)									//infinity loop
			{											//servo motors will not run if sw1 is not press before this

				rs_pos_sp(add_rs,0,position_a,speed_a); //drive all the RC servo to position a with speed rate of speed_a
				pos=rs_read_pos(add_rs,8);		   		//get feedback for 8th servo position
	
				while(pos!=position_a)					//test the 8th servo position 
				{
					pos=rs_read_pos(add_rs,8);			//keep reading if that servo no yet reach 4500steps
				}										
														//if the servo reach that position, than continue to give new position

				rs_pos_sp(add_rs,1,2500,speed_b);		//servos rotate to different steps with speed rate of speed_b	     
				rs_pos_sp(add_rs,2,1000,speed_b);
				rs_pos_sp(add_rs,3,2000,speed_b);
				rs_pos_sp(add_rs,4,1250,speed_b);
				rs_pos_sp(add_rs,5,2250,speed_b);
				rs_pos_sp(add_rs,6,800,speed_b);
				rs_pos_sp(add_rs,7,750,speed_b);
				rs_pos_sp(add_rs,8,500,speed_b);
			
				pos=rs_read_pos(add_rs,8);		   		//get feedback for 8th servo position
	
				while(pos!=500)							//test the 8th servo position 
				{
					pos=rs_read_pos(add_rs,8);			//keep reading if that servo no yet reach 4500steps
				}										
														//if the servo reach that position, than continue to give new position

			}
		}
	}	//while loop
	
}		//main

//=====================================================================================
//	functions
//  User can write all the necessary function here
//=====================================================================================
void delay(unsigned long data)
{
	// this is a delay function for user to use when the program need a delay
	// this function can be call by type : delay(xxxx),
	// user can replace the 'xxxx' with a value to determine how long the program
	// should delay for, the bigger the value, the longer the time of the delay
	for( ;data>0;data-=1);
}


void init(void)
{
	// this is a initialization for set the input and output of PIC18F2685
	// User is advised NOT to change the setting here
	// tris
	TRISB=0x00;				// led as output
	TRISA=0xff;				// push button as input
	TRISC=0x00;				// set port c as output
	
	// the setting for analog to digital converter, ADC for PIC18F2685
	// User is advised NOT to change the setting here
	// adc
	ADCON1=0x0F;
	
	//call the initialization function of I2C
	iic_init();		
	
	//	set the initial condition of output device on board
	//	User is advised not to change the initial condition
	led=0x00;				// turn OFF all LED on board by sending value '0x00'
	buzzer=0;				// turn OFF buzzer on board by sending value '0'
	error_led=0;			// turn OFF Error LED on board by sending value '0'
	busy_led=1;				// turn ON Busy LED on board by sending value '1'
	slave_reset=1;			// reset slave cards
	delay(10000);			
	slave_reset=0;			
	delay(50000);			
	busy_led=0;				// turn OFF Busy LED on board by sending value '0'
}


//=====================================================================================
//	Interrupt vector
//=====================================================================================
#pragma	code InterruptVectorHigh = 0x08
void InterruptVectorHigh(void)
{
 	_asm
		goto ISRHigh		// jump to interrupt routine
	_endasm
}
#pragma code
#pragma	code InterruptVectorLow = 0x18
void InterruptVectorLow(void)
{
 	_asm
		goto ISRLow			// jump to interrupt routine
	_endasm
}
#pragma code

//=====================================================================================
//	Interupt Service Routine
//	this a function reserved for interrupt service routine
//	User may need it in advance development of the program
//=====================================================================================
#pragma interrupt ISRHigh
void ISRHigh(void)
{

}

#pragma interrupt ISRLow
void ISRLow(void)
{

}

