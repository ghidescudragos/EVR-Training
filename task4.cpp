#include "task4.h"
static enum states cs = ST0;
static enum states ns = ST0;

volatile bool flag = false;
void setup_task4(void) {
  // definesc porturile pentru aprinderea led-urilor
  setup_task1();
  // ma asigur ca timerul este oprit cat timp fac modificarile si ca sterg configurarile anterioare
  TCCR1B = 0; TCCR1A = 0;
  TCNT1 = 0;
 
  // OCRA1H
  // 0011_1101_0000_1001
  // OCR1A[15:8] = 0b00111101;
  // // OCRA1L 
  // OCR1A[7:0] = 0b00001001;
  OCR1A = 15624; 
 
  // NU FOLOSESC TCNT0 pentru ca ESTE FOLOSIT de milis() in task2 pentru debounce !!!
  // folosesc TCNT1
  // ma asigur ca pinii de outpun nu sunt setati
  TCCR1A &=~(1<<COM1A1 | 1<<COM1A0 | 1<<COM1B1 | 1<<COM1B0);

  // setez modul CTC si setez MAX ca fiind valoarea din OCR1A 
  TCCR1B &=~(1<<WGM13);
  TCCR1B |=(1<<WGM12);
  TCCR1A &=~(1<<WGM11 | 1<<WGM10);

  // setez prescaler pe 256 si astfel trebuie sa am in OCR1A val (in baza 10) 15625 (4000000/256, unde 4*10^6 = nr. de cloci pe o perioada de 0.25 s)
  TCCR1B |=(1<<CS12);
  TCCR1B &=~(1<<CS11 | 1<<CS10);


  // ma asigur ca intrerupt flag este cleared
  TIFR1 |= (1<<OCF1A);
  // dau enable la intreruperi realizate de acest timer TCNT1 
  TIMSK1 |=(1<<OCIE1A);
  // ma asigur ca nu se activeaza intreruperea la overflow
  TIMSK1 &=~(1<<TOIE1);

  // enable la intreruperi (GLOBAL)
  SREG |= (1<<7);
}
  ISR(TIMER1_COMPA_vect){
    flag = true;
  }

void task4(void) {
  if (flag) {
    flag = false;
    switch (cs) {
      case ST0:
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        ns = ST1;
        break;

      case ST1:
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        PORTB |= (1 << PB5);
        ns = ST2;
        break;

      case ST2:
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        PORTB |= (1 << PB5) | (1 << PB4);
        ns = ST3;
        break;

      case ST3:
        PORTB &= ~((1 << PB5) | (1 << PB4) | (1 << PB3));
        PORTB |= (1 << PB5) | (1 << PB4) | (1 << PB3);
        ns = ST0;
        break;
    }
    cs = ns;
  }
}