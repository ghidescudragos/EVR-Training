#include "task2.h"
static enum stari_buton stare = NEAPASAT;
static unsigned long previousMillis = 0;
static const unsigned long interval = 150;

void setup_task2(void){
  DDRB  &= ~(1 << DDB2); // D10 input
  PORTB |=  (1 << PB2);  // Pull up -> stare neapasata este activa


  DDRB |= (1 << DDB1) | (1<< DDB0); //D09 si D08 output
  PORTB |= (1<<PB1); // D09 aprins
  PORTB &= ~(1<<PB0); // DO8 stins
}

void task2(void) {
  uint8_t i = (PINB >> PB2) & 1;
  unsigned long currentMillis = millis();
  //la fiecare 50 de milisecunde ca sa nu efectueaza schimbarea de foarte multe ori (poate tin mult apasat pe buton)
  if(currentMillis - previousMillis >= interval){

    if (i == 0 && stare == NEAPASAT) { 
      PORTB &= ~(1 << PB1);
      PORTB |=  (1 << PB0);
      stare = APASAT;
    } else if(i == 0 && stare == APASAT) {      
      PORTB |=  (1 << PB1);
      PORTB &= ~(1 << PB0);
      stare = NEAPASAT;
    }
    previousMillis = currentMillis;

  }
}