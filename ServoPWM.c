#define F_CPU 1000000UL
#define LED PD6
#include "ServoPWM.h"
#include <util/delay.h>
// Basic macros to make simple functions more readable
#define output_low(port, pin) port &= ~(1<<pin)
#define output_high(port, pin) port |= (1<<pin)
#define set_input(portdir, pin) portdir &= ~(1<<pin)
#define set_output(portdir, pin) portdir |= (1<<pin)

int main(void){
	setupPWM();
	int delta;
	double ThetaPan, ThetaTilt;
	ThetaPan = 90;
	ThetaTilt = 90;
	while(1){
		_delay_ms(250);
		output_high(PORTD, LED);
		if(ThetaPan < 179){
			delta = 5;
		}
		else{
			delta = -5;		
		}
		ThetaPan = changeAngle(ThetaPan, delta);
		ThetaTilt = changeAngle(ThetaTilt, delta);
		DutyCyclePan = updateDutyCycle(ThetaPan);
		DutyCycleTilt = updateDutyCycle(ThetaTilt);
		OCR1A = setPWM_Register(TOP, DutyCyclePan);// Pan: increment theta, calcualte Duty Cycle, set PWM register accordingly
		OCR1B = setPWM_Register(TOP, DutyCycleTilt);// Tilt: increment theta, calcualte Duty Cycle, set PWM register accordingly
		_delay_ms(250);// Give the motor time to move
		OCR1A = stopPWM(TOP);//Set the Pulse Width to Zero to stop sendong signals (removes servo "jitters")
		OCR1B = stopPWM(TOP);// Any value above TOP will produce duty cycle of 0, ending transmission
		output_low(PORTD, LED);
	}
	return 0;
}
