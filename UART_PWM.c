#define F_CPU 1000000UL
#define BAUDRATE 4800
#define LED PD6
#include "ServoPWM.h"
#include "BasicUART.h"
#include <util/delay.h>

// These basic macros are used 
#define output_low(port, pin) port &= ~(1<<pin)
#define output_high(port, pin) port |= (1<<pin)
#define set_input(portdir, pin) portdir &= ~(1<<pin)
#define set_output(portdir, pin) portdir |= (1<<pin)

char inString[32];// used to store incoming string
double deltaThetaPan;// used to hold the change in pan rotation angle
double deltaThetaTilt;// used to hold the change in tilt rotation angle

// Format will be like this: r010d005   Right 10 (deg), Up 5 (deg)
double parseDeltaThetaPan(char inString[9]){
	int deltaThetaLocal = 0;// (0-179)
	deltaThetaLocal += 100*(inString[1] - 48);// ASCII to int, in 100's place
	deltaThetaLocal += 10*(inString[2] - 48);// ASCII to int, in 10's place		
	deltaThetaLocal += (inString[3] - 48);// ASCII to int, in 1's place
	return (double) deltaThetaLocal;// need as a double for later
}

double parseDeltaThetaTilt(char inString[9]){
	int deltaThetaLocal = 0;// (0-179)
	deltaThetaLocal += 100*(inString[5] - 48);// ASCII to int, in 100's place
	deltaThetaLocal += 10*(inString[6] - 48);// ASCII to int, in 10's place		
	deltaThetaLocal += (inString[7] - 48);// ASCII to int, in 1's place
	return (double) deltaThetaLocal;// need as a double for later
}

int main(void){
	USART_init();// Initialize UART
	setupPWM();// Initialize PWM
	int ThetaPan, ThetaTilt;
	ThetaPan = 90;
	ThetaTilt = 90;
	while(1)
	{
		//_delay_ms(100);// Wait for 1 (s)
		output_high(PORTD, LED);// Indicates that were waiting for a string
		USART_getstring(inString);// Wait for a command
		output_low(PORTD, LED);// Indicates that we have received a string

		// read the numeric chars and convert to double
		deltaThetaPan = parseDeltaThetaPan(inString);
		deltaThetaTilt = parseDeltaThetaTilt(inString);
		

		// Determine which command was sent
		// Pan
		switch(inString[0]){
			case 'l':// pan right
				ThetaPan = changeAngle(ThetaPan, deltaThetaPan);
				DutyCyclePan = updateDutyCycle(ThetaPan);
				OCR1A = setPWM_Register(TOP, DutyCyclePan);// Pan: increment theta, calcualte Duty Cycle, set PWM register accordingly
				break;
			case 'r':// pan left
				ThetaPan = changeAngle(ThetaPan, -deltaThetaPan);
				DutyCyclePan = updateDutyCycle(ThetaPan);
				OCR1A = setPWM_Register(TOP, DutyCyclePan);// Pan: increment theta, calcualte Duty Cycle, set PWM register accordingly
				break;
		}
		// Tilt
		switch(inString[4]){
			case 'u':// tilt up
				ThetaTilt = changeAngle(ThetaTilt, deltaThetaTilt);
				DutyCycleTilt = updateDutyCycle(ThetaTilt);
				OCR1B = setPWM_Register(TOP, DutyCycleTilt);// Tilt: increment theta, calcualte Duty Cycle, set PWM register accordingly
				break;
			case 'd':// tilt down		
				ThetaTilt = changeAngle(ThetaTilt, -deltaThetaTilt);
				DutyCycleTilt = updateDutyCycle(ThetaTilt);
				OCR1B = setPWM_Register(TOP, DutyCycleTilt);// Tilt: increment theta, calcualte Duty Cycle, set PWM register accordingly
				break;
		}

		_delay_ms(80);// Give the motor time to move
		
		OCR1A = stopPWM(TOP);//Set the Pulse Width to Zero to stop sendong signals (removes servo "jitters")
		OCR1B = stopPWM(TOP);//Set the Pulse Width to Zero to stop sendong signals (removes servo "jitters")
	}
	return 0;	
}
