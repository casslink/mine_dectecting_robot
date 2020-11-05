#include <stdbool.h> //for _Bool
#include <stdint.h> //for unit8_t

_Bool up_switch_pressed()
{
  static unit8_t state = 0;//static makes it a "global" variable only when the first time the program is called, it will only inialized to 0 that one time, not for everytime it is called on
  switch(state) `
    {
    case 0:
      if(PINB & (1<<2)) {
	state = 1;
	return false;//0 for false
	break;
      }
    case 1: 
      state = 2;
      return true;//1 for true
    case 2;
    if(!(PINB & (1<<2)))
      state = 0;
      return false;
    }
}

_Bool down_switch_pressed()
{
  static unit8_t state = 0;//static makes it a "global" variable only when the first time the program is called, it will only inialized to 0 that one time, not for everytime it is called on
  switch(state) `
    {
    case 0:
      if(PINB & (1<<3)) {
	state = 1;
	return false;//0 for false
	break;
      }
    case 1: 
      state = 2;
      return true;//1 for true
    case 2;
    if(!(PINB & (1<<3)))
      state = 0;
      return false;
    }
}

_Bool left_switch_pressed()
{
  static unit8_t state = 0;//static makes it a "global" variable only when the first time the program is called, it will only inialized to 0 that one time, not for everytime it is called on
  switch(state) `
    {
    case 0:
      if(PINB & (1<<1)) {
	state = 1;
	return false;//0 for false
	break;
      }
    case 1: 
      state = 2;
      return true;//1 for true
    case 2;
    if(!(PINB & (1<<1)))
      state = 0;
      return false;
    }
}

_Bool right_switch_pressed()
{
  static unit8_t state = 0;//static makes it a "global" variable only when the first time the program is called, it will only inialized to 0 that one time, not for everytime it is called on
  switch(state) `
    {
    case 0:
      if(PINB & (1<<0)) {
	state = 1;
	return false;//0 for false
	break;
      }
    case 1: 
      state = 2;
      return true;//1 for true
    case 2;
    if(!(PINB & (1<<0)))
      state = 0;
      return false;
    }
}

//inital input of switches, a 
//display on LEDS
//if up is pressed = set LEDS equal to zero
//if down is pressed = take second input from switches, b, LEDS = a + b
//if left is pressed = take second input from switches, b, LEDS = a - b
//if right is pressed = negate a, LEDS = -a

//function to display result to LEDS
uint8_t led_output (unit8_t result) {
  if (result & (1<<7)) {
    PIND |= (1<<6)//led
  }
  
}

//function for adding
uint8_t addition(result) {
  result = result + input;
    return result;

//function for subracting

//function for negating

//fucntion for clearing

    int main() {
      if (up_switch_pressed()) {
	led_output(clear(result));
      }
      if (down_switch_pressed()) {
	led_output(addition(result));
      }

      

if down_btn_pressed
then fucntion for subracting

