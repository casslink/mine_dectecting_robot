/*
 * motorTest.c
 *
 * Created: 4/1/2019 12:16:20 PM
 * Author : clink1 and H.L.Li
 */ 

#define F_CPU 16000000ul
#include <avr/io.h>
#include <stdint.h>
#include <stdbool.h>
#include <avr/interrupt.h>
#include <util/delay.h>
#define FORWARD 0xAA
#define BACKWARDS 0x55
#define BREAK 0xFF
#define LEFT 0x66
#define RIGHT 0x99

//void call_left_sensor();
//void call_front_sensor();
//void call_right_sensor();
void call_sensor();
//void check_sensor();
void straight();
void backward();
void stop();
void right_turn();
void left_turn();
void output_byte(uint8_t byte);
void update_IO();

uint8_t shift_reg;
uint16_t distance[3];
uint8_t left_sensor, front_sensor, right_sensor;

int main(void)
{
	DDRD |= ( (1<<6) | (1<<4) | (1<<1) | (1<<0) ); //config as output for LEDS in portD
	DDRB |= (1<<7);
	
	DDRB &= ~( (1<<3) ); //config btns
	PORTB &= ~( (1<<3)); //disable pull up
	
	
	DDRB |= ( (1<<2) | (1<<1) | (1<<0) );
	
	//configure switches
	DDRD |= ( (1<<2) ); //config as output for SW in portB (trig)

	DDRB &= ~( (1<<5) | (1<<4)); //config as input for SW in portD (echo)
	PORTB &= ~( (1<<5) | (1<<4) ); //disable pull up portD
	DDRD &= ~(1<<7);
	PORTD &= ~(1<<7);
	
	TCCR0A = 0b10000011; //initialization of timer 0 control register A, normal pwm polarity and mode for both channels
	TCCR0B = 0b00000100; //initialization of timer 0 control register B, prescaler set to 256

//	uint8_t right_flag = 0;
    /* Replace with your application code */
	OCR0A = 104;
	PORTD |= (1<<0);
//	uint8_t direct;
	_delay_ms(200);
    while (1) {	
		call_sensor();
		_delay_ms(25);
//		check_sensor();
//		uint8_t delayIndex;			
		
//		direct = (left_sensor<<2) | (front_sensor<<1) | (right_sensor); 
		if (distance[2] < 153) {
			PORTD |= (1<<1);
			straight();
		}
		else if (distance[1] < 153) {
			left_turn();
			_delay_ms(200); //delay for 90 degree turn
			_delay_ms(200);
			_delay_ms(200);
		}
		else if (distance[0] > 153)  {
			right_turn();
			_delay_ms(200); //delay for 90 degree turn
			_delay_ms(200);
			_delay_ms(200);
		}
		else
			straight();
/*		else if (direct == 7) {
			left_turn();
			_delay_ms(200); //delay for 180 degree turn
			_delay_ms(200);
			_delay_ms(200);
			_delay_ms(200); 
			_delay_ms(200);
			_delay_ms(200);
		} */
/*		else {
			straight();
		} */
/*
		switch(direct) {
			case 0:
				right_turn();
				if (right_flag == 0) {
					for (delayIndex = 0; delayIndex < 4; ++delayIndex)
						_delay_ms(200);
					_delay_ms(50);                                                                                                    
					right_flag = 1;
				}
				else {
					for (delayIndex = 0; delayIndex < 6; ++delayIndex)
					_delay_ms(200);				
				}
				break;
			case 1:
				straight();
				break;
			case 2:
				left_turn();
				for (delayIndex = 0; delayIndex < 6; ++delayIndex)
					_delay_ms(200);
				_delay_ms(100);
				break;
			case 3:
				left_turn();
				for (delayIndex = 0; delayIndex < 6; ++delayIndex)
					_delay_ms(200);
//				_delay_ms(100);
				//straight();
				break;
			case 5:
				straight();
				//left_turn();
				break;
			case 7:
				left_turn();
				for (delayIndex = 0; delayIndex < 6; ++delayIndex)
					_delay_ms(200);
				_delay_ms(100);
				break;
			default:
				straight();
				break;
		}
*/	}

	return 0;
} 

/*	attempt 3
		if(left_sensor == 1) {
			PORTD |= (1<<6);
			straight();
		}
		else {
			PORTD &= ~(1<<6);
		}
		if(right_sensor == 1) {
			PORTD |= (1<<4);
			backward();
		}
		else {
			PORTD &= ~(1<<4);
		}
		if(front_sensor == 1) {
			left_turn();
		}
	
		call_left_sensor();
		_delay_ms(50);
		call_front_sensor();
		_delay_ms(50);
		call_right_sensor();
		_delay_ms(50); 
		check_sensor();*/
/*	//attempt 1
	sensor();
	while(distance[0] <= 102) {
		PORTD |= (1<<1); 
		stop();
		sensor();
		_delay_ms(500);
	}
	PORTD &= ~(1<<1);
	while(distance[0] <= 102) {
		left_turn();
		distance[0] = 0;
		sensor();	
		//distance[0] = 0;		
		//show distance on seg display to see it
	}
	straight();
	
	//attempt 2
	if (distance[0] <= 102) {
		PORTD |= (1<<6);
		left_turn();
	}
	else {
		PORTD &= ~(1<<6);
		straight();
	}
	if (distance[1] <= 102) {
		PORTD |= (1<<4);
	}
	else {
		PORTD &= ~(1<<4);
	}
	_delay_ms(100);
*/

void straight() {
	shift_reg = FORWARD;
	update_IO();
}

void backward() {
	shift_reg = BACKWARDS;
	update_IO();
}

void stop() {
	shift_reg = BREAK;
	update_IO();
}

void right_turn() {
	shift_reg = RIGHT;
	update_IO();
}

void left_turn() {
	shift_reg = LEFT;
	update_IO();
}

void output_byte(uint8_t byte) {
	uint8_t i;
	for(i=1; i!=0; i=i<<1) {
		if(byte & i) {
			PORTB |= (1<<2);
		}
		else {
			PORTB &= ~(1<<2);
		}
		PORTB |= (1<<1);
		PORTB &= ~(1<<1);
	}
}

//Function that calls output_byte and pulses the clock of the extra data register
void update_IO() {
	output_byte(shift_reg);
	
	PORTB |= (1<<0);
	PORTB &= ~(1<<0);
}

/*
void check_sensor() {
	if (distance[0] <= 153) {
		left_sensor = 1;
		PORTD |= (1<<6); //LED 3
	}
	else {
		left_sensor = 0;
		PORTD &= ~(1<<6);
	}
	if (distance[1] <= 153) {
		front_sensor = 1;
		PORTD |= (1<<4); //LED 4
	}
	else {
		front_sensor = 0;
		PORTD &= ~(1<<4);
	}
	if (distance[2] <= 153) {
		right_sensor = 1;
		PORTD |= (1<<1); //LED 5
	}
	else {
		right_sensor = 0;
		PORTD &= ~(1<<1);
	}	
}
*/

/*
void call_left_sensor() {
	distance[0] = 0;
	PORTD |= (1<<2);
	_delay_us(15);
	PORTD &= ~(1<<2);
	while (!(PIND & (1<<7))){}
	while((PIND & (1<<7)) && (distance[0] < 255)) {
		_delay_us(7);
		distance[0]++;
	}
}

void call_front_sensor() {
	distance[1] = 0;
	PORTD |= (1<<2);
	_delay_us(15);
	PORTD &= ~(1<<2);
	while (!(PINB & (1<<4))){}
	while((PINB & (1<<4)) && (distance[1] < 255)) {
		_delay_us(7);
		distance[1]++;
	}
}

void call_right_sensor() {
	distance[0] = 0;
	PORTD |= (1<<2);
	_delay_us(15);
	PORTD &= ~(1<<2);
	while (!(PINB & (1<<5))){}
	while((PINB & (1<<5)) && (distance[2] < 255)) {
		_delay_us(7);
		distance[2]++;
	}
}

*/
void call_sensor() {
	distance[0] = 0;
	distance[1] = 0;
	distance[2] = 0;
	PORTD |= (1<<2);
	_delay_us(15);
	PORTD &= ~(1<<2);
	//sw4
	//Wait for echo = 1
	while ((!(PIND & (1<<7))) && (!(PINB & (1<<4))) && (!(PINB & (1<<5)))){} 
	
	//Measure echo for distance
	while(((PIND & (1<<7)) && (distance[0] <= 255)) || ((PINB & (1<<4)) && (distance[1] <= 255)) || ((PINB & (1<<5)) && (distance[2] <= 255))) {
		_delay_us(7);
		if ((PIND & (1<<7)) && (distance[0] <= 255)) {
			distance[0]++;
		}
		if ((PINB & (1<<4)) && (distance[1] <= 255)) {
			distance[1]++;
		}
		if ((PINB & (1<<5)) && (distance[2] <= 255)) {
			distance[2]++;
		}
	}
}
