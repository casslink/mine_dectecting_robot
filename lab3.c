#include <avr/interrupt.h>
//call sei() to enable CPU interrupts after configuring
//in push button config enable pin change interrupts PCICR |= (1<<PCIE0)
//for each push button 
//PCMSK0 |= (1<<3); // PB3 pinchange

//if g_operation |= 0 than turn on LEDs to debug if not working 

//function signatures
enum operators {OP_NONE, OP_CLEAR, OP_ADD, OP_SUBTRACT, OP_NEGATE };

ISR(PCINT0_vect) {
  //assume one button pressed at time
  if (PINB & (1<<0)) //right
    g_operation = OP_ADD; //global variable
  if (PINB & (1<<1)) //left
    g_operation = OP_SUBTRACT
  //add other ifs
  if (PINB & (1<<3)) //down
    g_operation = OP_CLEAR;
 }

enum operators g_operation;
int main() {
  config_buttons();
  config_switches();
  config_LEDS();

  enum operators operation;
  uint8_t resutl = 0;
  while(1) {
    /*    operation = OP_NONE;
    if up_btn_press() {
	operation = OP_NEGATE;
      }
    if left_btn_press() {
	opereration = OP_SUBTRACT;
      }
    if right_btn_press() {
	opereration = OP_ADD;
      }       
    if down_btn_press() {
	opereration = OP_CLEAR;
    */
      }

    switch(g_operation) {
    case OP_ADD:
    result = result + get_switches();
    break;
    case OP_NEGATE:
    result = -result;
    break;
    //add cases for two other operations
    
    case OP_NONE:
    default:
      break;

    }
    output_to_LEDS(result);
   g_operation - OP_NONE;
}
    
/*
Friday Feb 15
-------------------------------------NOTES________________________________
I/O Synchronization

Coordinate software w/ hardware events
3 methods
1. Polling - repeatedly check (ex: phone is dumb device you need to constantly check if someone is on the other line by picking up the phone over and over again)
2. Delay - delays to match timing (ex: you coordinate, “call me in exactly one hour and i will pick up phone then)
3. Interrupt - peripheral signal CPU (ex: put a ringer on the phone and when you hear it you interrupt what you're doing and pick up the phone) 
CPU has request lines
Each is associated with function pointer (vector) pointing to “service routine”
Pin Change Interrupt

One for each pin on PORTB
PORTB had 8 bits, and if anyone of those 8 bits changes an interrupt is generated
Vector PCINT0_vect holds address of service_routine 
pin change interrupt 0
C-syntax to define ISR (seperate function outside of main)
ISR(PCINT0_vect) {
//assume one button pressed at time
if (PINB & (1<<0)) //right
g_operation = OP_ADD; //global variable
If (PINB & (1<<1)) //left
g_operation = OP_SUBTRACT
…
if (PINB & (1<<3)) //down
g_operation = OP_CLEAR; }
No arguments passed through, it is an argument to the macro since you do not know when the interrupt is going to happen you cannot pass an argument
*/
