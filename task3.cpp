#include "task3.h"
void setup_task3(void){

  DDRD  &= ~(1 << DDD2); // PD2 input
  PORTD |=  (1 << PD2);  // Pull up -> stare neapasata este activa

  //setez pentru pinul int01 (d2) intreruperea pe FALLING EDGE
  EICRA |=  (1 << ISC01);
  EICRA &= ~(1 << ISC00);

  //pentru a da enable la intreruperea de pe pinul INT0
  EIFR  |= (1 << INTF0); //scriu unu pentru a sterge flag ul setet posibil anterior (set for clear)
  EIMSK |= (1<<INT0);

  //pinul 7 il setez ca output cu valoarea initiala low
  DDRD |= (1<<DDD7);
  PORTD &= ~(1<<PD7);

  //enable la intreruperi (GLOBAL)
  SREG |= (1<<7);
}
  ISR(INT0_vect){
    PORTD ^= (1 << PD7);
  }

void task3(void){
}