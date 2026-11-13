#include "task6.h"
void setup_task6(void) {
  // Folosesc TCNT0, dar nu reset3ez complet configuratiile sale pentru ca este folosit de CPU pentru milis() si este deja setat in fast PWM mode cu prescaler de 64 si ca TOP foloseste 0xFF

  // Ma asigur ca nu se realizeaza nicio intrerupere pe compare match cu OCR0A SAU OCR0B
  TIMSK0 &= ~(1 << OCIE0A | 1 << OCIE0B);

  // Ma asigur ca OC0B este deconectat
  TCCR0A &= ~(1 << COM0B1 | 1 << COM0B0);


  // Folosesc Pin ul PC0/ADC[0] (D14) pentru a citi tensiunea potentiometrului
  // St. potentiometrului o leg la ground, mijlocul la pinul PC0, iar dr. pot. la 5V OUT

  // Selectez referinta interna de 5V AVcc
  ADMUX = 0;
  ADMUX |= (1 << REFS0);
  ADMUX &= ~(1 << REFS1);

  // Right adjust 
  ADMUX &= ~(1 << ADLAR);

  // Selectez ADC[0] ca intrare pentru ADC
  ADMUX &= ~(1 <<MUX3 | 1 <<MUX1 | 1 <<MUX2 | 1 <<MUX0);

  // Ma asigur ca IF este cleared scriind 1 logic in el
  ADCSRA |= (1<<ADIF);

  // Ma asigur ca buffer ul digital pentru ADC[0] este dezactivat
  DIDR0 |= (1<<ADC0D);

  /* Setez prescaler pentru a micsora frecventa de achizitie ca acel condensator folosit pt. sample and hold sa aiba timp sa se incarce 
    (frecventa unei conversii aprox. egala cu 9615Hz = (16*10^6/128)/13, unde 13 aprox. egal nr. cicluri ADC pt. o conversie)
  */  
  ADCSRA |= (1<<ADPS2 | 1<<ADPS1 | 1<<ADPS0);

  // Setez modul free running 
  ADCSRB &= ~(1<<ADTS2 | 1<<ADTS1 | 1<<ADTS0);

  // Dau enable la auto triggering
  ADCSRA |= (1<<ADATE);

  // Pornesc intreruperea de final de conversie in care voi citi valoarea din ADCH si o sa o pun in OCR0A
  // ADCSRA |= (1<<ADIE);

  // Pornesc ADC
  ADCSRA |= (1<<ADEN);
  
  // Ii spun sa inceapa prima conversie
  ADCSRA |= (1<<ADSC);

  // Dau enable la intreruperi globare in celelalte task uri, deci in task6 bitul I este deja setat
  // SREG |= (1<<7);

  // Setez valoarea lui OCR0A cu 0(ma asigur ca iesirea este LOW inainte de prima actualizare a lui OCR0A), valoare pe care o actualizez la fiecare citire a ADC>>2
  OCR0A = 0;


  // Setez directia pentru pinul PD6 ca output
  DDRD |= 1<<DDD6; 

  // Setez iesirea pentru pinul OC0A (PD6) pe clear on compare math with OCR0A
  TCCR0A |= (1 << COM0A1);
  TCCR0A &= ~(1 << COM0A0);
}

// ISR-ul 
// ISR (ADC_vect){
//   OCR0A =  (uint8_t) (ADC>>2);
// }

void task6(void) {
    OCR0A =  (uint8_t) (ADC>>2); 
}