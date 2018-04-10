#include <avr/io.h>

// Define Global Variables
uint16_t TOP = 10000;//62500;// This ultimatley sets the frequency (50Hz)  Note: uint16_t overflows above 65,535
double ThetaPan = 0;// (degrees) Angle for Pan Servo
double DutyCyclePan = 5;// (%) Sclaes the Duty Cycle, DutyCycle = 1/DutyCycleRecip, (1 -> allways on)
double ThetaTilt = 0;// (degrees) Angle for Pan Servo
double DutyCycleTilt = 5;// (%) Sclaes the Duty Cycle, DutyCycle = 1/DutyCycleRecip, (1 -> allways on)

void setupPWM(void){
	// This is Governing Equation for Frequency of PWM. Read this to understand!!!!!
	/* f_pwm = f_cpu/(2*N*TOP), where: TOP = OCR1A, N = prescaller */
	
	/* Prescaller is set by CS12, CS11, CS10 registers */
	/* Examples: 0,0,0 => no prescaler clock stopped; 0,0,1 => 1/1 clock will run;  */
	/* 0,1,0 => 1/8; 0,1,1 => 1/64; 1,0,0 => 1/256; 1,0,1 => 1/1024; 1,1,0 => external timer connected (not oscilator) */

	// initialize the direction of PORTB1 to be an output
	DDRB = (1<<PORTB1) | (1<<PORTB2);// PB1:2 as output for OC1A

	TCCR1A = (1<<COM1A0) | (1<<COM1A1) | (1<<COM1B0) | (1<<COM1B1);// Timer Counter Control Register A
	TCCR1B = (1<<WGM13);// WGM - Wave Generation Mode

	ICR1 = TOP;// Provides scalar for PWM Frequency !NOT DutyCycle!!
	

	if(F_CPU == 1000000){
		TCCR1B |= (1 << CS10);// Sets prescalar, currently to 1/1, also will start timer
	}
	else if(F_CPU == 8000000){
		TCCR1B |= (1 << CS11);// Sets prescalar, currently to 1/8, also will start timer
	}
	else if(F_CPU == 16000000){
		TCCR1B |= (1 << CS10) | (1 << CS11);// Sets prescalar, currently to 1/64, also will start timer
		// !! Note !! You will also need to change the "TOP" to 2500	
	}
}


double changeAngle(double Theta, double DeltaTheta){
	Theta += DeltaTheta;
	if(Theta > 179){
		Theta = 179;		
	}
	else if(Theta < 0){
		Theta = 0;	
	}
	return Theta;
}

double updateDutyCycle(double Theta){
	double DutyCycle = 5 + 5*Theta/180;
	return DutyCycle;
}

uint16_t setPWM_Register(uint16_t maxCount, double dutyCycle){	
	uint16_t RegisterValue = (uint16_t) maxCount - maxCount/100*dutyCycle;// Sets the Pulse Width
	return RegisterValue;	
}

uint16_t stopPWM(uint16_t Top){
		return Top + 1;
}

